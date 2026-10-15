// CEngineLightingManager::CreateInternalPrimitive(long type) -- allocates the lighting manager's internal primitive for its two types: a point light (type 15, 0x70 bytes) or a shadowed spot light (type 16, 0x7C bytes); null otherwise or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44EB0, length 99 bytes (112 with padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type,
// unsigned long flags) @ 0x00B8FF00 (called with (15, 1) / (16, 1)). Vtables (rtti-vtables.tsv): 0x012A212C =
// CEngineInternalPrimitiveLight, 0x012A21A4 = CEngineInternalPrimitiveShadowedSpotLight.
// Shape: two sequential type tests (`cmp eax,0Fh ; jne` then `cmp eax,10h ; jne`) each returning a `new` whose inline
// constructor calls the base constructor and stores the derived vtable; the failed allocations and the unknown type share
// the `xor eax,eax` tail; `this` is unused (`ret 4`). Same family as the landed CEnginePrimitiveManagerSprites factory @ 0x00BACF90.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct CEngineInternalPrimitiveLight : CEngineInternalPrimitiveBase {               // vtable 0x012A212C; 0x70 bytes
    unsigned char _rest48[0x28];                   // +0x48 .. +0x70 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveLight() : CEngineInternalPrimitiveBase(15, 1) { }
};

struct CEngineInternalPrimitiveShadowedSpotLight : CEngineInternalPrimitiveBase {   // vtable 0x012A21A4; 0x7C bytes
    unsigned char _rest48[0x34];                   // +0x48 .. +0x7C (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveShadowedSpotLight() : CEngineInternalPrimitiveBase(16, 1) { }
};

struct CEngineLightingManager {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEngineLightingManager::CreateInternalPrimitive(long type)
{
    if (type == 15)
        return new CEngineInternalPrimitiveLight;
    if (type == 16)
        return new CEngineInternalPrimitiveShadowedSpotLight;
    return 0;
}