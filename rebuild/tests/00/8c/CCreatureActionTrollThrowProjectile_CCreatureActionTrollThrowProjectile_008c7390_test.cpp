// Behaviour test for 0x008c7390 CCreatureAction_TrollThrowProjectile constructor (variant 1).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector m_pos; const C3DVector& GetPosition() const { return m_pos; } };
class CSoundPair { public: int m_first, m_second; CSoundPair() : m_first(0), m_second(0) { } };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
class CCombatActionBase { public:
    CThing* m_owner; CThing* m_thing; int m_code;
    CCombatActionBase(CThing* owner, CThing* thing, int code, const CSoundPair&) : m_owner(owner), m_thing(thing), m_code(code) { }
    CCombatActionBase(const CCombatActionBase& o) : m_owner(o.m_owner), m_thing(o.m_thing), m_code(o.m_code) { }
};
struct CAction : CCombatActionBase {
    CIntelligentPointer PProjectile; C3DVector TargetPos; bool TargetIsPosition; int ThrowFrame;
    CAction(CThing* owner, CThing* thing, const C3DVector& target) : CCombatActionBase(owner, thing, 150, CSoundPair())
    { TargetPos = target; TargetIsPosition = true; ThrowFrame = 0; }
    CAction(CThing* owner, CThing* thing) : CCombatActionBase(owner, thing, 150, CSoundPair())
    { TargetPos = thing->GetPosition(); TargetIsPosition = false; ThrowFrame = 0; }
    CAction(const CAction& o) : CCombatActionBase(o), PProjectile(o.PProjectile), TargetPos(o.TargetPos), TargetIsPosition(o.TargetIsPosition), ThrowFrame(o.ThrowFrame) { }
};
int main()
{
    CThing owner, thing, rock; thing.m_pos.X = 4; thing.m_pos.Y = 5; thing.m_pos.Z = 6;
    C3DVector target; target.X = 1; target.Y = 2; target.Z = 3;
#if 1 == 1
    CAction a(&owner, &thing, target);
    bool ok = a.TargetPos.X == 1 && a.TargetPos.Z == 3 && a.TargetIsPosition && a.PProjectile.GetPItem() == 0 && a.m_code == 150 && a.m_thing == &thing;
#elif 1 == 2
    CAction a(&owner, &thing);
    bool ok = a.TargetPos.X == 4 && a.TargetPos.Z == 6 && !a.TargetIsPosition && a.PProjectile.GetPItem() == 0 && a.m_code == 150 && a.m_owner == &owner;
#else
    CAction s(&owner, &thing, target); s.PProjectile.SetPItem(&rock); s.ThrowFrame = 12;
    CAction a(s);
    bool ok = a.PProjectile.GetPItem() == &rock && a.TargetPos.Y == 2 && a.TargetIsPosition && a.ThrowFrame == 12 && a.m_thing == &thing;
#endif
    if (!ok) { std::printf("008c7390_TEST FAIL\n"); return 1; }
    std::printf("008c7390_TEST PASS\n");
    return 0;
}