#include <cstdio>
#include <cstdlib>

struct CEngineSubPrimitiveSprite {
    int marker;
    void Destroy();
    void* VDD(unsigned int flags);
};

static int g_destroyed = 0;
static int g_freed = 0;

void CEngineSubPrimitiveSprite::Destroy()
{
    g_destroyed = this->marker;
}

void __fastcall FreeMesh(CEngineSubPrimitiveSprite* sprite)
{
    g_freed = sprite->marker;
    free(sprite);
}

void* CEngineSubPrimitiveSprite::VDD(unsigned int flags)
{
    this->Destroy();
    if (flags & 1)
        FreeMesh(this);
    return this;
}

int main()
{
    // case 1: flags bit0 clear -> dtor runs, no free, returns self
    CEngineSubPrimitiveSprite a;
    a.marker = 0x1234;
    g_destroyed = 0; g_freed = 0;
    void* r = a.VDD(0);
    if (r != &a) { printf("FAIL ret\n"); return 1; }
    if (g_destroyed != 0x1234) { printf("FAIL dtor\n"); return 1; }
    if (g_freed != 0) { printf("FAIL nofree\n"); return 1; }

    // case 2: heap alloc, flags bit0 set -> dtor + free
    CEngineSubPrimitiveSprite* b =
        (CEngineSubPrimitiveSprite*)malloc(sizeof(CEngineSubPrimitiveSprite));
    b->marker = 0x55;
    g_destroyed = 0; g_freed = 0;
    void* r2 = b->VDD(1);
    if (r2 != b) { printf("FAIL ret2\n"); return 1; }
    if (g_destroyed != 0x55) { printf("FAIL dtor2\n"); return 1; }
    if (g_freed != 0x55) { printf("FAIL free2\n"); return 1; }

    printf("VDD_OK\n");
    return 0;
}