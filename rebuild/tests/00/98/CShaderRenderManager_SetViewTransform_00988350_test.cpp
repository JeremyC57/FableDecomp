#include <cstdio>
struct SrcMat { unsigned int m[12]; };
class CShaderRenderManager { public:
    char pad[0x230];
    unsigned int dst[12];
    unsigned int f260, f264, f268, f26c;
    char pad2[0x3d8 - 0x270];
    unsigned int flags;
};
void __fastcall CShaderRenderManager_SetViewTransform(CShaderRenderManager* manager, int edxpad, const SrcMat* s)
{
    manager->dst[0] = s->m[0];
    manager->dst[1] = s->m[3];
    manager->dst[2] = s->m[6];
    manager->dst[3] = s->m[9];
    manager->dst[4] = s->m[1];
    manager->dst[5] = s->m[4];
    manager->dst[6] = s->m[7];
    manager->dst[7] = s->m[10];
    manager->dst[8] = s->m[2];
    manager->dst[9] = s->m[5];
    manager->dst[10] = s->m[8];
    manager->dst[11] = s->m[11];
    manager->f260 = 0;
    manager->f264 = 0;
    manager->f268 = 0;
    manager->f26c = 0x3f800000u;
    unsigned int *p = &manager->flags;
    *p |= 0xc990u;
}
int main()
{
    SrcMat s;
    for (int i = 0; i < 12; ++i) s.m[i] = (unsigned int)(i + 1);
    CShaderRenderManager mgr;
    mgr.flags = 0x1;
    CShaderRenderManager_SetViewTransform(&mgr, 0, &s);
    unsigned int expect[12] = {1,4,7,10, 2,5,8,11, 3,6,9,12};
    for (int i = 0; i < 12; ++i) {
        if (mgr.dst[i] != expect[i]) { std::printf("FAIL dst[%d]=%u\n", i, mgr.dst[i]); return 1; }
    }
    if (mgr.f260 || mgr.f264 || mgr.f268) { std::printf("FAIL zeros\n"); return 1; }
    if (mgr.f26c != 0x3f800000u) { std::printf("FAIL f26c\n"); return 1; }
    if (mgr.flags != (0x1u | 0xc990u)) { std::printf("FAIL flags=%x\n", mgr.flags); return 1; }
    std::printf("CShaderRenderManager_00988350_TEST PASS\n");
    return 0;
}