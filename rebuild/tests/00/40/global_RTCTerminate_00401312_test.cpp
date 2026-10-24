#include <cstdio>
#ifdef _MSC_VER
extern "C" __declspec(dllimport) void __stdcall RaiseException(unsigned long, unsigned long, unsigned long, const unsigned long*);
#endif

// Model of 0x00401312 (CRT _RTC_Terminate): walk the .rtc$T callback table, skip null slots and call each
// callback under its own __try/__except(EXCEPTION_EXECUTE_HANDLER), so one faulting callback does not stop the rest.
typedef void (*PVFV)(void);
static int g_calls = 0;
static int g_order[8];
static void First() { g_order[g_calls++] = 1; }
static void Second() { g_order[g_calls++] = 2; }
static void Third() { g_order[g_calls++] = 3; }
#ifdef _MSC_VER
static void Faulting() { g_order[g_calls++] = 9; RaiseException(0xE0000001, 0, 0, 0); }   // a callback that throws an SEH exception
#else
static void Faulting() { g_order[g_calls++] = 9; }
#endif

static PVFV g_table[] = { First, 0, Faulting, Second, 0, Third };

static void RTC_Terminate(PVFV* begin, PVFV* end)
{
    PVFV* f;
    for (f = begin; f < end; f++) {
#ifdef _MSC_VER
        __try {
            if (*f)
                (**f)();
        } __except (1) {
        }
#else
        if (*f)
            (**f)();
#endif
    }
}

int main()
{
    RTC_Terminate(g_table, g_table + sizeof(g_table) / sizeof(g_table[0]));
    if (g_calls != 4 || g_order[0] != 1 || g_order[1] != 9 || g_order[2] != 2 || g_order[3] != 3) {
        std::printf("00401312_TEST FAIL\n");
        return 1;
    }
    RTC_Terminate(g_table, g_table);              // empty range: nothing runs
    if (g_calls != 4) {
        std::printf("00401312_TEST FAIL\n");
        return 1;
    }
    std::printf("00401312_TEST PASS\n");
    return 0;
}