// Behaviour test for 0x006BBF20 CRegion::GetDisplayName (NF413: linked): links the row object (the function is declared here, not
// defined) and checks that it compares nameB with retail's pooled "" (0x0122D70E) and returns nameB when they differ, else nameA.
#include <cstdio>

static bool g_result = true;
static const void* g_lastSelf = 0;
static const char* g_lastArg = 0;
static int g_calls = 0;

class CCharString { public:
    char* data;
    bool operator!=(const char* s) const;   // the row's callee (0x0099E960), defined here
};

bool CCharString::operator!=(const char* s) const {
    g_calls++;
    g_lastSelf = this;
    g_lastArg = s;
    return g_result;
}

class CRegion { public:
    char pad[0x18];
    CCharString nameA;
    CCharString nameB;
};

CCharString* __fastcall CRegion_GetDisplayName(CRegion* region);   // definition linked from the row object

int main()
{
    CRegion r;
    g_result = true;
    CCharString* ret = CRegion_GetDisplayName(&r);
    if (ret != &r.nameB) { std::printf("FAIL: expected nameB\n"); return 1; }
    if (g_calls != 1 || g_lastSelf != (const void*)&r.nameB) { std::printf("FAIL: wrong self\n"); return 1; }
    if (g_lastArg == 0 || g_lastArg[0] != 0) { std::printf("FAIL: wrong arg (retail's pooled \"\")\n"); return 1; }
    g_result = false;
    g_lastArg = 0;
    ret = CRegion_GetDisplayName(&r);
    if (ret != &r.nameA) { std::printf("FAIL: expected nameA\n"); return 1; }
    if (g_lastArg == 0 || g_lastArg[0] != 0) { std::printf("FAIL: wrong arg 2\n"); return 1; }
    std::printf("CRegion_006bbf20_TEST PASS\n");
    return 0;
}
