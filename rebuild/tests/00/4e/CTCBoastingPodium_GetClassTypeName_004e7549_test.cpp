// Behaviour test for 0x004E7549 CTCBoastingPodium::GetClassTypeName (NF412 respell): links the row object, supplies
// CTCBoastingPodium::StatGetClassTypeName, and asserts that the row calls it once, constructs the result straight into the caller's
// hidden return slot (the static's object is the caller's own), and hands back that object.
#include <stdio.h>
#include <string.h>

static int g_calls = 0;
static const void* g_slot = 0;

class CCharString
{
public:
    const char* m_text;
    CCharString(const char* text) { m_text = text; g_slot = this; }   // user ctor: returned through a hidden slot, as in the row
};

class CTCBoastingPodium
{
public:
    static CCharString __fastcall StatGetClassTypeName();
    virtual CCharString GetClassTypeName() const;           // definition linked from the row object
};

CCharString __fastcall CTCBoastingPodium::StatGetClassTypeName()
{
    ++g_calls;
    return CCharString("CTCBoastingPodium");
}

int main(void)
{
    CTCBoastingPodium tc;
    const CTCBoastingPodium& ref = tc;
    CCharString r = ref.GetClassTypeName();
    if (g_calls != 1 || r.m_text == 0 || strcmp(r.m_text, "CTCBoastingPodium") != 0 || g_slot != (const void*)&r) {
        printf("FAIL_004e7549 calls=%d text=%s\n", g_calls, r.m_text ? r.m_text : "(null)");
        return 1;
    }
    printf("PASS_004e7549\n");
    return 0;
}
