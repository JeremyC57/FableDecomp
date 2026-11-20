// CGameScriptInterface::ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& other, bool isAlly) const -- add the other thing to (or remove it from) the ally list of the entity's kind-0x49 component when both script things are valid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089C100, length 131 bytes.
// Callees: the script things' slots 75 (+0x12c) and 11 (+0x2c), the component-map LowerBound @ 0x0040F020,
//          CTCEnemy::AddAllyThing @ 0x0076D270 and CTCEnemy::RemoveAllyThing @ 0x0076C850.
// Shape: the other thing is fetched before the flag test; the `push eax ; mov ecx,esi` is shared by both arms.
// Leaf: ?ImpSetEntityAsAllyOfThing@CGameScriptInterface@@ABEXABVCScriptThing@@0_N@Z
// Naming NF194 (2026-11-20, q11668): Ego_r.pdb 0x008DA6D0 + FableWin.pdb 0x02A8ED70 make it a PRIVATE const member (ABE); the row-local
//   declaration moves under private: in place (was public, QBE); its one caller 0x008A7EC0 follows in the same hand-back.
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Name fix (NF124, 2026-11-20, q6326): the CThing_Retail / CTCAllies_Retail / component-map stand-ins replaced by the shared
//   engine/CThing.h (GetTC, FABLE_RETAIL_CTHING_GETTC_EXPANDED) and engine/CTCEnemy.h (opt-ins FABLE_RETAIL_CTCENEMY_ADDALLYTHING /
//   _REMOVEALLYTHING). Kind 0x49 is TCI_ENEMY; the callees are Ego_r.pdb 0x0077B360 / 0x0077B3E0 + FableWin.pdb 0x02606170 / 0x02606200
//   ?AddAllyThing@CTCEnemy@@QAEXABVCThing@@@Z / ?RemoveAllyThing@CTCEnemy@@QAEXABVCThing@@@Z (were ?AddAlly / ?RemoveAlly@CTCAllies_Retail).
//   The row's own CGameScriptInterface model is kept as it was (its own leaf is unchanged here).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#define FABLE_RETAIL_CTCENEMY_ADDALLYTHING
#define FABLE_RETAIL_CTCENEMY_REMOVEALLYTHING
#include "engine/CTCEnemy.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CWorld; class CMainGameComponent; class CDisplayEngine; class CThingSearchTools;
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
private:
    void ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& other, bool isAlly) const;   // private (Ego_r / FableWin ABE)
};

void CGameScriptInterface::ImpSetEntityAsAllyOfThing(const CScriptThing& entity, const CScriptThing& other, bool isAlly) const
{
    if (entity.IsAlive() && other.IsAlive()) {
        CThing* t = entity.GetPThing();
        CTCEnemy* enemy;
        if (t->GetTC(TCI_ENEMY, &enemy)) {
            const CThing* o = other.GetPThing();
            if (isAlly)
                enemy->AddAllyThing(*o);
            else
                enemy->RemoveAllyThing(*o);
        }
    }
}
