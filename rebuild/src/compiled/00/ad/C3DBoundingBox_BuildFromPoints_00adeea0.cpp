// C3DBoundingBox::BuildFromPoints(const std::vector<C3DVector>& points) -- vector overload: forwards the element array and its (float) count to the pointer overload
// Leaf: ?BuildFromPoints@C3DBoundingBox@@QAEXABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADEEA0, 60 bytes, __fastcall, ret 4.
// Identity: Ego_r.pdb and FableWin.pdb both name this overload, and the pointer overload it calls (0x00ADED70) as
//   ?BuildFromPoints@C3DBoundingBox@@QAEXPBVC3DVector@@M@Z -- the count really is a float there, hence the unsigned
//   size() -> float conversion (fild + 2^32 fixup from 0x0122DCB4). Caller 0x00AC2DC3 passes this = owner+8, beside the
//   sphere overload 0x00ADDF70 on owner+0x20.
// Layout: Ego_r.pdb C3DVector (12: X,Y,Z), C3DBoundingSphere (16: Center, Radius), C3DBoundingBox (24: MinCoord, MaxCoord).
// No engine header exists for these classes; STLport 4.6 vector reduced to its three pointers (_Vector_base layout).
class C3DVector {
public:
    float X;
    float Y;
    float Z;
};
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    const _Tp& operator[](size_type __n) const { return *(_M_start + __n); }
};
}   // namespace std
class C3DBoundingSphere {
public:
    C3DVector Center;
    float     Radius;
    void AccurateBuildFromPoints(const C3DVector* points, long num_points, float tolerance);    // 0x00ADD920
    void AccurateBuildFromPoints(const std::vector<C3DVector>& points, float tolerance);
};
class C3DBoundingBox {
public:
    C3DVector MinCoord;
    C3DVector MaxCoord;
    void BuildFromPoints(const C3DVector* points, float num_points);                           // 0x00ADED70
    void BuildFromPoints(const std::vector<C3DVector>& points);
};

void C3DBoundingBox::BuildFromPoints(const std::vector<C3DVector>& points)
{
    BuildFromPoints(&points[0], points.size());
}