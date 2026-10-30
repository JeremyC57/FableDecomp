// CDarkwoodRockTroll::OnPredicateFail() -- when the rock troll reports it was killed and the parent escort script's attack flag is set, point the active quest card's objective at Barrowfields and clear the flag
// Leaf: OnPredicateFail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E03B00, length 226 bytes (+ int3 padding), `ret`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literals "" @ 0x0122D70E, "BarrowFields" @ 0x012C5D94,
// "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03" @ 0x012DF66C), ~CCharString @ 0x0099EAE0, CScriptThing slot 18
// MsgIsKilledBy(const CCharString&) (+0x48), CGameScriptInterface slot 655 GetActiveQuestName() (+0xa3c, by value) and
// slot 296 SetQuestCardObjective(4 x const CCharString&) (+0x4a0).
// Layout = Ego_r.pdb CDarkwoodRockTroll (32 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14 (CQ_TraderEscortScript; its byte +0x55 is an unnamed PDB pad, named descriptively), PMasterData +0x18,
// HealthBarID +0x1c. Same TU family as the landed CTraderToRescue::OnPredicateFail (0x00DFB100).
// Shape: one `if (killed-by // Shape: the killed-by string is a named local in its own block with the bool (bl) declared outside; the three literal// Shape: the killed-by string is a named local in its own block with the bool (bl) declared outside; the three literal flag)` condition: the "" temporary is destroyed after the bool is formed in bl (the
// conditional form also gives the dead zero store at the result slot); the three literal
// arguments are implicit conversion temporaries built right to left before the GetActiveQuestName call, whose result
// is the first argument; the vtable of the second call is fetched (edi) before the first call.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptThing { public:                              // vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48, 0x004AAAF0)
    long m_data[2];
};
struct CQ_TraderEscortScript {
    unsigned char _pad[0x55];
    bool TrollAttackPending;                       // +0x55 (unnamed in the PDB)
};
struct CQ_SunnyvaleMasterData;
class CDarkwoodRockTroll {
public:
    void*                        __vftable;        // +0x00
    CGameScriptInterface*        Game;             // +0x04
    CScriptThing                 Me;               // +0x08
    CQ_TraderEscortScript*       ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*      PMasterData;      // +0x18
    long                         HealthBarID;      // +0x1c
    void OnPredicateFail();
};
void CDarkwoodRockTroll::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("") && ParentClass->TrollAttackPending)
    {
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields");
        ParentClass->TrollAttackPending = false;
    }
}