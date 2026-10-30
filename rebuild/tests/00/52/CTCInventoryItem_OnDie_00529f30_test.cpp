#include <cstdio>

static int g_freed = 0;
static void* g_lastFreed = 0;
static int g_baseCalled = 0;


struct CTCInventoryItemBase
{
    void* m_vptr;
    void OnDie();
};

class CTCInventoryItem : public CTCInventoryItemBase
{ public:
    void* m_pField4;
    void OnDie();
};

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed++; g_lastFreed = p; }
void CTCInventoryItemBase::OnDie() { g_baseCalled++; }

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    int dummy = 0x1234;

    // Case 1: field is non-null -> free + base OnDie
    CTCInventoryItem a;
    a.m_vptr = 0;
    a.m_pField4 = &dummy;
    a.OnDie();
    bool c1 = (g_freed == 1) && (g_lastFreed == &dummy) && (g_baseCalled == 1);

    // Case 2: field is null -> no free, still base OnDie
    CTCInventoryItem b;
    b.m_vptr = 0;
    b.m_pField4 = 0;
    b.OnDie();
    bool c2 = (g_freed == 1) && (g_baseCalled == 2);

    if (c1 && c2) { printf("ONDIE_OK\n"); return 0; }
    printf("ONDIE_FAIL freed=%d base=%d\n", g_freed, g_baseCalled);
    return 1;
}