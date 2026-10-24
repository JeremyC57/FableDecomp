// _STL::__destroy_aux(COptimisedSubMesh* first, COptimisedSubMesh* last, const __false_type&) -- destroy a run of optimiser sub-meshes: each one's bone vector (bone names, storage) then its primitive-pointer vector
// Flags: /Gr
// Leaf: ??$__destroy_aux@PAVCOptimisedSubMesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD7DC0, 0x6B bytes, __fastcall (first ecx, last edx), `ret 4` (the
// __false_type reference).
// Identity: the 0x18-byte element is Ego_r.pdb QuadricOptimiserInternals::COptimisedSubMesh (Primitives
// vector<CCountedPointer<COptimisedPrimitive>> +0, Bones vector<COptimisedBone> +0xC, 16-byte bones whose first member
// is a CCharString), the type of the sibling __uninitialized_copy 0x00AD75F0; the caller is
// vector<COptimisedSubMesh>::_M_insert_overflow 0x00AD83D0 (`_M_clear()` -> `_Destroy(_M_start, _M_finish)`, its tag
// passed by address). The catalog's "_Copy_impl<CNavigationPosition*,...>" is propagated.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (per bone), free @ 0x00BFEA14 (each vector's storage),
// __destroy_aux<CCountedPointer<COptimisedPrimitive>*> @ 0x00AD37E0 (landed as DestroyRange_CountedPointer; its tag
// is the high byte of the `push ecx` frame word, `lea eax,[esp+13h]`).
// Shape: STLport `for (; first != last; ++first) _Destroy(&*first);` -- ~COptimisedSubMesh destroys Bones first
// (inline element loop over the bone names, then `deallocate` = `if (p) free(p)`), then Primitives (the counted-pointer
// range destroy out of line, then free).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    ~CCountedPointer();
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
};

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class T> class vector : public _Vector_base<T> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

} // namespace _STL

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
    _STL::vector<CCountedPointer<COptimisedPrimitive> > Primitives;   // +0x00
    _STL::vector<COptimisedBone>                        Bones;        // +0x0C
};

} // namespace QuadricOptimiserInternals

namespace _STL {
template <> void __destroy_aux<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>*>(
    CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>*, CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>*,
    const __false_type&);                                                               // 0x00AD37E0

template void __destroy_aux<QuadricOptimiserInternals::COptimisedSubMesh*>(
    QuadricOptimiserInternals::COptimisedSubMesh*, QuadricOptimiserInternals::COptimisedSubMesh*, const __false_type&);
} // namespace _STL