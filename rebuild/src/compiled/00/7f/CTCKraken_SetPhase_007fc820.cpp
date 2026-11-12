// CTCKraken::SetPhase(EKrakenBossPhase phase) -- record the phase and pass it to the boss component; phases 0/3 take the kraken out of combat, phases 1/2 put it in combat against a live target and (re)start the tentacles and head
// Leaf: ?SetPhase@CTCKraken@@QAEXJ@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC820, 180 bytes incl. the four-entry jump table 0x007FC8C4 (`ret 4`).
// Callees: CTCBase::GetWorld @ 0x00686A70, CTCBoss::SetNextBossPhase(long, bool) @ 0x007AEDA0, CCombatManager::IsAttacker
// @ 0x006E92C0 (const; was the descriptive IsCreatureAttacking, agentNF25b), DeregisterCombatant @ 0x006ED9C0, RegisterCombatant @ 0x006EDC50, IS_THING_ALIVE @ 0x004CC340 (`__fastcall`),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CTCKraken::CreateTentacles @ 0x007FC210, RaiseSunkenTentacles @ 0x007FB610,
// EndHeadEffects @ 0x007FB150 (the last three through the inline InitTentaclesAndHead, landed out of line at 0x007FC710).
// Layout (retail; the VC6-style std::list is a single 4-byte head pointer, so CTCKraken sits 8 below Ego_r.pdb from +0x30): PTarget
// +0x20 (CIntelligentPointer<CThing>), PTCBoss +0x40, Tentacles +0x44 (head node), Phase +0x60, Mode +0x64; CWorld's combat manager
// +0x4C.
// Shape: the combat manager and the parent thing are named locals read before the boss call (edi/ebx); the switch re-reads Phase.
// The target is a named local (GetPItem runs before the `push 0`); the tentacle list is a class whose inline empty() compares
// begin() with end() -- a direct `head->next == head` read is hoisted above the Mode store.
class CThing;
class CThingCreatureBase;

class CCombatManager { public:
    bool IsAttacker(const CThingCreatureBase& creature) const;                  // 0x006E92C0 ?IsAttacker@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z (Ego_r + FableWin)
    void DeregisterCombatant(CThingCreatureBase* creature);                     // 0x006ED9C0
    void RegisterCombatant(CThingCreatureBase* creature, CThing* target, bool hostile);   // 0x006EDC50
};

class CWorld { public:
    unsigned char   _pad00[0x4c];
    CCombatManager* CombatManager;                       // +0x4c
};

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointer_CThing : CBaseIntelligentPointer { };
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer_CThing& thing);   // 0x004CC340

class CTCBoss { public:
    void SetNextBossPhase(long phase, bool force);       // 0x007AEDA0
};

struct SListNode { SListNode* next; SListNode* prev; };
struct CThingList {                                      // VC6-style std::list<CIntelligentPointer<CThing> >: the head node only
    SListNode* m_head;
    SListNode* begin() const { return m_head->next; }
    SListNode* end() const { return m_head; }
    bool empty() const { return begin() == end(); }
};

typedef long EKrakenBossPhase;

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCKraken : public CTCBase {
public:
    void*                      __vfptr;                  // +0x00
    CThingCreatureBase*        ParentThing;              // +0x04 (CTCBase)
    unsigned char              _pad08[0x20 - 0x08];
    CIntelligentPointer_CThing PTarget;                  // +0x20
    unsigned char              _pad28[0x40 - 0x28];
    CTCBoss*                   PTCBoss;                  // +0x40
    CThingList                 Tentacles;                // +0x44
    unsigned char              _pad48[0x60 - 0x48];
    EKrakenBossPhase           Phase;                    // +0x60
    long                       Mode;                     // +0x64 EKrakenMode
private:
    void CreateTentacles();                              // 0x007FC210
public:
    void RaiseSunkenTentacles();                         // 0x007FB610
    void EndHeadEffects();                               // 0x007FB150
    void InitTentaclesAndHead()
    {
        if (Tentacles.empty())
            CreateTentacles();
        RaiseSunkenTentacles();
        EndHeadEffects();
    }
    void SetPhase(EKrakenBossPhase phase);
};

void CTCKraken::SetPhase(EKrakenBossPhase phase)
{
    CCombatManager* combat = GetWorld().CombatManager;
    CThingCreatureBase* thing = ParentThing;
    Phase = phase;
    PTCBoss->SetNextBossPhase(phase, false);
    switch (Phase) {
    case 0:
    case 3:
        if (combat->IsAttacker(*thing))
            combat->DeregisterCombatant(thing);
        break;
    case 1:
    case 2:
        if (!combat->IsAttacker(*thing)) {
            if (IS_THING_ALIVE(PTarget)) {
                CThing* target = ((CThing*)PTarget.GetPItem());
                combat->RegisterCombatant(thing, target, false);
            }
        }
        Mode = 1;
        InitTentaclesAndHead();
        break;
    }
}