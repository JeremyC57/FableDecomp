// Leaf: ?Update@CEngineInternalPrimitiveParticleSpriteTrailGroup@@UAEXABVCEnginePrimitive@@K@Z
class CEnginePrimitive { public:
    char pad[0x18];
    unsigned long field18; // +0x18
};

struct CEngineInternalPrimitiveParticleSpriteTrailGroup {
    char pad0[0x8];
    unsigned long EnginePrimitiveFlags;  // +0x0c
    char pad1[0xb8 - 0x10];
    unsigned long fieldb8;  // +0xb8
    virtual void Update(const class CEnginePrimitive&, unsigned long);
};

// this-in-ECX, dummy EDX to force both real args onto the stack ([esp+4],[esp+8]).
void CEngineInternalPrimitiveParticleSpriteTrailGroup::Update(const class CEnginePrimitive& prim, unsigned long n)
{
    CEngineInternalPrimitiveParticleSpriteTrailGroup* group = this;
    group->EnginePrimitiveFlags = n;
    group->fieldb8 = prim.field18;
}
// Flatten route (flatten_pilot v6, NF335): CEngineInternalPrimitiveParticleSpriteTrailGroup_Update -> CEngineInternalPrimitiveParticleSpriteTrailGroup::Update, the RTTI-placed PDB member; class model: the row's local class model.
