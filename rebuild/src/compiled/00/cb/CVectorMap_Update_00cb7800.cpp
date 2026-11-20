// CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >::Update() -- the script table map: when Dirty (+0x0D) is set, Sort expanded in place: std::sort over [begin, end) with Compare (+0x0C), then Dirty cleared
// Leaf: ?Update@?$CVectorMap@VCCharString@@VCScriptInfo@@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB7800, thiscall, plain ret; calls std::sort<pair<CCharString, CScriptInfo>*, CKeyPairCompareLess<..> >
//   0x00CB7780 (__fastcall, the comparator pushed).
// Identity: FableWin.pdb ?Update@?$CVectorMap@VCCharString@@VCScriptInfo@@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@@QAEXXZ
//   (0x03585BC0 Sort / 0x035855B0 Update; Ego_r.pdb holds the same CVectorMap<CCharString, CScriptInfo> instance: ~CVectorMap, LowerBound)
//   and the body: the engine/CVectorMap.h FABLE_RETAIL_CVECTORMAP_SORT Sort / Update text, as the landed pair 0x00A42040 / 0x00A420C0;
//   the element is pair<CCharString, CScriptInfo> by the sort callee 0x00CB7780 (CScriptInfoManager::Scripts).
// Naming (NF180, 2026-11-20, q4221): was the stand-in CTCVillage::OnInitialActivate over a made-up CInner / InnerActivate_00cb7780 model (propagated label); now the member on the shared headers, so the sort call binds the
//   STLport spelling the self row 0x00CB7780 defines.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_SORT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CVectorMap.h"
template void CVectorMap<CCharString, CScriptInfo, CKeyPairCompareLess<CCharString, CScriptInfo> >::Update();
