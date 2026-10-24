// std::__uninitialized_copy<SMeshMorphPair*,SMeshMorphPair*>(first, last, result, const __false_type&) -- placement-copy [first, last) of (mesh info, texture morph set) pairs into raw storage, return the end of the built range
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00709C90, length 70 bytes (+ int3 padding). `__fastcall` (/Gr algorithm TU):
//         first in ecx, last in edx, result + the tag reference on the stack, `ret 8`.
// Callees: CArray<CEngineTextureMorph>::CArray(const CArray&) @ 0x005BBDF1 (the 20-byte-element vector copy, out of line;
//          its manifest label "CExpressionSet::Add" is propagated).
// Identity: the manifest label `_Move<CMusicSetEntry*,CMusicSetEntry*>` is propagated across a byte-identical body. Both
//   callers sit in vector<pair<CMeshInfo, CEngineTextureMorphSet>>::_M_insert_overflow (0x00709D60), which
//   CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets (Ego_r.pdb `?GetMeshesAndMorphSets@...@@QBEXAAV?$vector
//   @U?$pair@VCMeshInfo@NGraphicAppearance@@VCEngineTextureMorphSet@@@std@@...`) reaches from its inline push_back
//   (call site 0x00707C83). Element: NGraphicAppearance::CMeshInfo (12 bytes: Index, SizeX, PrimaryMesh:1) followed by
//   CEngineTextureMorphSet (Morphs, a CArray<CEngineTextureMorph> = retail 12-byte STLport vector of 20-byte morphs).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, __false_type)` (`for (; first != last; ++first, ++cur)
//   _Construct(&*cur, *first)`, placement new keeps its null test, return cur); STLport's pair has an explicit
//   memberwise copy constructor: CMeshInfo copies as three dwords, the morph set through its member's copy constructor.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

namespace NGraphicAppearance {
struct CMeshInfo {                              // Ego_r.pdb, 12 bytes
    long  Index;                                // +0x00
    float SizeX;                                // +0x04
    unsigned char PrimaryMesh : 1;              // +0x08
};
}
struct CEngineTextureMorph;
struct CEngineTextureMorphArray {               // CArray<CEngineTextureMorph> (retail STLport vector, 12 bytes)
    CEngineTextureMorph* _M_start;
    CEngineTextureMorph* _M_finish;
    CEngineTextureMorph* _M_end_of_storage;
    CEngineTextureMorphArray(const CEngineTextureMorphArray& other);   // 0x005BBDF1
};
struct CEngineTextureMorphSet {                 // Ego_r.pdb: Morphs +0
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

namespace std {
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template SMeshMorphPair* __fastcall __uninitialized_copy(SMeshMorphPair*, SMeshMorphPair*, SMeshMorphPair*, const __false_type&);
}