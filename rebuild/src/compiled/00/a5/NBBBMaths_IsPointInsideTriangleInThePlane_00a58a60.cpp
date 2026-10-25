// NBBBMaths::IsPointInsideTriangleInThePlane(const vector<C2DVector>& points, const C2DVector& pos) -- 2D point-in-triangle test: the cross product of every edge with the vector to the point must not change sign against the closing edge's
// Leaf: ?IsPointInsideTriangleInThePlane@NBBBMaths@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A58A60, 228 bytes, plain `ret` (PDB
//   ?IsPointInsideTriangleInThePlane@NBBBMaths@@YI_NABV?$vector@VC2DVector@@...@std@@ABVC2DVector@@@Z: `bool __fastcall`,
//   the vector in ecx, the point in edx; retail's vector is STLport's). No callees; 0.0f @ 0x0122DEDC.
//   Callers: CSphere / CCylinder::GetFirstIntersectionWithTriangle (0x00A5925E, 0x00A59D1D), with the triangle projected
//   into the plane by the transform 0x00A5A9D0.
// Shape: the closing edge (points[0] - points[2]) and the point offset are initialised named locals kept on the x87
//   stack for the reference cross product (`sign`, homed in the frame); inside the loop the same locals are ASSIGNED, so
//   each difference is built in a frame temporary and copied into the local before the cross product
//   `edge.X * toPos.Y - edge.Y * toPos.X`; the two sign-change tests are one `||` (the shared false exit sits at the end);
//   the loop test is `i != 2` (retail `cmp esi,2 ; jne`; `i < 2` gives `jl`).
namespace std {
template <class T> class allocator { };
template <class T, class Alloc = allocator<T> > class vector {
public:
    typedef unsigned int size_type;
    T* _M_start;                                        // +0x00
    T* _M_finish;                                       // +0x04
    T* _M_end_of_storage;                               // +0x08
    const T* begin() const { return _M_start; }
    const T& operator[](size_type n) const { return *(begin() + n); }
};
}   // namespace std

class C2DVector {
public:
    float X;                                            // +0x00
    float Y;                                            // +0x04
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};

namespace NBBBMaths {

bool __fastcall IsPointInsideTriangleInThePlane(const std::vector<C2DVector>& points, const C2DVector& pos)
{
    C2DVector edge = points[0] - points[2];
    C2DVector toPos = pos - points[2];
    float sign = edge.X * toPos.Y - edge.Y * toPos.X;
    for (long i = 0; i != 2; ++i) {
        edge = points[i + 1] - points[i];
        toPos = pos - points[i];
        float cross = edge.X * toPos.Y - edge.Y * toPos.X;
        if ((sign > 0.0f && cross < 0.0f) || (sign < 0.0f && cross > 0.0f))
            return false;
    }
    return true;
}

}   // namespace NBBBMaths