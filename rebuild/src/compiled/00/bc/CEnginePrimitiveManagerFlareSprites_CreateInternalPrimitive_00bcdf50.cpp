// CEnginePrimitiveManagerFlareSprites::CreateInternalPrimitive(long type) -- only for type 0x11: allocate and construct a flaresprite primitive (0x9c bytes: base type 0x11, flags 0xc00000); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCDF50.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (0x11, 0xc00000). Vtable 0x012A6BA4 =
// CEngineInternalPrimitiveFlareSprite (rtti-vtables.tsv). Shape: the type test `cmp [esp+4],0x11 ; je` precedes `push esi` (an early `xor eax,eax ; ret 4`); `this` is unused. The two trailing stores are immediates in body order (+0x94 = -1 first, then +0x90 = 0).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct CEngineInternalPrimitiveFlareSprite : CEngineInternalPrimitiveBase {   // vtable 0x012A6BA4; 0x9c bytes
    unsigned char _pad48[0x48];                 // +0x48 .. +0x90 (not initialised here)
    long          m_90;                            // +0x90
    long          m_94;                            // +0x94 (-1)
    unsigned char _rest98[0x4];            // +0x98 .. +0x9c (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveFlareSprite() : CEngineInternalPrimitiveBase(0x11, 0xc00000) { m_94 = -1; m_90 = 0; }
};

class CEnginePrimitiveManagerFlareSprites { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerFlareSprites::CreateInternalPrimitive(long type)
{
    if (type != 0x11)
        return 0;
    return new CEngineInternalPrimitiveFlareSprite;
}