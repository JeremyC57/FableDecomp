// Behaviour test for 0x008bcf20 CCreatureAction_TeleportOut constructor (owner, target, flag).
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
class CThing { public: C3DVector m_pos; const C3DVector& GetPosition() const { return m_pos; } };
class CCharString { public: char m_text[32]; CCharString(const char* t, long) { std::strcpy(m_text, t); } };
class CSoundPair { public: int m_first, m_second; CSoundPair() : m_first(0), m_second(0) { } };
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CCombatActionBase { public:
    CBaseIntelligentPointer PCreature; CThing* m_thing; int m_code; char m_name[32];
    CCombatActionBase(CThing* owner, int code, const CSoundPair&) : m_thing(0), m_code(code) { PCreature.m_pItem = owner; m_name[0] = 0; }
    CCombatActionBase(CThing* owner, CThing* thing, int code, const CSoundPair&) : m_thing(thing), m_code(code) { PCreature.m_pItem = owner; m_name[0] = 0; }
    void SetName(const CCharString& name) { std::strcpy(m_name, name.m_text); }
};
struct CAction : CCombatActionBase {
    C3DVector TargetPos; bool Instant;
    CAction(CThing* owner) : CCombatActionBase(owner, 250, CSoundPair()), Instant(false)
    {
        { CCharString name("ST_TELEPORT", -1); SetName(name); }
        TargetPos = PCreature.GetPItem()->GetPosition();
    }
    CAction(CThing* owner, const C3DVector& target, bool instant) : CCombatActionBase(owner, 250, CSoundPair()), TargetPos(target), Instant(instant)
    {
        { CCharString name("ST_TELEPORT_OUT", -1); SetName(name); }
    }
    CAction(CThing* owner, CThing* thing, const C3DVector& target, bool instant) : CCombatActionBase(owner, thing, 250, CSoundPair()), TargetPos(target), Instant(instant)
    {
        { CCharString name("ST_TELEPORT_OUT", -1); SetName(name); }
    }
};
int main()
{
    CThing owner; owner.m_pos.X = 5; owner.m_pos.Y = 6; owner.m_pos.Z = 7;
    CThing other;
    C3DVector target; target.X = 1; target.Y = 2; target.Z = 3;
#if 2 == 1
    CAction a(&owner);
    bool ok = a.TargetPos.X == 5 && a.TargetPos.Z == 7 && std::strcmp(a.m_name, "ST_TELEPORT") == 0 && a.m_thing == 0;
#elif 2 == 2
    CAction a(&owner, target, true);
    bool ok = a.TargetPos.X == 1 && a.TargetPos.Z == 3 && a.Instant && std::strcmp(a.m_name, "ST_TELEPORT_OUT") == 0 && a.m_thing == 0;
#else
    CAction a(&owner, &other, target, true);
    bool ok = a.TargetPos.Y == 2 && a.Instant && std::strcmp(a.m_name, "ST_TELEPORT_OUT") == 0 && a.m_thing == &other;
#endif
    if (!ok || a.m_code != 250 || a.PCreature.GetPItem() != &owner) { std::printf("008bcf20_TEST FAIL\n"); return 1; }
    std::printf("008bcf20_TEST PASS\n");
    return 0;
}