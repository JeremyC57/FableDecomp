#include <cstdio>

class CInputProcessStrafe
{ public:
    void *vfptr;
    void dtor();
};

static int g_dtor_calls = 0;
static int g_delete_calls = 0;
static void *g_deleted = 0;
class CAInputProcess {
public:
    virtual ~CAInputProcess();
};
CAInputProcess::~CAInputProcess()
{
    CInputProcessStrafe * process = (CInputProcessStrafe *)this; (void)process;
    (void)process;
    g_dtor_calls++;
}
void __cdecl operator delete(void* p)
{
    g_delete_calls++;
    g_deleted = p;
}

void * __fastcall CInputProcessStrafe_scalar_deleting_destructor(
    CInputProcessStrafe *process, int edx, unsigned int flags);

int main()
{
    CInputProcessStrafe obj;
    obj.vfptr = 0;

    // flags bit0 clear: dtor called, no delete, returns self
    g_dtor_calls = g_delete_calls = 0; g_deleted = 0;
    void *r1 = CInputProcessStrafe_scalar_deleting_destructor(&obj, 0, 0);
    bool ok1 = (g_dtor_calls == 1) && (g_delete_calls == 0) && (r1 == &obj);

    // flags bit0 set: dtor called, delete called with self, returns self
    g_dtor_calls = g_delete_calls = 0; g_deleted = 0;
    void *r2 = CInputProcessStrafe_scalar_deleting_destructor(&obj, 0, 1);
    bool ok2 = (g_dtor_calls == 1) && (g_delete_calls == 1) && (g_deleted == &obj) && (r2 == &obj);

    // even flag (bit0 clear) -> no delete
    g_dtor_calls = g_delete_calls = 0;
    CInputProcessStrafe_scalar_deleting_destructor(&obj, 0, 2);
    bool ok3 = (g_delete_calls == 0);

    if (ok1 && ok2 && ok3) { printf("VDD_PARITY_OK\n"); return 0; }
    printf("VDD_FAIL %d %d %d\n", (int)ok1, (int)ok2, (int)ok3);
    return 1;
}