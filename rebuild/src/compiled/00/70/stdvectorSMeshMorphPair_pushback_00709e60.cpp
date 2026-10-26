// std::vector<SMeshMorphPair>::push_back(const value_type& x) -- STLport append of a (mesh info, texture morph set) pair: copy-construct in place when there is room, else grow through _M_insert_overflow
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00709E60, length 89 bytes. __fastcall (this in ecx), `ret 4`.
// Callees: CArray<CEngineTextureMorph>::CArray(const CArray&) @ 0x005BBDF1 (the morph set's vector copy),
//          vector<pair<CMeshInfo, CEngineTextureMorphSet>>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00709D60.
// Identity: the manifest label "std_vector_Cons_val_CMusicSetEntry" is propagated. 0x00709D60 is the insert-overflow of
//   the vector CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets fills (its inline push_back at 0x00707C49 is
//   this very body); this is the out-of-line COMDAT instance of the same push_back. Element: pair<NGraphicAppearance::
//   CMeshInfo, CEngineTextureMorphSet> (24 bytes retail, see the __uninitialized_copy sibling 0x00709C90).
// Shape: STLport 4.x `push_back(const _Tp& x)`: `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, x);
//   ++_M_finish; } else _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- the pair's explicit memberwise copy
//   constructor (CMeshInfo as three dwords, the morph array through 0x005BBDF1), `add [esi+4],18h`, and the empty
//   __false_type temporary homed in the dead `x` argument slot for the overflow call.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

namespace NGraphicAppearance {
class CMeshInfo { public:                              // Ego_r.pdb, 12 bytes
    long  Index;                                // +0x00
    float SizeX;                                // +0x04
    unsigned char PrimaryMesh : 1;              // +0x08
};
}
class CEngineTextureMorph;
struct CEngineTextureMorphArray {               // CArray<CEngineTextureMorph> (retail STLport vector, 12 bytes)
    CEngineTextureMorph* _M_start;
    CEngineTextureMorph* _M_finish;
    CEngineTextureMorph* _M_end_of_storage;
    CEngineTextureMorphArray(const CEngineTextureMorphArray& other);   // 0x005BBDF1
};
class CEngineTextureMorphSet { public:                 // Ego_r.pdb: Morphs +0
    CEngineTextureMorphArray Morphs;
};

template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const pair& __o) : first(__o.first), second(__o.second) {}
};
typedef pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> SMeshMorphPair;   // 24 bytes

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct SAllocProxy {      // STLport _STLP_alloc_proxy: the end-of-storage pointer
    _Tp* _M_data;
};

template <class _Tp> struct CRetailVector {    // STLport vector (12 bytes)
    _Tp* _M_start;
    _Tp* _M_finish;
    SAllocProxy<_Tp> _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00709D60
    void push_back(const _Tp& __x);
};

template <class _Tp> void CRetailVector<_Tp>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}

template struct CRetailVector<SMeshMorphPair>;