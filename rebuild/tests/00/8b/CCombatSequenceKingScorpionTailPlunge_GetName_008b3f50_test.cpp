// Behaviour test for 0x008B3F50 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_KING_SCORPION_TAIL_PLUNGE", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
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


static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((CCombatSequence_KingScorpionTailPlunge*)g_storage)->CCombatSequence_KingScorpionTailPlunge::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_KING_SCORPION_TAIL_PLUNGE") != 0 || g_slot != where) {
            printf("FAIL_008b3f50 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("KINGSCORPION_GETNAME_OK\n");
    return 0;
}
