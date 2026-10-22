#include <cstdio>
#include <new>

static int g_dtorCalls = 0;
static int g_freeCalls = 0;

struct CThingCreatureBase {
    int marker;
    void nonvirt_dtor();
    void free_this();
    void* vecDeletingDestructor(unsigned int flags);
};

void CThingCreatureBase::nonvirt_dtor()
{
    g_dtorCalls++;
}

void CThingCreatureBase::free_this()
{
    g_freeCalls++;
}

void* CThingCreatureBase::vecDeletingDestructor(unsigned int flags)
{
    this->nonvirt_dtor();
    if (flags & 1)
        this->free_this();
    return this;
}

int main()
{
    CThingCreatureBase obj;
    obj.marker = 42;

    // Case 1: flags & 1 == 0 -> dtor only, no free, returns this
    void* r1 = obj.vecDeletingDestructor(0);
    bool ok1 = (r1 == &obj) && (g_dtorCalls == 1) && (g_freeCalls == 0) && (obj.marker == 42);

    // Case 2: flags & 1 == 1 -> dtor + free, returns this
    void* r2 = obj.vecDeletingDestructor(1);
    bool ok2 = (r2 == &obj) && (g_dtorCalls == 2) && (g_freeCalls == 1);

    // Case 3: bit set alongside other bits
    void* r3 = obj.vecDeletingDestructor(3);
    bool ok3 = (r3 == &obj) && (g_dtorCalls == 3) && (g_freeCalls == 2);

    // Case 4: even flag (2) -> no free
    void* r4 = obj.vecDeletingDestructor(2);
    bool ok4 = (r4 == &obj) && (g_dtorCalls == 4) && (g_freeCalls == 2);

    if (ok1 && ok2 && ok3 && ok4)
        printf("VECDTOR_OK\n");
    else
        printf("VECDTOR_FAIL d=%d f=%d\n", g_dtorCalls, g_freeCalls);
    return (ok1 && ok2 && ok3 && ok4) ? 0 : 1;
}