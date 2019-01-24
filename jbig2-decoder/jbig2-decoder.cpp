// jbig2-decoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include <iterator>
#include <chrono>
#include <tuple>
#include "jbig2/JBig2_Context.h"
#include "jbig2/JBig2_Image.h"
#include "support/module.h"
#include "support/getopt_pp.h" // https://code.google.com/archive/p/getoptpp/
#include "support/image_write.h"

std::vector<unsigned char> ReadFile(const std::string& filename);
std::tuple<std::string, std::string, ImageFormat> GetOutputInfo(const std::string&, const std::string&, const std::string&);
std::string GetImageFileName(const std::string&, const std::string&, int);

int main(int argc, char *argv[])
{
	GetOpt::GetOpt_pp ops(argc, argv);

	ops.exceptions(std::ios::failbit | std::ios::eofbit);
	std::string output_file;
	std::string output_format;
	std::string input_file;
	std::string input_global_stream_file;
	bool should_output_processing_time = false;
	bool has_global_stream = false;
	std::vector<std::string> args;

	try {
		ops >> GetOpt::Option('o', "output-file", output_file, "");
		ops >> GetOpt::Option('f', "format", output_format, "");
		ops >> GetOpt::OptionPresent('t', "time", should_output_processing_time);
		ops >> GetOpt::GlobalOption(args);
	} catch (GetOpt::GetOptEx ex) {
		std::cerr << "Error in arguments" << std::endl;
		return -1;
	}

	if (args.size() > 2) {
		if (args.size() > 0)
			std::cerr << "too many input files provided" << std::endl;
		return -1;
	}

	if (args.size() == 1) {
		input_file = args[0];
	} else {
		input_global_stream_file = args[0];
		input_file = args[1];
		has_global_stream = true;
	}

	JBig2Module module;

	auto data = ReadFile(input_file);
	FX_DWORD sz = static_cast<FX_DWORD>(data.size());

	CJBig2_Context* cntxt = nullptr;
	if (! has_global_stream) {
		cntxt = CJBig2_Context::CreateContext(&module, nullptr, 0,
			reinterpret_cast<FX_BYTE*>(&(data[0])), sz, JBIG2_FILE_STREAM);
	} else {
		auto global_data = ReadFile(input_global_stream_file);
		cntxt = CJBig2_Context::CreateContext(
			&module, 
			reinterpret_cast<FX_BYTE*>(&(global_data[0])), static_cast<FX_DWORD>(global_data.size()),
			reinterpret_cast<FX_BYTE*>(&(data[0])), sz, 
			JBIG2_EMBED_STREAM 
		);
	}
	if (!cntxt) {
		std::cerr << "Error creating jbig2 context" << std::endl;
		return -1;
	 }

	std::chrono::duration<double> elapsed_time;
	std::vector<CJBig2_Image*> pages;
	FX_INT32 result = JBIG2_SUCCESS;

	std::cout << "begin decoding...\n";

	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	while (cntxt->GetProcessiveStatus() == FXCODEC_STATUS_FRAME_READY && result == JBIG2_SUCCESS) {
		if (cntxt->GetProcessiveStatus() == FXCODEC_STATUS_ERROR)
			throw std::runtime_error("cntxt->GetProcessiveStatus() == FXCODEC_STATUS_ERROR");
		CJBig2_Image* img = nullptr;
		result = cntxt->getNextPage(&img, nullptr);
		if (img)
			pages.push_back(img);
	}
	std::cout << "decoding complete";

	elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
	if (should_output_processing_time)
		std::cout << "  in " << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() << " ms.\n";
	else
		std::cout << ".\n";

	auto output_file_info = GetOutputInfo(output_file, output_format, input_file);
	auto fname = std::get<0>(output_file_info);
	auto ext = std::get<1>(output_file_info);
	auto format = std::get<2>(output_file_info);

	for (int i = 0; i < pages.size(); i++) {
		auto out_file = GetImageFileName(fname, ext, (pages.size() == 1) ? 0 : i+1);
		std::cout << "  writing " << out_file << " ...\n";
		WriteImage(out_file, format, pages[i]);
	}

	return 0;
}

size_t GetFileSize(std::ifstream& file_stream)
{
	std::streampos sz;

	file_stream.seekg(0, std::ios::end);
	sz = file_stream.tellg();
	file_stream.seekg(0, std::ios::beg);

	return static_cast<size_t>(sz);
}

std::vector<unsigned char> ReadFile(const std::string& filename)
{
	// open the file:
	std::ifstream file(filename, std::ios::binary);
	file.unsetf(std::ios::skipws);
	
	auto num_bytes = GetFileSize(file);

	std::vector<unsigned char> vec;
	vec.reserve(num_bytes);
	
	// read the data:
	vec.insert(vec.begin(),
		std::istream_iterator<unsigned char>(file),
		std::istream_iterator<unsigned char>()
	);
	
	return vec;
}

std::pair<std::string,std::string> SplitFilenameAndExtension(const std::string& fname)
{
	auto last_dot = fname.find_last_of(".");
	if (last_dot == fname.npos)
		return std::pair<std::string, std::string>(fname, "");
	else
		return std::pair<std::string, std::string>(
			fname.substr(0, last_dot),
			fname.substr(last_dot + 1, fname.size() - last_dot)
		);
}

std::tuple<std::string, std::string, ImageFormat> GetOutputInfo(const std::string& out_fname, const std::string& format_str, const std::string& inp_fname)
{
	if (out_fname.empty()) {
		// if there was no output filename provided, the name of the .jb2 file.
		auto input = SplitFilenameAndExtension(inp_fname);
		ImageFormat format = (!format_str.empty()) ? GetFormatFromExtension(format_str) : ImageFormat::PNG;
		return std::tuple<std::string, std::string, ImageFormat>(input.first, GetExtensionFromFormat(format), format);
	}

	if (!format_str.empty()) {
		// if the filename doesnt have an extension use the format string. Otherwise, assume they know what they're doing.
		auto output = SplitFilenameAndExtension(out_fname);
		std::string ext = (output.second.empty()) ? format_str : ""; 
		return std::tuple<std::string, std::string, ImageFormat>(out_fname, ext, GetFormatFromExtension(format_str));
	}

	// if there is an output filename and no format option, use the extension of the output filename to determine
	// the desired format.
	auto output = SplitFilenameAndExtension(out_fname);
	if (output.second.empty())
		output.second = GetExtensionFromFormat(ImageFormat::PNG);

	return std::tuple<std::string, std::string, ImageFormat>(output.first, output.second, GetFormatFromExtension(output.second));
}

std::string GetImageFileName(const std::string& base, const std::string& ext, int index)
{
	if (index == 0)
		return base + "." + ext;
	return base + "-" + std::to_string(index) + "." + ext;
}