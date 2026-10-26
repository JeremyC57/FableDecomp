// CEnginePrimitiveManagerSpriteTrails::CreateInternalPrimitive(long type) -- only for type 0x14: allocate and construct a spritetrail primitive (0xb4 bytes: base type 0x14, flags 0x200000, the words at +0x68..+0x74 cleared); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BCA8B0.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (0x14, 0x200000). Vtable 0x012A6A4C =
// CEngineInternalPrimitiveSpriteTrail (rtti-vtables.tsv). Shape: the type test `cmp [esp+4],0x14 ; je` precedes `push esi` (an early `xor eax,eax ; ret 4`); the zero register edi clears the words after the base constructor; `this` is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct SZeroWords_68 {                           // 4 words cleared by its own inline constructor (after the vtable store)
    long m_0;
    long m_1;
    long m_2;
    long m_3;
    SZeroWords_68() : m_0(0), m_1(0), m_2(0), m_3(0) { }
};

class CEngineInternalPrimitiveSpriteTrail : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A6A4C; 0xb4 bytes
    unsigned char _pad48[0x20];            // +0x48 .. +0x68 (not initialised here)
    SZeroWords_68 m_words68;               // +0x68 .. +0x78
    unsigned char _rest78[0x3c];           // +0x78 .. +0xb4 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveSpriteTrail() : CEngineInternalPrimitiveBase(0x14, 0x200000), m_words68() { }
};

class CEnginePrimitiveManagerSpriteTrails { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerSpriteTrails::CreateInternalPrimitive(long type)
{
    if (type != 0x14)
        return 0;
    return new CEngineInternalPrimitiveSpriteTrail;
}