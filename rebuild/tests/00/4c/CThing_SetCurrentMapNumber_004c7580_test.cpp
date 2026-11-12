// Behaviour test for 0x004C7580 CThing::SetCurrentMapNumber(long): links the row object and calls it on a zeroed CThing-sized
// buffer through the shared engine/CThing.h declaration; checks the short at +0x8E (CurrentMapNumber) and that the
// neighbouring bytes are untouched. (agentNF31 2026-11-12: replaces the old inline-assembly call of the __fastcall stand-in.)
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
    thing->SetCurrentMapNumber(0x00015AA5L);
    short stored;
    std::memcpy(&stored, buffer + 0x8e, sizeof(stored));
    bool ok = stored == (short)0x5AA5 && buffer[0x8d] == 0x11 && buffer[0x90] == 0x11;
    if (!ok) { std::printf("FAIL map=%d\n", stored); return 1; }
    std::printf("OK_0x004c7580 map=%d\n", stored);
    std::printf("004c7580_TEST PASS\n");
    return 0;
}
