// Behaviour test for 0x004DB1DA CTCActionUseActivateQuest::GetClassTypeName (NF412 respell): links the row object, supplies
// CTCActionUseActivateQuest::StatGetClassTypeName, and asserts that the row calls it once, constructs the result straight into the caller's
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

class CTCActionUseActivateQuest
{
public:
    static CCharString __fastcall StatGetClassTypeName();
    virtual CCharString GetClassTypeName() const;           // definition linked from the row object
};

CCharString __fastcall CTCActionUseActivateQuest::StatGetClassTypeName()
{
    ++g_calls;
    return CCharString("CTCActionUseActivateQuest");
}

int main(void)
{
    CTCActionUseActivateQuest tc;
    const CTCActionUseActivateQuest& ref = tc;
    CCharString r = ref.GetClassTypeName();
    if (g_calls != 1 || r.m_text == 0 || strcmp(r.m_text, "CTCActionUseActivateQuest") != 0 || g_slot != (const void*)&r) {
        printf("FAIL_004db1da calls=%d text=%s\n", g_calls, r.m_text ? r.m_text : "(null)");
        return 1;
    }
    printf("PASS_004db1da\n");
    return 0;
}
