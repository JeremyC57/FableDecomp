// behaviour test for 0x004D4249 CTCDInternalMarker::StatGetClassTypeName
#include <stdio.h>
#include <string.h>
class CCharStringData;
static const char* g_last; static long g_len;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1) { g_last = text; g_len = length; PStringData = (CCharStringData*)text; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) {}
    ~CCharString() {}
};
class CTCDInternalMarker {
public:
    static CCharString __fastcall StatGetClassTypeName();
};
CCharString __fastcall CTCDInternalMarker::StatGetClassTypeName()
{
    return CCharString("CTCDInternalMarker");
}
int main()
{
    CCharString r = CTCDInternalMarker::StatGetClassTypeName();
    int ok = g_last && strcmp(g_last, "CTCDInternalMarker") == 0 && g_len == -1 && (const char*)r.PStringData == g_last;
    printf(ok ? "004d4249_TEST PASS\n" : "004d4249_TEST FAIL\n");
    return ok ? 0 : 1;
}