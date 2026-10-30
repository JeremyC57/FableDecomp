#include "rebuild_abi.h"
#include <cstdio>

class CTCInventoryItem
{ public:
    char pad[0x3c];
    void* m_ptr;
};

extern void __fastcall CTCInventoryItem_OnDie(CTCInventoryItem* thisptr);

static int g_freed = 0;
static int g_based = 0;

static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* p)
{
    if (!g_free_hook) return; (void)p; g_freed = 1; }
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();
};
CParentDefClassBase::~CParentDefClassBase()
{
    CTCInventoryItem* t = (CTCInventoryItem*)this; (void)t; (void)t; g_based = 1;
}

int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    CTCInventoryItem a;
    for (int i = 0; i < (int)sizeof(a); ++i) ((char*)&a)[i] = 0;
    a.m_ptr = (void*)0x1234;
    CTCInventoryItem_OnDie(&a);
    if (g_freed == 1 && g_based == 1)
        printf("OK_0045855a_FREED_BASED\n");
    return 0;
}