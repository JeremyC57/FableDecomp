// std::swap(std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >& a, std::pair<...>& b) -- exchange two (id, active entity script) pairs through a temporary copy
// Leaf: ??$swap@U?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@@std@@YIXAAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@0@0@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA7E0, 88 bytes, `__fastcall` (a in ecx, b in edx), `ret`.
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx), built /Gr like the landed sibling
//   std::swap<pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > > 0x00CBA8E0.
// Identity: STLport `swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }` over the
//   CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, CKeyPairCompareLess<...> > element (16 bytes:
//   the 8-byte key, the counted pointer at +8). The pair assignment it calls twice, 0x00CBA520, is bound as
//   ??4?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@QAEAAU01@ABU01@@Z by the landed
//   __unguarded_linear_insert 0x00CBA840 of the same sort; its only reference is the iter_swap tail `jmp` at
//   0x00CBB260. Label correction: the manifest row "C3DAnimFile::C3DAnimFile" is propagated.
// Callees: the implicit pair operator= 0x00CBA520 (emitted out of line, as in the sibling), operator delete 0x00BFE9BC
//   (the temporary's counted-pointer release).
// Shape: the sibling 0x00CBA8E0 model: the pair copy is the implicit memberwise one (the key's two words, then the
//   counted pointer's inline copy with its count bump), the temporary's release inline at the end.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CActiveEntityScriptBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;   // 16 bytes
namespace std {
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<CActiveScriptPair>(CActiveScriptPair&, CActiveScriptPair&);
}