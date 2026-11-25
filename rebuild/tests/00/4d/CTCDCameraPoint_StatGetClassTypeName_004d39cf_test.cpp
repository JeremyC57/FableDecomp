// Behaviour test for 0x004D39CF CTCDCameraPoint::StatGetClassTypeName (NF388 literal audit, NF402 respell): links the row object
// and asserts what the row hands to CCharString::CCharString(const char*, long): the retail text "CTCDCameraPoint", length -1,
// one call, constructed straight into the hidden return slot.
#include <stdio.h>
#include <string.h>

class CCharString
{
public:
    char* m_data;
    CCharString(const char* text, long length);
};

class CTCDCameraPoint
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // definition linked from the row object
};

static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
CCharString::CCharString(const char* text, long length) { m_data = 0; g_text = text; g_len = length; ++g_calls; g_slot = this; }

int main(void)
{
    CCharString r = CTCDCameraPoint::StatGetClassTypeName();
    if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CTCDCameraPoint") != 0 || g_slot != (const void*)&r) {
        printf("FAIL_004d39cf calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
        return 1;
    }
    printf("GETACTIONNAME_OK\n");
    return 0;
}
