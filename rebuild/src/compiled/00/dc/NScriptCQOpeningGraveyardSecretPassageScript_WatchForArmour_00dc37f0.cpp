// CQ_OpeningGraveyardSecretPassageScript::WatchForArmour() -- shows a 0-of-4 quest counter and waits until the hero holds the ceremonial sword, shield, armour and helmet, updating the counter and the quest-card objective as each piece turns up
// Leaf: WatchForArmour
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC37F0, 987 bytes, plain `ret`.
// Callees: CCharString 0x0099EBF0 / 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the quest's
//   interface (+0x40): NewScriptFrame (7), GetHero (70), IsObjectInThingsPossession (184), SetQuestCardObjective (296),
//   DisplayQuestInfo (321), AddQuestInfoCounter (327), UpdateQuestInfoCounter (335), RemoveQuestInfoElement (338),
//   GetActiveQuestName (655).
// Shape: four `!have && Possess("...")` tests whose conditional CCharString temporaries carry the ebx flag bits; the
//   helmet branch jumps straight into the objective update (changed is known true); the found count is the sum of the
//   four bools.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing;

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_OpeningGraveyardSecretPassageScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void WatchForArmour();
};

void CQ_OpeningGraveyardSecretPassageScript::WatchForArmour()
{
    long counter = GSI->AddQuestInfoCounter("HUD_NOSTRO_ARMOUR_ICON", 4, 1.0f);
    GSI->DisplayQuestInfo(true);
    bool haveSword = false;
    bool haveShield = false;
    bool haveArmour = false;
    bool haveHelmet = false;
    long found;
    do {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        bool changed = false;
        if (!haveSword && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_SWORD", GSI->GetHero())) {
            if (IsActiveThreadTerminating())
                return;
            haveSword = true;
            changed = true;
        }
        if (!haveShield && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_SHIELD", GSI->GetHero())) {
            if (IsActiveThreadTerminating())
                return;
            haveShield = true;
            changed = true;
        }
        if (!haveArmour && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_ARMOUR", GSI->GetHero())) {
            if (IsActiveThreadTerminating())
                return;
            haveArmour = true;
            changed = true;
        }
        if (!haveHelmet && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_HELMET", GSI->GetHero())) {
            if (IsActiveThreadTerminating())
                return;
            haveHelmet = true;
            changed = true;
        }
        if (changed) {
            if (IsActiveThreadTerminating())
                return;
            GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03A", "", "Graveyard");
        }
        found = haveSword + haveShield + haveArmour + haveHelmet;
        GSI->UpdateQuestInfoCounter(counter, found, -1);
    } while (found != 4);
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03B", "", "Graveyard");
    GSI->RemoveQuestInfoElement(counter);
    GSI->DisplayQuestInfo(false);
}
}