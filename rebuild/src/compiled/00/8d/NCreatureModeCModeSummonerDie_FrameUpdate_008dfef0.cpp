// NCreatureMode::CModeSummonerDie::FrameUpdate() -- vtable slot 3: count the dying frames down; when they run out, a player-state creature is given the summoner-dead action
// Leaf: ?FrameUpdate@CModeSummonerDie@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008DFEF0, 94 bytes (+2 int3 padding).
// Name: slot 3 of the vtable 0x012682EC (rtti-vtables.tsv); slot 3 is CModeBase::FrameUpdate (the landed
// 0x0073AC50 is the base body CModeUnderground's slot 3 calls first), slot 5 the landed OnActivate 0x008D6920 that
// arms the counter at +0x74.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030 (action code
// 250, then the derived CCreatureAction_SummonerDead vtable 0x01287174 from its inline constructor),
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, ~CCombatActionBase @ 0x0062E900.
// Layout: CModeBase {vptr, creature +4}; m_framesToDie +0x74 (as the landed OnActivate); the creature's state byte +0x10
// (1 = the player creature, as OnActivate). Frame: the 8-byte CSoundPair() temporary, then the 0x114-byte action.
// Shape: `if (--count <= 0)` (decrement, compare, store); the zero register of the compare is reused for the
// CSoundPair zero stores, which land after the three argument pushes; the action is an unnamed temporary passed
// by reference and destroyed by the base destructor. Plain `ret`.
class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

class CCreatureActionBase { public:
    virtual void v00();
};

class CCombatActionBase : public CCreatureActionBase { public:   // retail sizeof 0x114
    unsigned char _body[0x110];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                  // 0x0062E900
};

class CThingCreatureBase { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                 // +0x10 (1 = the player creature)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CCreatureAction_SummonerDead : public CCombatActionBase { public:   // vtable 0x01287174
    virtual void v00();
    CCreatureAction_SummonerDead(CThingCreatureBase* owner)
        : CCombatActionBase(owner, 250, CSoundPair())
    {
    }
};

namespace NCreatureMode {

struct CModeBase {                         // CTCCreatureModeManager::CModeBase
    virtual ~CModeBase();
    CThingCreatureBase* m_pThing;          // +0x04
};

struct CModeSummonerDie : CModeBase {
    unsigned char _pad08[0x74 - 0x08];     // CModeStandBase state
    long          m_framesToDie;           // +0x74
    virtual void FrameUpdate();
};

void CModeSummonerDie::FrameUpdate()
{
    if (--m_framesToDie <= 0) {
        CThingCreatureBase* creature = m_pThing;
        if (creature->m_state == 1)
            creature->SetCurrentAction(CCreatureAction_SummonerDead(creature));
    }
}

} // namespace NCreatureMode