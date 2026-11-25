// CCombatSequence_VillagerNearAttack::GetName
// Returns a CCharString by value built from a static string literal.
// Retail: push -1; push 0x1278cb8; ecx=retbuf; call CCharString::ctor; return retbuf.

class CCharString {
public:
    CCharString(const char* s, long len);
    char* m_data;
};

struct CActionDoCreatureAction;

class CCombatSequence_VillagerNearAttack {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_VillagerNearAttack::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_VILLAGER_NEAR_ATTACK", -1);
}