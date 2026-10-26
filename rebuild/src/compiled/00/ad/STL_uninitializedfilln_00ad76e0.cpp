// std::__uninitialized_fill_n(COptimisedSubMesh* first, unsigned int n, const COptimisedSubMesh& x, const __false_type&) -- copy-construct n copies of one optimiser sub-mesh into raw storage
// Flags: /Gr
// Leaf: ??$__uninitialized_fill_n@PAVCOptimisedSubMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD76E0, 0xEB bytes, __fastcall (first ecx, n edx), `ret 8` (x, the
// __false_type reference), returns the end of the constructed run.
// Identity: same element as the sibling __uninitialized_copy 0x00AD75F0 -- Ego_r.pdb QuadricOptimiserInternals::
// COptimisedSubMesh { Primitives (vector<CCountedPointer<COptimisedPrimitive>>) +0; Bones (vector<COptimisedBone>)
// +0xC } -- called from vector<COptimisedSubMesh>::_M_insert_overflow 0x00AD83D0 when fill_len != 1.
// Catalog "Vector_ReallocateElements_6Way" is descriptive.
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy<const COptimisedBone*, COptimisedBone*> @ 0x00AD1B20.
// Shape: STLport `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` with an unsigned count (`jbe`, the
// count homed in the `push ecx` frame word, `dec ecx` at the bottom) and one zero register for the placement-new
// test; the element copy constructor is COptimisedSubMesh's implicit one (both vector copy constructors inline).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

class CCPPointerInfo { public:
    unsigned long RefCount;                                  // +0x00
    void (__fastcall* DeleteFunc)(void* data);               // +0x04
    void*         Data;                                      // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    CCountedPointer(const CCountedPointer& x) : Data(x.Data), Info(x.Info) { if (Info) ++Info->RefCount; }
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class T> class allocator {                        // STLport allocator<T>: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    _Vector_base(unsigned int n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    }
};

} // namespace std

namespace QuadricOptimiserInternals {

class COptimisedPrimitive;

class COptimisedBone {                                      // 16 bytes
public:
    CCharString BoneName;                                   // +0x0
    long        Parent;                                     // +0x4
    bool        Remapped;                                   // +0x8
    long        NewIndex;                                   // +0xC
};

class COptimisedSubMesh {                                   // 0x18 bytes in retail
public:
    std::vector<CCountedPointer<COptimisedPrimitive> > Primitives;   // +0x00
    std::vector<COptimisedBone>                        Bones;        // +0x0C
};

} // namespace QuadricOptimiserInternals

namespace std {
template <> QuadricOptimiserInternals::COptimisedBone*
__uninitialized_copy<const QuadricOptimiserInternals::COptimisedBone*, QuadricOptimiserInternals::COptimisedBone*>(
    const QuadricOptimiserInternals::COptimisedBone*, const QuadricOptimiserInternals::COptimisedBone*,
    QuadricOptimiserInternals::COptimisedBone*, const __false_type&);                   // 0x00AD1B20

template QuadricOptimiserInternals::COptimisedSubMesh*
__uninitialized_fill_n<QuadricOptimiserInternals::COptimisedSubMesh*, unsigned int, QuadricOptimiserInternals::COptimisedSubMesh>(
    QuadricOptimiserInternals::COptimisedSubMesh*, unsigned int, const QuadricOptimiserInternals::COptimisedSubMesh&,
    const __false_type&);
} // namespace std