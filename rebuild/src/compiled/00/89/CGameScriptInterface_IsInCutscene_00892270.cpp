// Leaf: ?IsInCutscene@CGameScriptInterface@@UBE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h under its class-model switch FABLE_RETAIL_NSCRIPT (was the flattened
//   free stand-in ?CGameScriptInterface_IsInCutscene@@YIDPBUCGameScriptInterface@@@Z); the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
// Naming wave 18: the global stand-in g_cutsceneMgr (0x013B8790) is the private static CCountedPointer<CPlayerGui>
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"   // switch route: the real class model
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public: char pad[0xf6]; bool inCutscene; };   // bool: the PDB member returns bool
bool CGameScriptInterface::IsInCutscene() const
{
    return CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().inCutscene;
}