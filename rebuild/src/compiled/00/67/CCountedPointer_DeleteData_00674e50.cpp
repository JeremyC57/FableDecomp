// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CWoundMorphs>) the counted pointer's per-type deleter: destroy the wound morph table (free its vector storage) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCWoundMorphs@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00674E50, 33 bytes (+ int3 padding), `ret` (static `__fastcall`, data in ecx).
// Identity (proven): CCountedPointer<CWoundMorphs>::DeleteData(void*); evidence: its only reference is the count-block store in CCountedPointer<CWoundMorphs>::Reset 0x00675060 (0x006750B0), called by CCreatureDef::CCreatureDef 0x006765A0 at 0x00676687 on this+0x28 = Ego_r.pdb CCreatureDef::PWoundMorphs (CCountedPointer<CWoundMorphs>) with a fresh 0x10-byte object (zeroed vector, Dirty +0x0D cleared = CWoundMorphs::Morphs, a CVectorMap); Ego_r.pdb names ?DeleteData@?$CCountedPointer@VCWoundMorphs@@@@SIXPAX@Z.
// Label: the manifest's "Object_FreeInnerAndDelete_00674e50" is behavioural (family J103: five byte-identical deleters; retail has no identical-code
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
enum EBodyLocation { };                                    // Ego_r.pdb (enumerators not needed)
template <class K, class V> class CKeyPairCompareLess { };   // empty comparator
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {   // retail 0x10 bytes
public:
    C    Compare;                                          // +0x0C
    bool Dirty;                                            // +0x0D
};
class CWoundMorphs {                                       // retail 0x10 bytes (Ego_r.pdb 0x14 with a Dinkumware vector)
public:
    class CEntry {                                         // 40 bytes (Ego_r.pdb), trivially destructible
    public:
        EBodyLocation Location;                            // +0x00
        long ReplacingTextureIndex;                        // +0x04
        long ReplacingBumpMapIndex;                        // +0x08
        long WoundTextureIndex;                            // +0x0C
        long WoundBumpMapIndex;                            // +0x10
        long ScarTextureIndex;                             // +0x14
        long ScarBumpMapIndex;                             // +0x18
        long FadeToScarDuration;                           // +0x1C
        long TotalDuration;                                // +0x20
        float PermanentScarStrength;                       // +0x24
    };
    CVectorMap<EBodyLocation, CEntry, CKeyPairCompareLess<EBodyLocation, CEntry> > Morphs;   // +0x00
};

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CWoundMorphs>;