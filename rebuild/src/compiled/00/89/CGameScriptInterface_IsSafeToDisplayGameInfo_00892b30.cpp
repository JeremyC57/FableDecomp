// Leaf: ?IsSafeToDisplayGameInfo@CGameScriptInterface@@UBE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_IsSafeToDisplayGameInfo@@YI_NPAX@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
// Naming wave 18: the global stand-in g_obj (0x013B8790) is the private static CCountedPointer<CPlayerGui>
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
#define FABLE_RETAIL_NSCRIPT
struct CSub { bool Query(bool flag); };
#include "engine/CCountedPointer.h"
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public: char pad[0x18]; CSub sub; };

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x00490927 ?IsAllowedToSetText@CSubtitleRenderer@@QBE_N_N@Z (was ?Query@CSub@@QAE_N_N@Z)
class CSubtitleRenderer { public: bool IsAllowedToSetText(bool) const; };
bool CGameScriptInterface::IsSafeToDisplayGameInfo() const
{
    return reinterpret_cast<const CSubtitleRenderer*>(&(CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().sub))->IsAllowedToSetText(false);
}