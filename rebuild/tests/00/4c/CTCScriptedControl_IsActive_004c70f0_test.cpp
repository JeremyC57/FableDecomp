// Behaviour test for 0x004C70F0 CTCScriptedControl::IsActive() const: links the row object over the shared engine/CTCScriptedControl.h
// and supplies IsActionListEmpty (0x007122C0); checks locked / empty / non-empty combinations through the Locked byte (+0x18).
// (NF108 2026-11-19: rewritten for the const bool member; the old test was a self-contained copy of the long-returning stand-in.)
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISACTIONLISTEMPTY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCScriptedControl.h"

static bool g_empty = true;
bool CTCScriptedControl::IsActionListEmpty() const { return g_empty; }

int main()
{
    static unsigned char storage[0x40];
    std::memset(storage, 0, sizeof(storage));
    const CTCScriptedControl* control = reinterpret_cast<const CTCScriptedControl*>(storage);
    g_empty = true; storage[0x18] = 0;
    if (control->IsActive()) { std::printf("004c70f0_TEST FAIL 1\n"); return 1; }
    storage[0x18] = 1;
    if (!control->IsActive()) { std::printf("004c70f0_TEST FAIL 2\n"); return 1; }
    storage[0x18] = 0; g_empty = false;
    if (!control->IsActive()) { std::printf("004c70f0_TEST FAIL 3\n"); return 1; }
    std::printf("004c70f0_TEST PASS\n");
    return 0;
}
