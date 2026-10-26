#include <cstdio>

class CThingPhysicalSwitch { public:
    void *vfptr;
};

static int g_dtor_calls = 0;
static int g_del_calls = 0;
static void *g_last_deleted = 0;

void __fastcall CThingPhysicalSwitch_dtor(CThingPhysicalSwitch *physicalSwitch) {
    (void)physicalSwitch;
    ++g_dtor_calls;
}
void __fastcall CThingPhysicalSwitch_del(void *p) {
    ++g_del_calls;
    g_last_deleted = p;
}

void * __fastcall CThingPhysicalSwitch_scalar_deleting_destructor(
    CThingPhysicalSwitch *physicalSwitch, int edx, unsigned int flags);

int main() {
    CThingPhysicalSwitch obj;
    obj.vfptr = 0;

    // flags & 1 == 0 : dtor runs, no delete, returns this
    g_dtor_calls = g_del_calls = 0; g_last_deleted = 0;
    void *r0 = CThingPhysicalSwitch_scalar_deleting_destructor(&obj, 0, 0);
    bool ok0 = (r0 == &obj) && (g_dtor_calls == 1) && (g_del_calls == 0);

    // flags & 1 == 1 : dtor runs, delete runs on this, returns this
    g_dtor_calls = g_del_calls = 0; g_last_deleted = 0;
    void *r1 = CThingPhysicalSwitch_scalar_deleting_destructor(&obj, 0, 1);
    bool ok1 = (r1 == &obj) && (g_dtor_calls == 1) && (g_del_calls == 1) && (g_last_deleted == &obj);

    // flags == 3 : low bit set -> delete
    g_dtor_calls = g_del_calls = 0;
    void *r3 = CThingPhysicalSwitch_scalar_deleting_destructor(&obj, 0, 3);
    bool ok3 = (r3 == &obj) && (g_del_calls == 1);

    // flags == 2 : low bit clear -> no delete
    g_dtor_calls = g_del_calls = 0;
    void *r2 = CThingPhysicalSwitch_scalar_deleting_destructor(&obj, 0, 2);
    bool ok2 = (r2 == &obj) && (g_del_calls == 0);

    if (ok0 && ok1 && ok2 && ok3) {
        printf("VDD_PARITY_OK\n");
        return 0;
    }
    printf("VDD_PARITY_FAIL %d %d %d %d\n", ok0, ok1, ok2, ok3);
    return 1;
}