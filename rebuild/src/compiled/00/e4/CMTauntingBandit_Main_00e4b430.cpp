// NScript::CV_BanditTollScript::CM_TauntingBandit::Main() -- taunting-bandit spawner: count the "TauntingBandit" things; while fewer than the script def's maximum exist, spawn one more here (creature def from this marker's data string, scripted "TauntingBandit") and attach it to the "BanditTollVillage"
// Leaf: ?Main@CM_TauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Identity (naming wave 11): ?Main@CM_TauntingBandit@CV_BanditTollScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012E4DB8 slot 1 is stored by the factory 0x00E48D00 registered as "M_TauntingBandit"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4B430, length 375 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("TauntingBandit" @ 0x012E4C60,
//   "BanditTollVillage" @ 0x012E4FB0), IsDone @ 0x00F35B30, ~CScriptThing @ 0x004AA840, the vector<CScriptThing>
//   destructor @ 0x008AC970 (out of line on the IsDone exit; manifest "~vector<CConversation>" is a folded twin),
//   free @ 0x00BFEA14 (inline destructor copy); CScriptThing slots 0 (virtual destructor), 3 GetDataString, 6 GetPos;
//   CGameScriptInterface (vtable 0x01260F0C) slots 72 GetThingWithScriptName, 75 GetAllThingsWithScriptName, 91
//   CreateCreature, 492 EntityAttachToVillage. The script def @ 0x0143E90C holds the bandit cap at +0x444.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8} as the landed CHTG_AmbushPoint::Main 0x00D73610;
//   STLport vector<CScriptThing> {start, finish, end_of_storage} with 12-byte elements.
// Shape: the local vector's destructor is expanded inline at the function end and called out of line at the early
//   IsDone exit; its element walk is a by-value `_Destroy(first, last)` over the virtual destructor.
namespace NScript { class CPredicateBase; }
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
#include "engine/C3DVector.h"
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// agentNF60: std::vector is the shared STLport 4.6 vector (stlport/fable_containers.h; was a local namespace-std stand-in).
#include "stlport/fable_containers.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: MaxTauntingBandits -> BanditCountForToll (+0x444).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_BanditTollScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CM_TauntingBandit;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_BanditTollScript;
namespace NScript {
class CV_BanditTollScript::CM_TauntingBandit : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_BanditTollScript::CM_TauntingBandit::Main()
{
    std::vector<CScriptThing> bandits;
    Game->GetAllThingsWithScriptName("TauntingBandit", bandits);
    if (bandits.size() < pGlobalScriptDef->BanditCountForToll) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing bandit = Game->CreateCreature(Me.GetDataString(), Me.GetPos(), "TauntingBandit", false);
        Game->EntityAttachToVillage(bandit, Game->GetThingWithScriptName("BanditTollVillage"));
    }
}