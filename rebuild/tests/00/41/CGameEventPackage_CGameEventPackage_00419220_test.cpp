// test for 00419220 CGameEventPackage ctor
#include <stdio.h>
#include <string.h>
#include <new>
#include "engine/CGameEvent.h"
struct CGameEvent_Init : CGameEvent {
    CGameEvent_Init() { Player = -1; Valid = false; }
};
class CGameEventPackage {
public:
    CGameEventPackage();
    long            TimeStamp;                           // +0x00
    unsigned long   NoEvents;                            // +0x04
    CGameEvent_Init Events[40];                          // +0x08
};
CGameEventPackage::CGameEventPackage()
    : NoEvents(0)
{
}

int main()
{
    static unsigned char buf[0x648]; memset(buf, 0x77, sizeof(buf));
    CGameEventPackage* p = new (buf) CGameEventPackage;
    int ok = sizeof(CGameEventPackage) == 0x648 && p->NoEvents == 0 && *(long*)buf == 0x77777777;
    for (int i = 0; i < 40; ++i) {
        unsigned char* e = buf + 8 + i * 0x28;
        ok &= e[4] == 0xff && e[0x26] == 0 && e[0] == 0x77 && e[5] == 0x77 && e[0x25] == 0x77 && e[0x27] == 0x77;
    }
    printf(ok ? "00419220_TEST PASS\n" : "00419220_TEST FAIL\n");
    return ok ? 0 : 1;
}