#include <cstdio>

class CTCInventoryItem { public:
    char pad[0x3c];
    void* f3c;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;
static int g_base_called = 0;

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; g_freed++; g_freed_ptr = p; }
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();
};
CParentDefClassBase::~CParentDefClassBase()
{
    CTCInventoryItem* self = (CTCInventoryItem*)this; (void)self; g_base_called++; (void)self;
}

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* self);

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    CTCInventoryItem a;
    a.f3c = (void*)0xdeadbeef;
    CTCInventoryItem_OnDie(&a);
    bool ok1 = (g_freed == 1) && (g_freed_ptr == (void*)0xdeadbeef) && (g_base_called == 1);

    CTCInventoryItem b;
    b.f3c = 0;
    CTCInventoryItem_OnDie(&b);
    bool ok2 = (g_freed == 1) && (g_base_called == 2);

    if (ok1 && ok2)
        printf("004f9cfc_TEST PASS\n");
    else
        printf("FAIL f=%d bp=%p bc=%d\n", g_freed, g_freed_ptr, g_base_called);
    return 0;
}