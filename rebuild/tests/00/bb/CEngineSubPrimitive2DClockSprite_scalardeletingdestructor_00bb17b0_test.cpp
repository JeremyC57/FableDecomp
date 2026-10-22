#include <stdio.h>

struct CEngineSubPrimitive2DClockSprite {
    int marker;
    void destroy();
    void freeMem();
    void* vec_deleting_destructor(unsigned int flags);
};

static int g_destroyed = 0;
static int g_freed = 0;

void CEngineSubPrimitive2DClockSprite::destroy() { g_destroyed++; }
void CEngineSubPrimitive2DClockSprite::freeMem() { g_freed++; }

void* CEngineSubPrimitive2DClockSprite::vec_deleting_destructor(unsigned int flags)
{
    this->destroy();
    if (flags & 1)
        this->freeMem();
    return this;
}

int main()
{
    CEngineSubPrimitive2DClockSprite a; a.marker = 7;
    // flags=0: destroy only, no free, returns this
    g_destroyed = g_freed = 0;
    void* r0 = a.vec_deleting_destructor(0);
    if (r0 != &a || g_destroyed != 1 || g_freed != 0) { printf("FAIL0\n"); return 1; }

    // flags=1: destroy + free, returns this
    g_destroyed = g_freed = 0;
    void* r1 = a.vec_deleting_destructor(1);
    if (r1 != &a || g_destroyed != 1 || g_freed != 1) { printf("FAIL1\n"); return 1; }

    // even flag (2): low bit clear -> no free
    g_destroyed = g_freed = 0;
    void* r2 = a.vec_deleting_destructor(2);
    if (r2 != &a || g_destroyed != 1 || g_freed != 0) { printf("FAIL2\n"); return 1; }

    printf("VDD_OK\n");
    return 0;
}