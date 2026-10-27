// std::__destroy_aux(std::pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>* first, std::pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>* last, const __false_type&) -- destroy every element of [first, last)
// Leaf: ??$__destroy_aux@PAU?$pair@VCMeshInfo@NGraphicAppearance@@VCEngineTextureMorphSet@@@std@@@std@@YIXPAU?$pair@VCMeshInfo@NGraphicAppearance@@VCEngineTextureMorphSet@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BC29E, 29 bytes, `__fastcall` (first ecx, last edx), `ret 4`.
// Identity (proven): 0x18-byte elements whose deleting destructor 0x005BC277 destroys the member at +0xC (0x005BC0D3); its callers are the landed vector<pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> >::_M_insert_overflow 0x00709D60 and __uninitialized_fill_n 0x00709CE0 of the same element.
// Callees: the element's scalar deleting destructor 0x005BC277 (flags 0).
// Shape (family M097, representative 0x004DD79F): STLport `for (; __first != __last; ++__first) _Destroy(&*__first);` in a size
//   TU: `optimize("s")` keeps the element destructor out of line, and VC7.1 calls the class's `??_G` with 0 for `p->~T()`.
#pragma optimize("s", on)
namespace NGraphicAppearance {
class CMeshInfo {                                         // Ego_r.pdb, 12 bytes
public:
    long Index;
    float SizeX;
    unsigned char PrimaryMesh;
};
}
class CEngineTextureMorphSet {                            // Ego_r.pdb: CArray<CEngineTextureMorph> Morphs
public:
    ~CEngineTextureMorphSet();                            // 0x005BC0D3
    unsigned long Members[3];
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
namespace std {
struct __false_type { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
typedef ::std::pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet> _E;
template void __fastcall __destroy_aux<_E*>(_E*, _E*, const __false_type&);
}