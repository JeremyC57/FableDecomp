#include <stdio.h>
// Behaviour test for 0x00A03B30 CInputEvent::~CInputEvent (links the row object).
// NF139 (2026-11-20, q4299): rewritten on engine/CInputEvent.h (was a print-only stub); PassPattern unchanged. The destructor is
//   empty: running it on an event's storage must leave every byte unchanged. The constructor (0x00A04410) is not linked: the
//   destructor is called explicitly on a filled block.
#include "engine/CInputEvent.h"

static unsigned char g_storage[sizeof(CInputEvent)];

int main()
{
    for (unsigned int i = 0; i < sizeof(g_storage); ++i)
        g_storage[i] = (unsigned char)(0x40 + i);
    CInputEvent* event = (CInputEvent*)g_storage;
    event->~CInputEvent();
    for (unsigned int i = 0; i < sizeof(g_storage); ++i)
    {
        if (g_storage[i] != (unsigned char)(0x40 + i))
        {
            printf("FAIL_00a03b30 byte %u\n", i);
            return 1;
        }
    }
    printf("OK_00a03b30");
    return 0;
}
