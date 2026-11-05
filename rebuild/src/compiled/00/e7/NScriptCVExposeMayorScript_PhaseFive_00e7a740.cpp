// CV_ExposeMayorScript::PhaseFive() -- final phase: on the exposure path retire the side quests, point the quest card at Bowerstone Posh and remove Lady Grey once it loads; on the bribe path drop the quest card; otherwise complain
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7A740, 0x2a9 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840; CGameScriptInterface slots 7 NewScriptFrame, 11 IsLevelLoaded, 72 GetThingWithScriptName,
//   108 RemoveThing, 112 AddScreenMessage, 280 DeactivateQuest, 281 DeactivateQuestLater, 294 RemoveQuestCardFromHero,
//   296 SetQuestCardObjective, 655 GetActiveQuestName.
// Shape: an if / else-if / else chain on Path, each arm opening with the translator's terminate check; the objective
//   call builds its three literal temporaries right to left before GetActiveQuestName.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, the shared master data +0x44 (its +0xd5 / +0xd6 are unnamed
//   path-taken flags, next to +0xd7 read by EndMission 0x00E7B050), Path +0x48 (PATH_ID).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
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
    unsigned char _pad00[0xd5];
    bool MayorBribePathTaken;                            // +0xd5 (unnamed)
    bool MayorExposePathTaken;                           // +0xd6 (unnamed)
    bool MayorBribed;                                    // +0xd7 (unnamed)
};
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    CMasterScriptData* PMasterData;                      // +0x44
    long Path;                                           // +0x48 PATH_ID
    void PhaseFive();
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;

void NScript::CV_ExposeMayorScript::PhaseFive()
{
    if (Path == 1) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("V_ParanoidWhispers", 0);
        Game->DeactivateQuestLater("V_MayorsInvitation", 0);
        Game->DeactivateQuestLater("V_HeroDuel", 0);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_EXPOSE_MAYOR_OBJECTIVE_05", "BowerstonePosh", "");
        while (!Game->IsLevelLoaded("BowerstonePosh")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptThing ladyGrey = Game->GetThingWithScriptName("LadyGrey");
        Game->RemoveThing(ladyGrey, false, true);
        PMasterData->MayorExposePathTaken = true;
    } else if (Path == 2) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("V_ParanoidWhispers", 0);
        PMasterData->MayorBribePathTaken = true;
        Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->AddScreenMessage("Player got to stage three without choosing a path!", (ETextGroupSelectionMethod)0);
    }
}