#include <cstdio>

class CInputProcessInventoryMapScreen
{ public:
    int marker;
    void Destroy();
    void* ScalarDeletingDestructor(unsigned int flags);
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;

void CInputProcessInventoryMapScreen::Destroy()
{
    g_dtor_calls++;
    this->marker = 0xDEAD;
}

void operator_delete_impl(void* p)
{
    g_delete_calls++;
    (void)p;
}

void* CInputProcessInventoryMapScreen::ScalarDeletingDestructor(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        operator_delete_impl(this);
    return this;
}

int main()
{
    CInputProcessInventoryMapScreen a;
    a.marker = 0;
    // flags without bit 0: dtor only, no delete
    void* r1 = a.ScalarDeletingDestructor(0);
    bool ok1 = (r1 == &a) && (g_dtor_calls == 1) && (g_delete_calls == 0) && (a.marker == (int)0xDEAD);

    // flags with bit 0: dtor + delete
    void* r2 = a.ScalarDeletingDestructor(1);
    bool ok2 = (r2 == &a) && (g_dtor_calls == 2) && (g_delete_calls == 1);

    // flags with other bits set but not bit0: no delete
    void* r3 = a.ScalarDeletingDestructor(2);
    bool ok3 = (r3 == &a) && (g_dtor_calls == 3) && (g_delete_calls == 1);

    if (ok1 && ok2 && ok3)
        printf("VDTOR_OK\n");
    else
        printf("VDTOR_FAIL d=%d del=%d\n", g_dtor_calls, g_delete_calls);
    return (ok1 && ok2 && ok3) ? 0 : 1;
}