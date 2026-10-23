#include <cstdio>

// definition of the global the function-under-test writes
void *g_CurrentThingManager_013b8a1c = 0;

void __fastcall SetCurrentThingManager(void *pManager)
{
    g_CurrentThingManager_013b8a1c = pManager;
}

int main()
{
    void *marker = (void *)0xDEADBEEF;

    // invoke with this/arg in ecx via a small __asm shim
    __asm
    {
        mov ecx, marker
        call SetCurrentThingManager
    }

    if (g_CurrentThingManager_013b8a1c != marker)
    {
        printf("FAIL got=%p want=%p\n", g_CurrentThingManager_013b8a1c, marker);
        return 1;
    }

    // second value to prove it truly writes ecx each time
    marker = (void *)0x12345678;
    __asm
    {
        mov ecx, marker
        call SetCurrentThingManager
    }
    if (g_CurrentThingManager_013b8a1c != marker)
    {
        printf("FAIL2 got=%p\n", g_CurrentThingManager_013b8a1c);
        return 1;
    }

    printf("OK_0x004c74f0\n");
    return 0;
}