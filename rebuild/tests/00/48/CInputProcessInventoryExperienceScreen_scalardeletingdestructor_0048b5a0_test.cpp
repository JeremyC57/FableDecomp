#include <cstdio>
#include <new>

static int g_dtor_calls = 0;
static void* g_last_deleted = 0;

struct CInputProcessInventoryExperienceScreen {
    int a, b, c;
    void* deleting_destructor(unsigned int flags);
};

extern void __fastcall CInputProcessInventoryExperienceScreen_dtor(void* process);
void __fastcall CInputProcessInventoryExperienceScreen_dtor(void* process) { (void)process; ++g_dtor_calls; }

void* CInputProcessInventoryExperienceScreen::deleting_destructor(unsigned int flags)
{
    CInputProcessInventoryExperienceScreen_dtor(this);
    if (flags & 1)
        ::operator delete(this);
    return this;
}

int main()
{
    // flags & 1 clear: destructor runs, no delete, returns this
    CInputProcessInventoryExperienceScreen obj;
    g_dtor_calls = 0;
    void* r = obj.deleting_destructor(0);
    bool ok1 = (r == &obj) && (g_dtor_calls == 1);

    // flags & 1 set: destructor runs then operator delete on heap object
    CInputProcessInventoryExperienceScreen* p = new CInputProcessInventoryExperienceScreen();
    g_dtor_calls = 0;
    void* r2 = p->deleting_destructor(1);
    bool ok2 = (r2 == p) && (g_dtor_calls == 1);

    if (ok1 && ok2) {
        printf("VDD_PARITY_OK\n");
        return 0;
    }
    printf("VDD_FAIL %d %d\n", (int)ok1, (int)ok2);
    return 1;
}