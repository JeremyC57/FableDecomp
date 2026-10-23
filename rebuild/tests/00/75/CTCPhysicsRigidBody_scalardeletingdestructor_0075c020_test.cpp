#include <cstdio>

struct CTCPhysicsRigidBody {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCPhysicsRigidBody* g_last_dtor = 0;

void __fastcall CTCPhysicsRigidBody_dtor(CTCPhysicsRigidBody* body)
{
    g_last_dtor = body;
    body->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCPhysicsRigidBody_scalar_deleting_destructor(CTCPhysicsRigidBody* body, int edx_unused, unsigned int flags)
{
    CTCPhysicsRigidBody_dtor(body);
    if (flags & 1)
        operator delete(body);
    return body;
}

int main()
{
    CTCPhysicsRigidBody a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCPhysicsRigidBody_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCPhysicsRigidBody b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCPhysicsRigidBody_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0075c020_TEST PASS\n");
    return 0;
}