// Behaviour test for 0x00499A70 CProgressDisplay::SetToDisplayText(bool) on the shared engine/CProgressDisplay.h:
// false -> DisplayText (+0x7C) cleared, TextTag = retail's pooled "" (NF413: asserted by text), TextTagIndex (+0x44) = 0, no CalculateNextTextTag;
// true -> DisplayText set, CalculateNextTextTag called once, no assignment, TextTagIndex left to it.
#include "engine/CProgressDisplay.h"

#include <stdio.h>
#include <string.h>

namespace
{
    int g_AssignmentCalls = 0;
    int g_CalculateCalls = 0;
    const char* g_AssignedText = 0;
    unsigned char* g_Storage = 0;
}

const CCharString& CCharString::operator=(const char* text)
{
    ++g_AssignmentCalls;
    g_AssignedText = text;
    return *this;
}

void CProgressDisplay::CalculateNextTextTag()
{
    ++g_CalculateCalls;
    *reinterpret_cast<long*>(g_Storage + 0x44) = 7;
}

int main()
{
    static unsigned char storage[sizeof(CProgressDisplay)];
    memset(storage, 0, sizeof(storage));
    g_Storage = storage;
    CProgressDisplay* display = reinterpret_cast<CProgressDisplay*>(storage);
    long* index = reinterpret_cast<long*>(storage + 0x44);

    *index = 9;
    storage[0x7c] = 1;
    display->CProgressDisplay::SetToDisplayText(false);          // direct (non-virtual) call: no vtable in raw storage
    if (storage[0x7c] != 0 || *index != 0 || g_AssignmentCalls != 1 || g_AssignedText == 0 || g_AssignedText[0] != 0 ||
        g_CalculateCalls != 0)
        return 1;

    *index = 9;
    display->CProgressDisplay::SetToDisplayText(true);
    if (storage[0x7c] != 1 || *index != 7 || g_AssignmentCalls != 1 || g_CalculateCalls != 1)
        return 1;

    printf("FABLETLC_PROGRESS_DISPLAY_TEXT_MODE_BEHAVIOR PASS\n");
    return 0;
}
