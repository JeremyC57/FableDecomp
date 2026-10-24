// Behaviour test for _RTC_Initialize (0x004012CE): every non-null table entry runs once, in table order, null slots are
// skipped, nothing past __rtc_izz runs, and an initializer that faults is swallowed by its __except so the walk goes on.
// PassPattern kept from the landed test: RTC_INITIALIZE_TEST PASS.
#include <stdio.h>

// ---- reconstruction under test (verbatim copy of src/004012ce.cpp) ----
// Leaf: __RTC_Initialize
// Flags: /O1
// Grade: RELOCATION_MATCH 68/68 on the real span with /O1 (catalog row 61 stopped before the __SEH_epilog tail)
// _RTC_Initialize() -- the VC7.1 CRT walk over the .rtc$I initializer table (initsect.cpp): call every non-null entry
//   between __rtc_iaa and __rtc_izz, each inside its own __try/__except(EXCEPTION_EXECUTE_HANDLER) so a faulting
//   initializer cannot abort the walk.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004012CE, 68 bytes (the catalog row stopped at 61, before the `call __SEH_epilog ; ret`
//   tail), plain `ret`; table 0x01368E58 .. 0x01368F58. Its twin _RTC_Terminate follows at 0x00401312 (table 0x01369060).
// Why it differed: the CRT objects are size-optimised. Under /O1 VC7.1 builds the SEH frame with the shared helpers
//   __SEH_prolog 0x0040138C / __SEH_epilog 0x004013C7 (push 0Ch ; push scopetable ; call) and keeps the cursor in the
//   frame slot [ebp-1Ch]; under the default /O2 it inlines the fs:[0] frame (120 bytes). The source was right, the flags
//   were not.
typedef void (__cdecl* _PVFV)();

static _PVFV g_table[65];          // test: one contiguous table stands in for .rtc$IAA..IZZ
#define __rtc_iaa g_table
#define __rtc_izz (g_table + 64)

extern "C" void __cdecl _RTC_Initialize()
{
    for (_PVFV* f = __rtc_iaa; f < __rtc_izz; ++f) {
        __try {
            if (*f)
                (**f)();
        }
        __except (1) {
        }
    }
}

// ---- harness ----
static int g_calls;
static void __cdecl First() { g_calls = g_calls * 10 + 1; }
static void __cdecl Second() { g_calls = g_calls * 10 + 2; }
static void __cdecl Faulting() { g_calls = g_calls * 10 + 3; *(int*)0 = 1; g_calls = -1000; }
static void __cdecl Beyond() { g_calls = -1; }

int main()
{
    g_table[0] = &First;
    g_table[7] = &Second;
    g_table[20] = &Faulting;
    g_table[63] = &First;
    g_table[64] = &Beyond;                   // == __rtc_izz: outside the range
    _RTC_Initialize();
    if (g_calls != 1231) { printf("FAIL calls %d\n", g_calls); return 2; }
    printf("RTC_INITIALIZE_TEST PASS\n");
    return 0;
}