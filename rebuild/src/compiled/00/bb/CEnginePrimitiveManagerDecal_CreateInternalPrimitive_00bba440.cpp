// CEnginePrimitiveManagerDecal::CreateInternalPrimitive(long type) -- only for the decal type (0x28): allocate and construct a decal primitive (0x94 bytes: base type 0x28, flags 0x4000); null otherwise or when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBA440, length 64 bytes.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (0x28, 0x4000). Vtable 0x012A5AE4 =
// CEngineInternalPrimitiveDecal (rtti-vtables.tsv). Shape: the type test `cmp [esp+4],28h ; je` comes before `push esi` (its early `xor eax,eax ; ret 4`), the
// inlined decal constructor is the base constructor plus the vtable store; `this` is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x30];                    // +0x04 .. +0x34
    unsigned char PostRenderEnable;                // +0x34 bitfield group: bit 3 = post-render enabled
    unsigned char _pad35[0x13];                    // +0x35 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

class CEngineInternalPrimitiveDecal : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A5AE4; 0x94 bytes
    unsigned char _body[0x4c];                     // +0x48 .. +0x94 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveDecal() : CEngineInternalPrimitiveBase(0x28, 0x4000) { }
};

class CEnginePrimitiveManagerDecal { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerDecal::CreateInternalPrimitive(long type)
{
    if (type != 0x28)
        return 0;
    return new CEngineInternalPrimitiveDecal;
}