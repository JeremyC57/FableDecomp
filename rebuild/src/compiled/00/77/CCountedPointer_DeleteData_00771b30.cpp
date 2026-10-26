// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<COpinionSet>) the counted pointer's per-type deleter: destroy the opinion set (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCOpinionSet@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771B30, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<COpinionSet>::DeleteData(void*); evidence: its only reference is the count-block store in CCountedPointer<COpinionSet>::Reset 0x00771B60 (0x00771BB0), called by CTCOpinionSource::OnCreate 0x00771660 on this+0x78 = Ego_r.pdb CTCOpinionSource::POpinionProduced (CCountedPointer<COpinionSet>, Ego_r +0x7C) with a fresh object whose vector holds five zeroed floats = COpinionSet::OpinionValues (CArray<float>); Ego_r.pdb names ?DeleteData@?$CCountedPointer@VCOpinionSet@@@@SIXPAX@Z.
// Label: the manifest's "ObjHolder_FreeAndDelete_00771b30" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
//   folding, so each address is its own CCountedPointer<T>).
// Callees: free 0x00BFEA14 (the STLport malloc allocator's deallocate of the vector block), operator delete 0x00BFE9BC.
// Shape: the landed CCountedPointer<CSpeechFrame>::DeleteData 0x00A4A910: `delete (T*)data` -- the null test, the inline
//   (forced) destructor chain of a POD-element vector (`_Destroy` of trivially destructible elements is empty; `_Vector_base`'s
//   destructor frees a non-null start), then operator delete.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

namespace std {
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    __forceinline ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __forceinline ~vector() { }                            // _Destroy over trivially destructible elements: nothing
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class T> class CArray : public std::vector<T> { };
class COpinionSet {                                        // retail 0x0C bytes (Ego_r.pdb 0x10 with a Dinkumware vector)
public:
    CArray<float> OpinionValues;                           // +0x00
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<COpinionSet>;