#include <cstdio>

class CEngineSubPrimitiveBase
{ public:
    int marker;
    void Destroy();
    void FreeSelf();
    void* VecDelDtor_CEngineSubPrimitiveGameMesh(unsigned int flags);
};

static int g_destroyed = 0;
static int g_freed = 0;

void CEngineSubPrimitiveBase::Destroy() { g_destroyed++; }
void CEngineSubPrimitiveBase::FreeSelf() { g_freed++; }

void* CEngineSubPrimitiveBase::VecDelDtor_CEngineSubPrimitiveGameMesh(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        this->FreeSelf();
    return this;
}

int main()
{
    CEngineSubPrimitiveBase a;
    a.marker = 0x1234;

    // flags bit0 clear: destroy only, no free, returns this
    g_destroyed = g_freed = 0;
    void* r0 = a.VecDelDtor_CEngineSubPrimitiveGameMesh(0);
    bool ok0 = (r0 == &a) && (g_destroyed == 1) && (g_freed == 0);

    // flags bit0 set: destroy + free, returns this
    g_destroyed = g_freed = 0;
    void* r1 = a.VecDelDtor_CEngineSubPrimitiveGameMesh(1);
    bool ok1 = (r1 == &a) && (g_destroyed == 1) && (g_freed == 1);

    // higher bits set but bit0 clear: no free
    g_destroyed = g_freed = 0;
    a.VecDelDtor_CEngineSubPrimitiveGameMesh(2);
    bool ok2 = (g_destroyed == 1) && (g_freed == 0);

    if (ok0 && ok1 && ok2)
        printf("VECDELDTOR_OK\n");
    else
        printf("VECDELDTOR_FAIL d=%d f=%d\n", g_destroyed, g_freed);
    return (ok0 && ok1 && ok2) ? 0 : 1;
}