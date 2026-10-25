// std::__uninitialized_copy(const COptimisedSubMesh* first, const COptimisedSubMesh* last, COptimisedSubMesh* result, const __false_type&) -- copy-construct a run of optimiser sub-meshes (their primitive pointer and bone vectors) into raw storage
// Flags: /Gr
// Leaf: ??$__uninitialized_copy@PBVCOptimisedSubMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD75F0, 0xEB bytes (+ int3 padding), __fastcall (first ecx, last edx),
// `ret 8` (result, the __false_type reference), returns the end of the constructed run.
// Identity: the 0x18-byte element holds two STLport vectors -- an 8-byte counted pointer vector at +0 (copy bumps the
// count block) and a 16-byte element vector at +0xC copied by 0x00AD1B20 -- i.e. Ego_r.pdb
// QuadricOptimiserInternals::COptimisedSubMesh { CArray<CCountedPointer<COptimisedPrimitive>> Primitives;
// CArray<COptimisedBone> Bones; } with retail 12-byte vectors. Callers: vector<COptimisedSubMesh>::_M_insert_overflow
// 0x00AD83D0 (twice, the prefix and the remainder). Catalog "Vector_CopyConstructor_6Way" is descriptive.
// Callees: malloc @ 0x00BFEA0E (both vectors' allocate(n)), __uninitialized_copy<const COptimisedBone*, COptimisedBone*>
// @ 0x00AD1B20 (manifest "_Uninit_copy<CFeatDef*,...>" is a propagation; its tag reference is the dead `result` slot).
// Shape: STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);` with the placement-new null test;
// COptimisedSubMesh's implicit copy constructor runs both STLport vector copy constructors inline
// (`_Vector_base(x.size(), a)` zeroes the three words, then allocate (null when empty), then __uninitialized_copy:
// the counted-pointer one inline with its own placement-new test and `if (Info) ++Info->RefCount`, the bone one a
// call). `last` lives in the `push ecx` frame word across the call.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
};

struct CCPPointerInfo {
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
__uninitialized_copy<const QuadricOptimiserInternals::COptimisedSubMesh*, QuadricOptimiserInternals::COptimisedSubMesh*>(
    const QuadricOptimiserInternals::COptimisedSubMesh*, const QuadricOptimiserInternals::COptimisedSubMesh*,
    QuadricOptimiserInternals::COptimisedSubMesh*, const __false_type&);
} // namespace std