// CPhysicsMeshObject::GetNavigationLineList(std::vector<C2DLineF>& lines, long level) const -- append the physics mesh's navigation lines for one level to `lines`, each end point taken from mesh space to world space through the object's position matrix (z = 0)
// Grade: DIFFER(452v438) -- retail holds no C2DLineF temporary (frame 0x20 = iterator, array ref, two 12-byte C3DVector slots: start at +0x14, end at +0x08), the push_back copy reads start/end straight from those slots, and end is copied as dwords then transformed in place (x cached, y re-read); ours materialises a 16-byte line temporary (frame 0x40) and keeps end in st(). 4 of 4 attempts (12d20_a..d in probe/).
// Leaf: ?GetNavigationLineList@CPhysicsMeshObject@@QBEXAAV?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A12D20, 438 bytes, thiscall, `ret 8` (lines, level).
// Identity: Ego_r.pdb `?GetNavigationLineList@CPhysicsMeshObject@@QBEXAAV?$vector@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@J@Z`;
//   Ego_r donor order puts it after the CPhysicsMeshObject constructors (landed 0x00A127D0 / 0x00A12CC0) and
//   HasNavigationLineListInfo (landed 0x00A12C90) and before GetNavigationLevelVolumes (0x00A12EE0); the landed
//   vector<C2DLineF>::_M_insert_overflow 0x00A13800 already identifies this body from its shared __uninitialized_copy
//   0x00A13490. Its caller 0x006A4F60 (CThingPhysical::GetNavCollisionLineList) binds the invented
//   `?BuildCollisionLines@CPhysicsMeshObject@@QAEXPAUSCollisionLineList@@J@Z` (NAME_FIXES.tsv).
// Callees: CPhysicsMesh::GetNavigationLines(long) const 0x00A73D70 (returns const CArray<C2DLineF>&; Ego_r.pdb
//   `?GetNavigationLines@CPhysicsMesh@@QBEABV?$CArray@VC2DLineF@@@@J@Z`), malloc 0x00BFEA0E, STLport
//   `__uninitialized_copy<C2DLineF*,C2DLineF*>` 0x00A13490, free 0x00BFEA14 (push_back with _M_insert_overflow inlined,
//   fill_len 1, atend true).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class C2DLineF {
public:
    C2DVector Point1;
    C2DVector Point2;
    C2DLineF(const C2DVector& p1, const C2DVector& p2) : Point1(p1), Point2(p2) { }
    C2DLineF(const C2DLineF& other) : Point1(other.Point1), Point2(other.Point2) { }
};
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& p) : X(p.X), Y(p.Y), Z(p.Z) {}
};
class CMatrix3x4 {
public:
    C3DVector XAxis, YAxis, ZAxis, Position;
};
template <class T> class CArray {
public:
    T* Data;
    T* DataEnd;
    T* begin() const { return Data; }
    T* end() const { return DataEnd; }
};
class CPhysicsMesh {
public:
    const CArray<C2DLineF>& GetNavigationLines(long level) const;   // 0x00A73D70
};
template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
} // namespace std

class CPhysicsMeshObject {
public:
    void*                                 VTable;           // +0x00
    CIVCountedPointer<const CPhysicsMesh> Mesh;             // +0x04
    CMatrix3x4                            PositionMatrix;   // +0x08
    float                                 Scale;            // +0x38
    float                                 OO_Scale;         // +0x3C
    void GetNavigationLineList(std::vector<C2DLineF>& lines, long level) const;
};

inline C3DVector operator*(const CMatrix3x4& m, const C3DVector& v)
{
    C3DVector result;
    const float x = v.X;
    const float y = v.Y;
    result.X = x * m.XAxis.X + y * m.YAxis.X + v.Z * m.ZAxis.X + m.Position.X;
    result.Y = x * m.XAxis.Y + y * m.YAxis.Y + v.Z * m.ZAxis.Y + m.Position.Y;
    result.Z = x * m.XAxis.Z + y * m.YAxis.Z + v.Z * m.ZAxis.Z + m.Position.Z;
    return result;
}

void CPhysicsMeshObject::GetNavigationLineList(std::vector<C2DLineF>& lines, long level) const
{
    const CArray<C2DLineF>& meshLines = Mesh->GetNavigationLines(level);
    for (const C2DLineF* it = meshLines.begin(); it != meshLines.end(); ++it)
    {
        C3DVector start = PositionMatrix * C3DVector(it->Point1.X, it->Point1.Y, 0.0f);
        C3DVector end = PositionMatrix * C3DVector(it->Point2.X, it->Point2.Y, 0.0f);
        lines.push_back(C2DLineF(C2DVector(start.X, start.Y), C2DVector(end.X, end.Y)));
    }
}
