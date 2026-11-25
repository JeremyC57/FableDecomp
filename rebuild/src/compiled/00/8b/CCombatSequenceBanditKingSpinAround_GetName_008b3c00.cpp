// CCombatSequence_BanditKingSpinAround::GetName  @ 008b3c00
// __fastcall, returns CCharString by value (hidden ret-buffer ptr).
// Body: construct CCharString(0x12788d8, -1) into the return buffer, return it.

class CCharString
{
public:
    CCharString(const char* s, long n);
    void* m_data;
};

class CCombatSequence_BanditKingSpinAround
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditKingSpinAround::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND", -1);
}