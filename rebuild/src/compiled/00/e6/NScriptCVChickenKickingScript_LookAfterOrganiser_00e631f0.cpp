// CV_ChickenKickingScript::LookAfterOrganiser() -- each time the hero leaves Oak Bay and comes back, re-create the chicken master if he ran off: at his original marker once the hero knows the ghost has gone, else at the scared-master marker
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E631F0, length 701 bytes. Identity: spawned by CV_ChickenKickingScript::Main
//   0x00E62A00 as "LookAfterOrganiser" 0x012E7378 (CSpawnedFunc vtable 0x012E732C at 0x00E62CFB); the strings name the chicken-kicking quest (manifest
//   "CQ_GuildTrainingScript::KeepTabsOnWhisper" is propagated).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CScriptThing::GetPos (virtual slot 6, +0x18); through the
//   interface (+0x40): NewScriptFrame (7), IsRegionLoaded (12), GetThingWithScriptName (72), CreateCreature (91).
// Strings: "OakBay" 0x012D31E4, "ChickenMaster" 0x012E73D0, "MK_CK_ORG" 0x012E7470, "ScaredMasterMarker" 0x012E745C,
//   "CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED" 0x012E7424.
// Layout (retail = Ego_r.pdb CV_ChickenKickingScript - 0x14): KnowGhostHasGone +0x54, RanOff +0x56.
// Shape: the "while loaded" wait is written `!= false` (test/setne, see RunThemeOverriding 0x00D35ED0); the marker
//   handle and the created creature are by-value CScriptThing temporaries destroyed out of line.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CV_ChickenKickingScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool ChickenLanded;                                 // +0x48
    long FinalSector;                                   // +0x4c
    long DistanceBand;                                  // +0x50
    bool KnowGhostHasGone;                              // +0x54
    bool SpectatorsCreated;                             // +0x55
    bool RanOff;                                        // +0x56
    void LookAfterOrganiser();
};
} // namespace NScript
using NScript::CV_ChickenKickingScript;

void NScript::CV_ChickenKickingScript::LookAfterOrganiser()
{
    if (IsActiveThreadTerminating())
        return;
    do {
        while (Game->IsRegionLoaded("OakBay") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsRegionLoaded("OakBay")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (RanOff) {
            if (IsActiveThreadTerminating())
                return;
            if (KnowGhostHasGone) {
                if (IsActiveThreadTerminating())
                    return;
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("MK_CK_ORG").GetPos(), "ChickenMaster", false);
            } else {
                if (IsActiveThreadTerminating())
                    return;
                Game->CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", Game->GetThingWithScriptName("ScaredMasterMarker").GetPos(), "ChickenMaster", false);
            }
            RanOff = false;
        }
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}