// CCombatSequence_BanditIdleNear::GetName  @ 008b3910
// __fastcall, returns CCharString by value; param CActionDoCreatureAction* (unused).
// Body: builds CCharString("COMBAT_SEQUENCE_BANDIT_IDLE_SHORT", -1) into the hidden return buffer, returns it.

class CCharString {
public:
    char *m_data;
    CCharString(const char *s, long n);   // ctor @ 0x99ebf0
};

class CCombatSequence_BanditIdleNear {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditIdleNear::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_IDLE_SHORT", -1);
}