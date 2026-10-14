// CEnginePrimitiveManagerLines::CreateInternalPrimitive(long type) -- only for type 6: allocate and construct a line primitive (0x7c bytes: base type 6, flags 0x2000000); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7E50.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (6, 0x2000000). Vtable 0x012A68F4 =
// CEngineInternalPrimitiveLine (rtti-vtables.tsv). Shape: the type test `cmp [esp+4],6 ; je` precedes `push esi` (an early `xor eax,eax ; ret 4`); `this` is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineInternalPrimitiveBase {              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct CEngineInternalPrimitiveLine : CEngineInternalPrimitiveBase {   // vtable 0x012A68F4; 0x7c bytes
    unsigned char _rest48[0x34];           // +0x48 .. +0x7c (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveLine() : CEngineInternalPrimitiveBase(6, 0x2000000) { }
};

struct CEnginePrimitiveManagerLines {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerLines::CreateInternalPrimitive(long type)
{
    if (type != 6)
        return 0;
    return new CEngineInternalPrimitiveLine;
}