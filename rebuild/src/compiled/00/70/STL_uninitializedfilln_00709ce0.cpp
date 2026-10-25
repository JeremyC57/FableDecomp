// std::__uninitialized_fill_n(pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>* first, unsigned int n, const pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>& x, const __false_type&) -- copy-construct n (mesh info, texture morph set) pairs into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00709CE0, 66 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: one, 0x00709E03 -- the vector's _M_insert_overflow 0x00709D60 (the vector CTCHeroAttachableAppearanceModifiers::
//   GetMeshesAndMorphSets fills; its landed push_back 0x00709E60 is the same TU).
// Element: 24 bytes -- NGraphicAppearance::CMeshInfo (Ego_r.pdb: long Index, float SizeX, the PrimaryMesh bitfield; three
//   dwords) then CEngineTextureMorphSet (its CArray<CEngineTextureMorph>, a 12-byte STLport vector copied by the
//   out-of-line copy constructor 0x005BBDF1). STLport's memberwise pair copy constructor, as the landed push_back.
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated across the byte-identical-shaped STLport helpers.
// Shape: STLport 4.6 __uninitialized_fill_n verbatim (`for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);`, unsigned
//   n so `jbe`), the placement new keeping its null test (`test esi,esi ; je`).
inline void* operator new(unsigned int, void* p) { return p; }

namespace NGraphicAppearance {
class CMeshInfo {                               // Ego_r.pdb, 12 bytes
public:
    long          Index;                        // +0x00
    float         SizeX;                        // +0x04
    unsigned char PrimaryMesh : 1;              // +0x08
};
}

class CEngineTextureMorph;
class CEngineTextureMorphArray {                // CArray<CEngineTextureMorph> (retail STLport vector, 12 bytes)
public:
    CEngineTextureMorph* _M_start;              // +0x00
    CEngineTextureMorph* _M_finish;             // +0x04
    CEngineTextureMorph* _M_end_of_storage;     // +0x08
    CEngineTextureMorphArray(const CEngineTextureMorphArray& other);   // 0x005BBDF1
};

class CEngineTextureMorphSet {                  // Ego_r.pdb: Morphs +0
public:
    CEngineTextureMorphArray Morphs;            // +0x00
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> SMeshMorphPair;     // 24 bytes

template SMeshMorphPair* __fastcall __uninitialized_fill_n(SMeshMorphPair*, unsigned int, const SMeshMorphPair&,
                                                           const __false_type&);
}