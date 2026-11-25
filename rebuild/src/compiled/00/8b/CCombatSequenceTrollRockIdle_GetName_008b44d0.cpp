// CCombatSequence_TrollRockIdle::GetName  @ 008b44d0
// Returns a CCharString by value, constructed from a string literal pointer.

class CCharString {
public:
    CCharString(const char* s, long n);
    void* m_data;
};

class CActionDoCreatureAction;

class CCombatSequence_TrollRockIdle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockIdle::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_TROLL_ROCK_IDLE", -1);
}