#include <stdio.h>

class CInputProcessHeroAbilitiesScreen
{ public:
    int tag;
    void scalar_destructor();
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_dtor_this = 0;
static void* g_delete_this = 0;

void CInputProcessHeroAbilitiesScreen::scalar_destructor()
{
    g_dtor_called++;
    g_dtor_this = this;
}

extern void __cdecl fable_operator_delete(void*);
void __cdecl fable_operator_delete(void* p)
{
    g_delete_called++;
    g_delete_this = p;
}

void* __fastcall CInputProcessHeroAbilitiesScreen_scalar_deleting_destructor(
        CInputProcessHeroAbilitiesScreen* process, int edx, unsigned int flags);

int main()
{
    CInputProcessHeroAbilitiesScreen obj;
    obj.tag = 0x1234;

    g_dtor_called = g_delete_called = 0;
    void* r1 = CInputProcessHeroAbilitiesScreen_scalar_deleting_destructor(&obj, 0, 0);
    bool ok1 = (r1 == &obj) && g_dtor_called == 1 && g_delete_called == 0 && g_dtor_this == &obj;

    g_dtor_called = g_delete_called = 0;
    void* r2 = CInputProcessHeroAbilitiesScreen_scalar_deleting_destructor(&obj, 0, 1);
    bool ok2 = (r2 == &obj) && g_dtor_called == 1 && g_delete_called == 1
               && g_dtor_this == &obj && g_delete_this == &obj;

    g_dtor_called = g_delete_called = 0;
    void* r3 = CInputProcessHeroAbilitiesScreen_scalar_deleting_destructor(&obj, 0, 2);
    bool ok3 = (r3 == &obj) && g_dtor_called == 1 && g_delete_called == 0;

    if (ok1 && ok2 && ok3)
        printf("VDD_PARITY_OK\n");
    else
        printf("VDD_FAIL o1=%d o2=%d o3=%d\n", ok1, ok2, ok3);
    return (ok1 && ok2 && ok3) ? 0 : 1;
}