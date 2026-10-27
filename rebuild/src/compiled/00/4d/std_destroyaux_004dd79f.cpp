// std::__destroy_aux(std::pair<long, CIntelligentPointer<CThing> >* first, std::pair<long, CIntelligentPointer<CThing> >* last, const __false_type&) -- destroy every element of [first, last)
// Leaf: ??$__destroy_aux@PAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@std@@@std@@YIXPAU?$pair@JV?$CIntelligentPointer@VCThing@@@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD79F, 29 bytes, `__fastcall` (first ecx, last edx), `ret 4`.
// Identity (probable): 12-byte elements whose deleting destructor 0x004DA6A9 only runs ~CBaseIntelligentPointer 0x00A01C10 on +4 (a {long, CIntelligentPointer<T>} record); Ego_r.pdb's 12-byte vector element of that shape is pair<long, CIntelligentPointer<CThing> > (vector<pair<long, CIntelligentPointer<CThing> > >, a local in the dump); its callers are that vector's _Destroy / ~vector wrappers 0x004DFBFC, 0x004E1C2B, 0x004E34EC.
// Callees: the element's scalar deleting destructor 0x004DA6A9 (flags 0).
// Shape (family M097, representative 0x004DD79F): STLport `for (; __first != __last; ++__first) _Destroy(&*__first);` in a size
//   TU: `optimize("s")` keeps the element destructor out of line, and VC7.1 calls the class's `??_G` with 0 for `p->~T()`.
#pragma optimize("s", on)
class CThing;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
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
typedef ::std::pair<long, CIntelligentPointer<CThing> > _E;
template void __fastcall __destroy_aux<_E*>(_E*, _E*, const __false_type&);
}