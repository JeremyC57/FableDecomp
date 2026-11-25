// CCombatSequence_NonInterruptableBoxStrafe::GetName  @ 008b3630
// Returns a CCharString by value, constructed from a global string literal.

class CCharString {
public:
    CCharString(const char* s, long len);
    char* m_data;
};

class CCombatSequence_NonInterruptableBoxStrafe {
public:
    virtual CCharString GetName() const;
};

static const char* const kName = "COMBAT_SEQUENCE_NON_INTERRUPTABLE_BOX_STRAFE";

CCharString CCombatSequence_NonInterruptableBoxStrafe::GetName() const
{
    return CCharString(kName, -1);
}