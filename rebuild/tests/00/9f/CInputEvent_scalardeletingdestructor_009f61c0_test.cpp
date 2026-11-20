#include <cstdio>
#include <cstdlib>
// Behaviour test for 0x009F61C0 CInputEvent::`scalar deleting destructor' (the compiler-generated ??_GCInputEvent@@QAEPAXI@Z).
// NF142 (2026-11-20, q4299): rewritten on engine/CInputEvent.h, the header the row now uses (the old test modelled the
//   CLandscapeBackgroundPatch stand-in). A delete of a CInputEvent runs the scalar deleting destructor (a pick-any COMDAT, the same one
//   the row emits): the destructor runs on the object, then operator delete frees it. PassPattern unchanged.
#include "engine/CInputEvent.h"

static int g_freed = 0;
static int g_destroyed = 0;
static const void* g_last_dtor = 0;
static const void* g_last_freed = 0;

CInputEvent::~CInputEvent() { g_last_dtor = this; ++g_destroyed; }

void operator delete(void* p)
{
    ++g_freed;
    g_last_freed = p;
    std::free(p);
}

int main()
{
    CInputEvent* event = (CInputEvent*)std::malloc(sizeof(CInputEvent));
    delete event;
    if (g_destroyed == 1 && g_freed == 1 && g_last_dtor == event && g_last_freed == event) {
        std::printf("CLandscapeBackgroundPatch_009f61c0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL d=%d f=%d\n", g_destroyed, g_freed);
    return 1;
}
