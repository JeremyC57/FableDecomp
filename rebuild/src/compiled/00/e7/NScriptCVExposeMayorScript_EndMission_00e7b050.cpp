// CV_ExposeMayorScript::EndMission() -- finish the Expose Mayor quest down the path the hero took: the exposure path (optionally becoming mayor) completes the quest, the bribe path waits for the bribe flag; then retire the wise villager's information and the quest
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7B050, 0x23b bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   PlayCutscene(CCharString, bool, CCharString) 0x00E78100 (by-value strings built in their argument slots, right to
//   left), ~CScriptThing 0x004AA840; CGameScriptInterface slots 7 NewScriptFrame, 72 GetThingWithScriptName, 281
//   DeactivateQuestLater, 288 SetQuestAsCompleted, 361 ClearThingHasInformation, 374 FadeScreenIn, 447 SetThingAsUsable,
//   655 GetActiveQuestName, 720 SetCategoryActivity.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, the shared master data +0x44 (its +0xd7 is an unnamed bool set when the
//   mayor has been bribed), Path +0x48 (PATH_ID; 1 = expose), SaidYesToBecomeMayor +0x53.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/C3DVector.h"
extern C3DVector g_ZeroVector;                           // 0x0143E8E0
struct CScriptThingImp {                                 // the game-side thing behind a CScriptThing
    virtual void i0(); virtual void i1(); virtual void i2(); virtual void i3(); virtual void i4(); virtual void i5();
    virtual const C3DVector& GetPosition() const;        // slot 6 (+0x18)
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CMasterScriptData {
    unsigned char _pad00[0xd7];
    bool MayorBribed;                                    // +0xd7 (unnamed)
};
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    CMasterScriptData* PMasterData;                      // +0x44
    long Path;                                           // +0x48 PATH_ID
    long StageNumber;                                    // +0x4c
    bool HeroFlashedLightsCorrectly;                     // +0x50
    bool PrisonerTalkedAboutLamp;                        // +0x51
    bool ResetLamp;                                      // +0x52
    bool SaidYesToBecomeMayor;                           // +0x53
    void PlayCutscene(CCharString name, bool fade, CCharString region);   // 0x00E78100
    void EndMission();
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;

void NScript::CV_ExposeMayorScript::EndMission()
{
    CScriptThing door = Game->GetThingWithScriptName("GreyHouseFrontDoor");
    if (Path == 1) {
        if (IsActiveThreadTerminating())
            return;
        if (SaidYesToBecomeMayor) {
            if (IsActiveThreadTerminating())
                return;
            PlayCutscene("CS_MAYOREXPOSE_BECOMEMAYOR", true, "NULL");
            Game->SetCategoryActivity("ExposedMayor", true);
            Game->SetThingAsUsable(door, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->SetCategoryActivity("ExposedMayorNotBecomeMayor", true);
        }
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        while (!PMasterData->MayorBribed) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->SetCategoryActivity("BribedMayor", true);
    }
    CScriptThing wiseVillager = Game->GetThingWithScriptName("WiseVillager");
    Game->ClearThingHasInformation(wiseVillager);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    if (Path == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->FadeScreenIn();
    }
}