// Behaviour test for 0x004C7440 CThing::SetPlayerNumber(long): links the row object and calls it on a filled CThing-sized buffer
// through the shared engine/CThing.h declaration; checks the signed char at +0x90 (PlayerNumber) and that the neighbouring bytes
// are untouched. (NF108 2026-11-19: replaces the old inline-assembly call of the __fastcall EnableNavigator stand-in.)
#include <cstdio>
#include <cstring>
#include <stddef.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "fable_string.h"
#include "engine/CThing.h"

int main()
{
    static unsigned char buffer[sizeof(CThing) + 16];
    std::memset(buffer, 0x11, sizeof(buffer));
    CThing* thing = reinterpret_cast<CThing*>(buffer);
    thing->SetPlayerNumber(0x0102L);
    bool ok = buffer[0x90] == 0x02 && buffer[0x8f] == 0x11 && buffer[0x91] == 0x11;
    thing->SetPlayerNumber(-1L);
    ok = ok && buffer[0x90] == 0xff && buffer[0x91] == 0x11;
    if (!ok) { std::printf("FAIL player=%d\n", buffer[0x90]); return 1; }
    std::printf("OK_0x004c7440\n");
    return 0;
}
