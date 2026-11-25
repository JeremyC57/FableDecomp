// Leaf: ?GetName@CCombatSequence_KingScorpionTailPlunge@@UBE?AVCCharString@@XZ
// CCombatSequence_KingScorpionTailPlunge::GetName  (retail 0x008b3f50)
// virtual CCharString __fastcall GetName();  returns a CCharString built from a
// string literal via CCharString(const char*, long).

class CCharString {
public:
    CCharString(const char* s, long len);
    void* m_data;
};

// The ctor lives at retail 0x0099ebf0; declaration only so the compiler emits a
// real call to it (external, not inlined).

class CCombatSequence_KingScorpionTailPlunge {
public:
    virtual CCharString GetName() const;
    int m_pad;
};

CCharString CCombatSequence_KingScorpionTailPlunge::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_KING_SCORPION_TAIL_PLUNGE", -1);
}