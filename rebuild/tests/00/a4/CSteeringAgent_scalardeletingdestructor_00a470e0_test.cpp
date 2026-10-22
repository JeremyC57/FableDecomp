#include <cstdio>

struct CSteeringAgent {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CSteeringAgent* g_last_dtor = 0;

void __fastcall CSteeringAgent_dtor(CSteeringAgent* agent)
{
    g_last_dtor = agent;
    agent->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CSteeringAgent_scalar_deleting_destructor(CSteeringAgent* agent, int edx_unused, unsigned int flags)
{
    CSteeringAgent_dtor(agent);
    if (flags & 1)
        operator delete(agent);
    return agent;
}

int main()
{
    CSteeringAgent a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CSteeringAgent_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CSteeringAgent b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CSteeringAgent_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}