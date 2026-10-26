class CEnginePrimitive { public:
    char pad[0x18];
    unsigned long field18; // +0x18
};

struct CEngineInternalPrimitiveParticleSpriteTrailGroup {
    char pad0[0x0c];
    unsigned long EnginePrimitiveFlags;  // +0x0c
    char pad1[0xb8 - 0x10];
    unsigned long fieldb8;  // +0xb8
};

// this-in-ECX, dummy EDX to force both real args onto the stack ([esp+4],[esp+8]).
void __fastcall CEngineInternalPrimitiveParticleSpriteTrailGroup_Update(
        CEngineInternalPrimitiveParticleSpriteTrailGroup* group,
        int /*edx*/,
        const CEnginePrimitive& prim,
        unsigned long n)
{
    group->EnginePrimitiveFlags = n;
    group->fieldb8 = prim.field18;
}