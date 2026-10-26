#include <cstdio>

struct CMember { int x; };

class CPersistContext { public:
    int calls;
    void* args[4];
    void Persist(CMember* m) { if (calls < 4) args[calls] = m; calls++; }
};

class CGuildMasterDef { public:
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Persist(&def->m28);
    ctx->Persist(&def->m2c);
}

int main()
{
    CGuildMasterDef d;
    CPersistContext ctx; ctx.calls = 0;
    CGuildMasterDef_Transfer(&d, 0, &ctx);
    unsigned char* base = (unsigned char*)&d;
    if (ctx.calls == 2 && ctx.args[0] == (void*)(base + 0x28) && ctx.args[1] == (void*)(base + 0x2c)) {
        std::printf("CGuildMasterDef_004de545_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL calls=%d\n", ctx.calls);
    return 1;
}