// CEnginePrimitiveManagerSprites::CreateInternalPrimitive(long type) -- only for type 7: allocate and construct a sprite primitive (0x88 bytes: base type 7, flags 0x200000); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BACF90.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (7, 0x200000). Vtable 0x012A5444 =
// CEngineInternalPrimitiveSprite (rtti-vtables.tsv). Shape: the type test `cmp [esp+4],7 ; je` precedes `push esi` (an early `xor eax,eax ; ret 4`); `this` is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct CEngineInternalPrimitiveSprite : CEngineInternalPrimitiveBase {   // vtable 0x012A5444; 0x88 bytes
    unsigned char _rest48[0x40];           // +0x48 .. +0x88 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveSprite() : CEngineInternalPrimitiveBase(7, 0x200000) { }
};

struct CEnginePrimitiveManagerSprites {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerSprites::CreateInternalPrimitive(long type)
{
    if (type != 7)
        return 0;
    return new CEngineInternalPrimitiveSprite;
}