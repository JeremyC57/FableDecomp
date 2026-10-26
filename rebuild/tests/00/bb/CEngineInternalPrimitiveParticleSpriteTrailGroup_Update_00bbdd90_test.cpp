#include <cstdio>

class CEnginePrimitive { public:
    char pad[0x18];
    unsigned long field18;
};

struct CEngineInternalPrimitiveParticleSpriteTrailGroup {
    char pad0[0x0c];
    unsigned long EnginePrimitiveFlags;
    char pad1[0xb8 - 0x10];
    unsigned long fieldb8;
};

void __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_Update(
        CEngineInternalPrimitiveParticleSpriteTrailGroup* group,
        int, const CEnginePrimitive& prim, unsigned long n);

int main() {
    CEngineInternalPrimitiveParticleSpriteTrailGroup obj;
    CEnginePrimitive prim;
    obj.EnginePrimitiveFlags = 0; obj.fieldb8 = 0;
    prim.field18 = 0xDEADBEEF;
    CEngineInternalPrimitiveParticleSpriteTrailGroup_Update(&obj, 0, prim, 0x12345678u);
    if (obj.EnginePrimitiveFlags == 0x12345678u && obj.fieldb8 == 0xDEADBEEF)
        printf("00bbdd90_TEST PASS\n");
    else
        printf("FAIL c=%08lx b8=%08lx\n", obj.EnginePrimitiveFlags, obj.fieldb8);
    return 0;
}