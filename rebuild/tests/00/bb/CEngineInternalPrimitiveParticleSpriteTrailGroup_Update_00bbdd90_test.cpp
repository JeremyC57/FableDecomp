#include <cstdio>

class CEnginePrimitive { public:
    char pad[0x18];
    unsigned long field18;
};

struct CEngineInternalPrimitiveParticleSpriteTrailGroupLayout {
    char pad0[0x0c];
    unsigned long EnginePrimitiveFlags;
    char pad1[0xb8 - 0x10];
    unsigned long fieldb8;
};

// NF335 (NF324 Q2): the row now defines the PDB member CEngineInternalPrimitiveParticleSpriteTrailGroup::Update
// (?Update@CEngineInternalPrimitiveParticleSpriteTrailGroup@@UAEXABVCEnginePrimitive@@K@Z).
// The test calls it qualified through a pointer to this declaration-only model, cast from the test's object:
// no body and never constructed, so the test emits no vftable and no copy of the row's symbol.
class CEngineInternalPrimitiveParticleSpriteTrailGroup { public:
    virtual void Update(const CEnginePrimitive&, unsigned long);
};

int main() {
    CEngineInternalPrimitiveParticleSpriteTrailGroupLayout obj;
    CEnginePrimitive prim;
    obj.EnginePrimitiveFlags = 0; obj.fieldb8 = 0;
    prim.field18 = 0xDEADBEEF;
    ((CEngineInternalPrimitiveParticleSpriteTrailGroup*)(&obj))->CEngineInternalPrimitiveParticleSpriteTrailGroup::Update(prim, 0x12345678u);
    if (obj.EnginePrimitiveFlags == 0x12345678u && obj.fieldb8 == 0xDEADBEEF)
        printf("00bbdd90_TEST PASS\n");
    else
        printf("FAIL c=%08lx b8=%08lx\n", obj.EnginePrimitiveFlags, obj.fieldb8);
    return 0;
}