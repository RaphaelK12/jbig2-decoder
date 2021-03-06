// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
 
// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef _FXCRT_COORDINATES_
#define _FXCRT_COORDINATES_
template<class baseType> class CFX_PSVTemplate;
template<class baseType> class CFX_VTemplate;
template<class baseType> class CFX_PRLTemplate;
template<class baseType> class CFX_RTemplate;
template<class baseType> class CFX_ETemplate;
template<class baseType> class CFX_ATemplate;
template<class baseType> class CFX_RRTemplate;
class CFX_Matrix;
template<class baseType>
class CFX_PSVTemplate : public CFX_Object
{
public:
    typedef CFX_PSVTemplate<baseType>	FXT_PSV;
    typedef CFX_PSVTemplate<baseType>	FXT_POINT;
    typedef CFX_PSVTemplate<baseType>	FXT_SIZE;
    void		Set(baseType x, baseType y)
    {
        FXT_PSV::x = x, FXT_PSV::y = y;
    }
    void		Set(const FXT_PSV &psv)
    {
        FXT_PSV::x = psv.x, FXT_PSV::y = psv.y;
    }
    void		Add(baseType x, baseType y)
    {
        FXT_PSV::x += x, FXT_PSV::y += y;
    }
    void		Subtract(baseType x, baseType y)
    {
        FXT_PSV::x -= x, FXT_PSV::y -= y;
    }
    void		Reset()
    {
        FXT_PSV::x = FXT_PSV::y = 0;
    }
    FXT_PSV&	operator += (const FXT_PSV &obj)
    {
        x += obj.x;
        y += obj.y;
        return *this;
    }
    FXT_PSV&	operator -= (const FXT_PSV &obj)
    {
        x -= obj.x;
        y -= obj.y;
        return *this;
    }
    FXT_PSV&	operator *= (baseType lamda)
    {
        x *= lamda;
        y *= lamda;
        return *this;
    }
    FXT_PSV&	operator /= (baseType lamda)
    {
        x /= lamda;
        y /= lamda;
        return *this;
    }
    friend	FX_BOOL		operator == (const FXT_PSV &obj1, const FXT_PSV &obj2)
    {
        return obj1.x == obj2.x && obj1.y == obj2.y;
    }
    friend	FX_BOOL		operator != (const FXT_PSV &obj1, const FXT_PSV &obj2)
    {
        return obj1.x != obj2.x || obj1.y != obj2.y;
    }
    friend	FXT_PSV		operator + (const FXT_PSV &obj1, const FXT_PSV &obj2)
    {
        CFX_PSVTemplate obj;
        obj.x = obj1.x + obj2.x;
        obj.y = obj1.y + obj2.y;
        return obj;
    }
    friend	FXT_PSV		operator - (const FXT_PSV &obj1, const FXT_PSV &obj2)
    {
        CFX_PSVTemplate obj;
        obj.x = obj1.x - obj2.x;
        obj.y = obj1.y - obj2.y;
        return obj;
    }
    friend	FXT_PSV		operator * (const FXT_PSV &obj, baseType lamda)
    {
        CFX_PSVTemplate t;
        t.x = obj.x * lamda;
        t.y = obj.y * lamda;
        return t;
    }
    friend	FXT_PSV		operator * (baseType lamda, const FXT_PSV &obj)
    {
        CFX_PSVTemplate t;
        t.x = lamda * obj.x;
        t.y = lamda * obj.y;
        return t;
    }
    friend	FXT_PSV		operator / (const FXT_PSV &obj, baseType lamda)
    {
        CFX_PSVTemplate t;
        t.x = obj.x / lamda;
        t.y = obj.y / lamda;
        return t;
    }
    baseType x, y;
};
typedef CFX_PSVTemplate<FX_INT32>			CFX_Point;
typedef CFX_PSVTemplate<FX_FLOAT>			CFX_PointF;
typedef CFX_PSVTemplate<FX_INT32>			CFX_Size;
typedef CFX_PSVTemplate<FX_FLOAT>			CFX_SizeF;
//typedef CFX_ArrayTemplate<CFX_Point>		CFX_Points;
//typedef CFX_ArrayTemplate<CFX_PointF>		CFX_PointsF;
typedef CFX_PSVTemplate<FX_INT32> *			FX_LPPOINT;
typedef CFX_PSVTemplate<FX_FLOAT> *			FX_LPPOINTF;
typedef CFX_PSVTemplate<FX_INT32> const *	FX_LPCPOINT;
typedef CFX_PSVTemplate<FX_FLOAT> const *	FX_LPCPOINTF;
#define CFX_FloatPoint	CFX_PointF
template<class baseType>
class CFX_VTemplate: public CFX_PSVTemplate<baseType>
{
public:
    typedef CFX_PSVTemplate<baseType>	FXT_PSV;
    typedef CFX_PSVTemplate<baseType>	FXT_POINT;
    typedef CFX_PSVTemplate<baseType>	FXT_SIZE;
    typedef CFX_VTemplate<baseType>		FXT_VECTOR;
    void		Set(baseType x, baseType y)
    {
        FXT_PSV::x = x, FXT_PSV::y = y;
    }
    void		Set(const FXT_PSV &psv)
    {
        FXT_PSV::x = psv.x, FXT_PSV::y = psv.y;
    }
    void		Set(const FXT_POINT &p1, const FXT_POINT &p2)
    {
        FXT_PSV::x = p2.x - p1.x, FXT_PSV::y = p2.y - p1.y;
    }
    void		Reset()
    {
        FXT_PSV::x = FXT_PSV::y = 0;
    }
    baseType	SquareLength() const
    {
        return FXT_PSV::x * FXT_PSV::x + FXT_PSV::y * FXT_PSV::y;
    }
    baseType	Length() const
    {
        return FXSYS_sqrt(FXT_PSV::x * FXT_PSV::x + FXT_PSV::y * FXT_PSV::y);
    }
    void		Normalize()
    {
        FX_FLOAT fLen = FXSYS_sqrt(FXT_PSV::x * FXT_PSV::x + FXT_PSV::y * FXT_PSV::y);
        if (fLen < 0.0001f) {
            return;
        }
        FXT_PSV::x = ((baseType)FXT_PSV::x) / fLen;
        FXT_PSV::y = ((baseType)FXT_PSV::y) / fLen;
    }
    baseType	DotProduct(baseType x, baseType y) const
    {
        return FXT_PSV::x * x + FXT_PSV::y * y;
    }
    baseType	DotProduct(const FXT_VECTOR &v) const
    {
        return FXT_PSV::x * v.x + FXT_PSV::y * v.y;
    }
    FX_BOOL		IsParallel(baseType x, baseType y) const
    {
        baseType t = FXT_PSV::x * y - FXT_PSV::y * x;
        return FXSYS_fabs(t) < 0x0001f;
    }
    FX_BOOL		IsParallel(const FXT_VECTOR &v) const
    {
        return IsParallel(v.x, v.y);
    }
    FX_BOOL		IsPerpendicular(baseType x, baseType y) const
    {
        baseType t = DotProduct(x, y);
        return FXSYS_fabs(t) < 0x0001f;
    }
    FX_BOOL		IsPerpendicular(const FXT_VECTOR &v) const
    {
        return IsPerpendicular(v.x, v.y);
    }
    void		Translate(baseType dx, baseType dy)
    {
        FXT_PSV::x += dx, FXT_PSV::y += dy;
    }
    void		Scale(baseType sx, baseType sy)
    {
        FXT_PSV::x *= sx, FXT_PSV::y *= sy;
    }
    void		Rotate(FX_FLOAT fRadian)
    {
        FX_FLOAT xx = (FX_FLOAT)FXT_PSV::x;
        FX_FLOAT yy = (FX_FLOAT)FXT_PSV::y;
        FX_FLOAT cosValue = FXSYS_cos(fRadian);
        FX_FLOAT sinValue = FXSYS_sin(fRadian);
        FXT_PSV::x = xx * cosValue - yy * sinValue;
        FXT_PSV::y = xx * sinValue + yy * cosValue;
    }
    friend	FX_FLOAT	Cosine(const FXT_VECTOR &v1, const FXT_VECTOR &v2)
    {
        FXSYS_assert(v1.SquareLength() != 0 && v2.SquareLength() != 0);
        FX_FLOAT dotProduct = v1.DotProduct(v2);
        return dotProduct / (FX_FLOAT)FXSYS_sqrt(v1.SquareLength() * v2.SquareLength());
    }
    friend	FX_FLOAT	ArcCosine(const FXT_VECTOR &v1, const FXT_VECTOR &v2)
    {
        return (FX_FLOAT)FXSYS_acos(Cosine(v1, v2));
    }
    friend	FX_FLOAT	SlopeAngle(const FXT_VECTOR &v)
    {
        CFX_VTemplate vx;
        vx.Set(1, 0);
        FX_FLOAT fSlope = ArcCosine(v, vx);
        return v.y < 0 ? -fSlope : fSlope;
    }
};
typedef CFX_VTemplate<FX_INT32> CFX_Vector;
typedef CFX_VTemplate<FX_FLOAT> CFX_VectorF;
template<class baseType>
class CFX_RTemplate: public CFX_Object
{
public:
    typedef CFX_PSVTemplate<baseType>	FXT_POINT;
    typedef CFX_PSVTemplate<baseType>	FXT_SIZE;
    typedef CFX_VTemplate<baseType>		FXT_VECTOR;
    typedef CFX_PRLTemplate<baseType>	FXT_PARAL;
    typedef CFX_RTemplate<baseType>		FXT_RECT;
    void		Set(baseType left, baseType top, baseType width, baseType height)
    {
        FXT_RECT::left = left, FXT_RECT::top = top, FXT_RECT::width = width, FXT_RECT::height = height;
    }
    void		Set(baseType left, baseType top, const FXT_SIZE &size)
    {
        FXT_RECT::left = left, FXT_RECT::top = top, FXT_RECT::Size(size);
    }
    void		Set(const FXT_POINT &p, baseType width, baseType height)
    {
        TopLeft(p), FXT_RECT::width = width, FXT_RECT::height = height;
    }
    void		Set(const FXT_POINT &p1, const FXT_POINT &p2)
    {
        TopLeft(p1), FXT_RECT::width = p2.x - p1.x, FXT_RECT::height = p2.y - p1.y, FXT_RECT::Normalize();
    }
    void		Set(const FXT_POINT &p, const FXT_VECTOR &v)
    {
        TopLeft(p), FXT_RECT::width = v.x, FXT_RECT::height = v.y, FXT_RECT::Normalize();
    }
    void		Reset()
    {
        FXT_RECT::left = FXT_RECT::top = FXT_RECT::width = FXT_RECT::height = 0;
    }
    FXT_RECT&	operator += (const FXT_POINT &p)
    {
        left += p.x, top += p.y;
        return *this;
    }
    FXT_RECT&	operator -= (const FXT_POINT &p)
    {
        left -= p.x, top -= p.y;
        return *this;
    }
    baseType	right() const
    {
        return left + width;
    }
    baseType	bottom() const
    {
        return top + height;
    }
    void		Normalize()
    {
        if (width < 0) {
            left += width;
            width = -width;
        }
        if (height < 0) {
            top += height;
            height = -height;
        }
    }
    void		Offset(baseType dx, baseType dy)
    {
        left += dx;
        top += dy;
    }
    void		Inflate(baseType x, baseType y)
    {
        left -= x;
        width += x * 2;
        top -= y;
        height += y * 2;
    }
    void		Inflate(const FXT_POINT &p)
    {
        Inflate(p.x, p.y);
    }
    void		Inflate(baseType left, baseType top, baseType right, baseType bottom)
    {
        FXT_RECT::left -= left;
        FXT_RECT::top -= top;
        FXT_RECT::width += left + right;
        FXT_RECT::height += top + bottom;
    }
    void		Inflate(const FXT_RECT &rt)
    {
        Inflate(rt.left, rt.top, rt.left + rt.width, rt.top + rt.height);
    }
    void		Deflate(baseType x, baseType y)
    {
        left += x;
        width -= x * 2;
        top += y;
        height -= y * 2;
    }
    void		Deflate(const FXT_POINT &p)
    {
        Deflate(p.x, p.y);
    }
    void		Deflate(baseType left, baseType top, baseType right, baseType bottom)
    {
        FXT_RECT::left += left;
        FXT_RECT::top += top;
        FXT_RECT::width -= left + right;
        FXT_RECT::height -= top + bottom;
    }
    void		Deflate(const FXT_RECT &rt)
    {
        Deflate(rt.left, rt.top, rt.top + rt.width, rt.top + rt.height);
    }
    FX_BOOL		IsEmpty() const
    {
        return width <= 0 || height <= 0;
    }
    FX_BOOL		IsEmpty(FX_FLOAT fEpsilon) const
    {
        return width <= fEpsilon || height <= fEpsilon;
    }
    void		Empty()
    {
        width = height = 0;
    }
    FX_BOOL		Contains(baseType x, baseType y) const
    {
        return x >= left && x < left + width && y >= top && y < top + height;
    }
    FX_BOOL		Contains(const FXT_POINT &p) const
    {
        return Contains(p.x, p.y);
    }
    FX_BOOL		Contains(const FXT_RECT &rt) const
    {
        return rt.left >= left && rt.right() <= right() && rt.top >= top && rt.bottom() <= bottom();
    }
    baseType	Width() const
    {
        return width;
    }
    baseType	Height() const
    {
        return height;
    }
    FXT_SIZE	Size() const
    {
        FXT_SIZE size;
        size.Set(width, height);
        return size;
    }
    void		Size(FXT_SIZE s)
    {
        width = s.x, height = s.y;
    }
    FXT_POINT	TopLeft() const
    {
        FXT_POINT p;
        p.x = left;
        p.y = top;
        return p;
    }
    FXT_POINT	TopRight() const
    {
        FXT_POINT p;
        p.x = left + width;
        p.y = top;
        return p;
    }
    FXT_POINT	BottomLeft() const
    {
        FXT_POINT p;
        p.x = left;
        p.y = top + height;
        return p;
    }
    FXT_POINT	BottomRight() const
    {
        FXT_POINT p;
        p.x = left + width;
        p.y = top + height;
        return p;
    }
    void		TopLeft(FXT_POINT tl)
    {
        left = tl.x;
        top = tl.y;
    }
    void		TopRight(FXT_POINT tr)
    {
        width = tr.x - left;
        top = tr.y;
    }
    void		BottomLeft(FXT_POINT bl)
    {
        left = bl.x;
        height = bl.y - top;
    }
    void		BottomRight(FXT_POINT br)
    {
        width = br.x - left;
        height = br.y - top;
    }
    FXT_POINT	Center() const
    {
        FXT_POINT p;
        p.x = left + width / 2;
        p.y = top + height / 2;
        return p;
    }
    void		GetParallelogram(FXT_PARAL &pg) const
    {
        pg.x = left, pg.y = top;
        pg.x1 = width, pg.y1 = 0;
        pg.x2 = 0, pg.y2 = height;
    }
    void		Union(baseType x, baseType y)
    {
        baseType r = right(), b = bottom();
        if (left > x) {
            left = x;
        }
        if (r < x) {
            r = x;
        }
        if (top > y) {
            top = y;
        }
        if (b < y) {
            b = y;
        }
        width = r - left;
        height = b - top;
    }
    void		Union(const FXT_POINT &p)
    {
        Union(p.x, p.y);
    }
    void		Union(const FXT_RECT &rt)
    {
        baseType r = right(), b = bottom();
        if (left > rt.left) {
            left = rt.left;
        }
        if (r < rt.right()) {
            r = rt.right();
        }
        if (top > rt.top) {
            top = rt.top;
        }
        if (b < rt.bottom()) {
            b = rt.bottom();
        }
        width = r - left;
        height = b - top;
    }
    void		Intersect(const FXT_RECT &rt)
    {
        baseType r = right(), b = bottom();
        if (left < rt.left) {
            left = rt.left;
        }
        if (r > rt.right()) {
            r = rt.right();
        }
        if (top < rt.top) {
            top = rt.top;
        }
        if (b > rt.bottom()) {
            b = rt.bottom();
        }
        width = r - left;
        height = b - top;
    }
    FX_BOOL		IntersectWith(const FXT_RECT &rt) const
    {
        FXT_RECT rect = rt;
        rect.Intersect(*this);
        return !rect.IsEmpty();
    }
    FX_BOOL		IntersectWith(const FXT_RECT &rt, FX_FLOAT fEpsilon) const
    {
        FXT_RECT rect = rt;
        rect.Intersect(*this);
        return !rect.IsEmpty(fEpsilon);
    }
    friend	FX_BOOL	operator == (const FXT_RECT &rc1, const FXT_RECT &rc2)
    {
        return rc1.left == rc2.left && rc1.top == rc2.top && rc1.width == rc2.width && rc1.height == rc2.height;
    }
    friend	FX_BOOL	operator != (const FXT_RECT &rc1, const FXT_RECT &rc2)
    {
        return rc1.left != rc2.left || rc1.top != rc2.top || rc1.width != rc2.width || rc1.height != rc2.height;
    }
    baseType left, top;
    baseType width, height;
};
typedef CFX_RTemplate<FX_INT32>			CFX_Rect;
typedef CFX_RTemplate<FX_FLOAT>			CFX_RectF;
typedef CFX_RTemplate<FX_INT32> *		FX_LPRECT;
typedef CFX_RTemplate<FX_FLOAT> *		FX_LPRECTF;
typedef CFX_RTemplate<FX_INT32> const *	FX_LPCRECT;
typedef CFX_RTemplate<FX_FLOAT> const *	FX_LPCRECTF;
struct FX_RECT {

    int			left;

    int			top;

    int			right;

    int			bottom;

    FX_RECT() {}

    FX_RECT(int left1, int top1, int right1, int bottom1)
    {
        left = left1;
        top = top1;
        right = right1;
        bottom = bottom1;
    }

    int			Width() const
    {
        return right - left;
    }

    int			Height() const
    {
        return bottom - top;
    }

    FX_BOOL		IsEmpty() const
    {
        return right <= left || bottom <= top;
    }

    FX_BOOL		operator == (const FX_RECT& src) const
    {
        return left == src.left && right == src.right && top == src.top && bottom == src.bottom;
    }

    void		Offset(int dx, int dy)
    {
        left += dx;
        right += dx;
        top += dy;
        bottom += dy;
    }

    FX_BOOL		Contains(const FX_RECT& other_rect) const
    {
        return other_rect.left >= left && other_rect.right <= right && other_rect.top >= top && other_rect.bottom <= bottom;
    }

    FX_BOOL		Contains(int x, int y) const
    {
        return x >= left && x < right && y >= top && y < bottom;
    }
};

#endif
