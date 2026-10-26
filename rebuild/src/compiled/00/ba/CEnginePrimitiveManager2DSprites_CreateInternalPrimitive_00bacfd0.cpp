// CEnginePrimitiveManager2DSprites::CreateInternalPrimitive(long type) -- allocates the manager's internal primitive for its two types: a 2D sprite (type 0x22, 0x8C bytes) or a 2D clock sprite (type 0x23, 0x94 bytes); null otherwise or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACFD0, length 107 bytes (112 with padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type,
// unsigned long flags) @ 0x00B8FF00 (with (0x22 / 0x23, 0x80000000)). Vtables (rtti-vtables.tsv): 0x012A54BC =
// CEngineInternalPrimitive2DSprite, 0x012A5534 = CEngineInternalPrimitive2DClockSprite.
// Shape: a two-case `switch` (`sub eax,22h ; je ; dec eax ; jne`) whose arms return a `new` with the inline constructor (base
// call, derived vtable store); unknown types and failed allocations share the `xor eax,eax` tail; `this` is unused
// (`ret 4`). Same family as the landed CEnginePrimitiveManagerSprites factory @ 0x00BACF90.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

class CEngineInternalPrimitive2DSprite : public CEngineInternalPrimitiveBase { public:        // vtable 0x012A54BC; 0x8C bytes
    unsigned char _rest48[0x44];                   // +0x48 .. +0x8C (not initialised here)
    virtual void v00();
    CEngineInternalPrimitive2DSprite() : CEngineInternalPrimitiveBase(0x22, 0x80000000) { }
};

class CEngineInternalPrimitive2DClockSprite : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A5534; 0x94 bytes
    unsigned char _rest48[0x4c];                   // +0x48 .. +0x94 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitive2DClockSprite() : CEngineInternalPrimitiveBase(0x23, 0x80000000) { }
};

class CEnginePrimitiveManager2DSprites { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManager2DSprites::CreateInternalPrimitive(long type)
{
    switch (type) {
    case 0x22:
        return new CEngineInternalPrimitive2DSprite;
    case 0x23:
        return new CEngineInternalPrimitive2DClockSprite;
    }
    return 0;
}