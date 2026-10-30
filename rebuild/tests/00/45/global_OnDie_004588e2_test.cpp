#include "rebuild_abi.h"
#include <cstdio>

class CTCInventoryItem
{ public:
    char pad0[0x54];
    void* field_54;
};

void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr);

static int g_freed = 0;
static int g_based = 0;

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; if (p) g_freed = 1; }
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();
};
CParentDefClassBase::~CParentDefClassBase()
{
    CTCInventoryItem* thisptr = (CTCInventoryItem*)this; (void)thisptr; if (thisptr) g_based = 1;
}

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    CTCInventoryItem a;
    for (int i = 0; i < (int)sizeof(a); ++i) ((char*)&a)[i] = 0;

    // case 1: field_54 non-null -> free called + base called
    int dummy = 42;
    a.field_54 = &dummy;
    g_freed = 0; g_based = 0;
    CTCInventoryItem_OnDie(&a);
    int c1 = (g_freed == 1 && g_based == 1);

    // case 2: field_54 null -> free NOT called, base still called
    a.field_54 = 0;
    g_freed = 0; g_based = 0;
    CTCInventoryItem_OnDie(&a);
    int c2 = (g_freed == 0 && g_based == 1);

    if (c1 && c2)
        printf("OK_004588e2\n");
    else
        printf("FAIL_004588e2 c1=%d c2=%d\n", c1, c2);
    return 0;
}