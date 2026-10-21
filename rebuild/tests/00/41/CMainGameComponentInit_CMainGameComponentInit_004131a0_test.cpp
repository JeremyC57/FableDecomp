// test for 004131a0 CMainGameComponentInit ctor
#include <stdio.h>
#include <string.h>
#include <new>
class CWideString {
public:
    CWideString();                                       // 0x0099AED0
    void* PData;
};
class CCharString {
public:
    CCharString();                                       // 0x0099E4B0
    void* PData;
};
class CMainGameComponentInit {
public:
    CMainGameComponentInit();
    CWideString InitialWorldName;                        // +0x00
    CWideString InitialWorldHolySiteName;                // +0x04
    CCharString InitialQuestName;                        // +0x08
    CWideString SaveGameName;                            // +0x0c
};
CMainGameComponentInit::CMainGameComponentInit()
{
}

static void* g_order[8]; static int g_n;
CWideString::CWideString() { PData = (void*)1; g_order[g_n++] = this; }
CCharString::CCharString() { PData = (void*)2; g_order[g_n++] = this; }
int main()
{
    static unsigned char buf[16];
    CMainGameComponentInit* p = new (buf) CMainGameComponentInit;
    int ok = sizeof(*p) == 16 && g_n == 4 && g_order[0] == buf && g_order[1] == buf + 4 && g_order[2] == buf + 8
          && g_order[3] == buf + 12 && p->InitialQuestName.PData == (void*)2 && p->SaveGameName.PData == (void*)1;
    printf(ok ? "004131a0_TEST PASS\n" : "004131a0_TEST FAIL\n");
    return ok ? 0 : 1;
}