// CCreatureAction_NymphTeleportOutOfLimbo::CCreatureAction_NymphTeleportOutOfLimbo(CThingCreatureBase*, const C3DVector*) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable (0x01282A04), the destination at +0x114 copied from the argument or, when it is null, from the owner's position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA860, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CThing::GetPosition @ 0x004C73D0 (thiscall, no arguments; returns the position by pointer).
// Shape: the null test of the vector argument straddles the vtable store (init-list member after the base), the three floats are copied one register each.
class CThingCreatureBase;
class C3DVector { public: float x, y, z; };
struct CThingCalls { const C3DVector* GetPosition(); };   // 0x004C73D0
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCreatureAction_NymphTeleportOutOfLimbo : public CCombatActionBase { public:   // vtable 0x01282A04
    C3DVector m_destination;                // +0x114
    virtual void v00();
    CCreatureAction_NymphTeleportOutOfLimbo(CThingCreatureBase* owner, const C3DVector* destination);
};

CCreatureAction_NymphTeleportOutOfLimbo::CCreatureAction_NymphTeleportOutOfLimbo(CThingCreatureBase* owner, const C3DVector* destination)
    : CCombatActionBase(owner, 150, CSoundPair())
    , m_destination(*(destination ? destination : ((CThingCalls*)owner)->GetPosition()))
{
}