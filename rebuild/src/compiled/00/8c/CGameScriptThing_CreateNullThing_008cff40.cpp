// CGameScriptThing::CreateNullThing() -- the null script thing: a copy of the script-thing manager's null handle, read through the quest manager singleton's PGameScriptThingManager
// Leaf: ?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CFF40, 21 bytes: a static `__fastcall` returning CScriptThing by value
//   (hidden result pointer in ecx, plain `ret`, returns the result pointer).
// Identity: Ego_r.pdb name set ?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ is the only static __fastcall
//   CScriptThing() producer in the PDB name sets; the body forwards the caller's hidden result to
//   CGameScriptThingManager::GetNullScriptThing 0x00704730 (landed, PDB ?GetNullScriptThing@CGameScriptThingManager@@QAE?AVCScriptThing@@XZ)
//   on [[0x013B89FC]+4] = CQuestManager::PGameScriptThingManager (Ego_r CQuestManager +0x04, CCountedPointer::Data).
//   Its callers are the CGameScriptInterface script-thing getters (GetHero 0x00891CA0, EntityGetStandingOnThing 0x00892220,
//   GetThingWithScriptName 0x008930F0, GetHeroTargetedThing 0x0089A490, EntityGetStandingInsideBuilding 0x0089BC50,
//   SetResidency 0x008A1CC0, CScriptGameResourceObjectScriptedThing::GetScriptThing 0x009038D0), each on its own
//   "return the null thing" path. It is not CScriptThing::CScriptThing() (that is 0x006E7B40: base ctor 0x0099A2D0,
//   vtable 0x01238C8C, PImp nulled) and not the operator&& the propagated manifest label claimed.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

class CGameScriptThingManager {
public:
    CScriptThing GetNullScriptThing();                  // 0x00704730
};


class CQuestManager {                                   // Ego_r: vptr, PGameScriptThingManager +0x04, ...
public:
    virtual ~CQuestManager();
    CCountedPointer<CGameScriptThingManager> PGameScriptThingManager;   // +0x04
};

extern CQuestManager* g_pQuestManager;                  // 0x013B89FC (the CTBaseSingletonNoDefaultAlloc<CQuestManager> instance)

class CGameScriptThing : public CScriptThing {
public:
    static CScriptThing __fastcall CreateNullThing();
};

CScriptThing __fastcall CGameScriptThing::CreateNullThing()
{
    return g_pQuestManager->PGameScriptThingManager.Data->GetNullScriptThing();
}
