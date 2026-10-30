// CMurderTwistAssassin::OnPredicateFail() -- when the assassin is killed (by anyone): set the parent quest's state to 4, give the hero the def's morality and renown, give the hero the def's reward object, and set the active quest card's objective to "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_RETURN" in "OakBay"
// Leaf: ?OnPredicateFail@CMurderTwistAssassin@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB3D10, 300 bytes, plain `ret` (virtual).
// Callees: CScriptThing slot 18 MsgIsKilledBy(const CCharString&) (+0x48) on the entity's own thing (+8); CCharString(const
//   char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CDefString::operator CCharString() 0x00415D70; CGameScriptInterface
//   (ghidra_out/cgsi_slots_aligned.tsv) slots 156 GiveHeroMorality(float) (+0x270) 0x00899EF0, 161 GiveHeroRenownPoints(long)
//   (+0x284) 0x008990C0, 121 GiveHeroObject(const CCharString&, long, bool) (+0x1e4) 0x008902E0, 655 GetActiveQuestName()
//   (+0xa3c, CCharString by value) 0x00891880 and 296 SetQuestCardObjective(4 x const CCharString&) (+0x4a0) 0x00896A30.
//   String "" 0x0122D70E.
// Data: the script def 0x0143E90C: morality +0x704 (float), renown +0x714 (long), reward object +0x718 (CDefString).
// Layout (as the landed CV_SickChildScript::CWomanToAttract::OnPredicateFail 0x00ECD390): interface +4, Me +8,
//   ParentClass +0x14 (its quest state long at +0x4c).
// Shape: the slot-296 vtable is fetched before the nested slot-655 call (plain nested call); the three literal temporaries
//   (the objective's trailing arguments) are built right to left before the quest-name result slot is pushed.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString { public:
    long Index;
    operator CCharString() const;                  // 0x00415D70
};
class CScriptDef { public:
    unsigned char _pad[0x704];
    float         MurderTwistMorality;             // +0x704
    unsigned char _pad708[0x714 - 0x708];
    long          MurderTwistRenown;               // +0x714
    CDefString    MurderTwistReward;               // +0x718
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CScriptThingMethods {
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptThing : public CScriptThingMethods { public:        // 12 bytes, vtable 0x01238C8C
    void* PImp_Data;
    void* PImp_Info;
};
struct CMurderTwistScript {
    unsigned char _pad[0x4c];
    long          State;                           // +0x4c
};
struct CMurderTwistAssassin {
    virtual void v00();
    CGameScriptInterface* Game;                    // +0x04
    CScriptThing          Me;                      // +0x08
    CMurderTwistScript*   ParentClass;             // +0x14
    void*                 PMasterData;             // +0x18
    virtual void OnPredicateFail();
};
void CMurderTwistAssassin::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->State = 4;
        Game->GiveHeroMorality(pGlobalScriptDef->MurderTwistMorality);
        Game->GiveHeroRenownPoints(pGlobalScriptDef->MurderTwistRenown);
        Game->GiveHeroObject(pGlobalScriptDef->MurderTwistReward, -1, false);
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_MURDER_TWIST_OBJECTIVE_RETURN", "OakBay", "");
    }
}