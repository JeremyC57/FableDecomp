// Behaviour test for 0x008FD4BC CAIStateGroup_MinionReactToCorpse::GetName (NF413, the nf406_b2 0x004D31C0 form): links the row
// object (the static is declared here, not defined) and asserts what the row hands to CCharString::CCharString(const char*, long):
// the retail text "SG_MINION_REACT_TO_CORPSE", length -1, one call, constructed straight into the hidden return slot.
#include <stdio.h>
#include <string.h>

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length);
};

class CAIStateGroup_MinionReactToCorpse
{
public:
    static CCharString __fastcall GetName();   // definition linked from the row object
};

static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
CCharString::CCharString(const char* text, long length) { PStringData = 0; g_text = text; g_len = length; ++g_calls; g_slot = this; }

int main(void)
{
    CCharString r = CAIStateGroup_MinionReactToCorpse::GetName();
    if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "SG_MINION_REACT_TO_CORPSE") != 0 || g_slot != (const void*)&r) {
        printf("FAIL self=%p text=%s len=%ld calls=%d\n", g_slot, g_text ? g_text : "(null)", g_len, g_calls);
        return 1;
    }
    printf("008fd4bc_TEST PASS\n");
    return 0;
}
