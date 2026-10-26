#include <cstdio>

static int g_count = 0;
static void* g_ptrs[8];

class CPersistContext { public:
    void PersistFloat(void* p) { g_ptrs[g_count++] = p; }
    void PersistOther(void* p) { g_ptrs[g_count++] = p; }
    void PersistSub(void* p)   { g_ptrs[g_count++] = p; }
};

class CPhysicsDef { public:
    unsigned char _pad[0x28];
    float Diameter;
    float Friction;
    float Elasticity;
    int   pad34;
    int   pad38;
    int   pad3c;
    unsigned char sub[4];
};

void __fastcall CPhysicsDef_Transfer(CPhysicsDef* def, int edx, CPersistContext* ctx)
{
    ctx->PersistFloat(&def->Diameter);
    ctx->PersistFloat(&def->Friction);
    ctx->PersistFloat(&def->Elasticity);
    ctx->PersistOther(&def->pad3c);
    ctx->PersistFloat((float*)&def->pad34);
    ctx->PersistFloat((float*)&def->pad38);
    ctx->PersistSub(&def->sub[0]);
}

int main() {
    CPhysicsDef d;
    CPersistContext ctx;
    CPhysicsDef_Transfer(&d, 0, &ctx);
    if (g_count != 7) { std::printf("FAIL count %d\n", g_count); return 1; }
    if (g_ptrs[0] != &d.Diameter)   { std::printf("FAIL p0\n"); return 1; }
    if (g_ptrs[3] != &d.pad3c) { std::printf("FAIL p3\n"); return 1; }
    if (g_ptrs[6] != &d.sub[0]){ std::printf("FAIL p6\n"); return 1; }
    std::printf("CPhysicsDef_0045212a_TEST PASS\n");
    return 0;
}