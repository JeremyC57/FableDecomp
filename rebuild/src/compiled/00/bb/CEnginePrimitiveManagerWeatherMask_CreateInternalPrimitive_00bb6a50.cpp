// CEnginePrimitiveManagerWeatherMask::CreateInternalPrimitive(long type) -- only for type 0x1f: allocate and construct a weathermask primitive (0xa8 bytes: base type 0x1f, flags 0x200000, the words at +0x48..+0x54 cleared); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB6A50.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (0x1f, 0x200000). Vtable 0x012A570C =
// CEngineInternalPrimitiveWeatherMask (rtti-vtables.tsv). Shape: the type test `cmp [esp+4],0x1f ; je` precedes `push esi` (an early `xor eax,eax ; ret 4`); the zero register edi clears the words after the base constructor; `this` is unused.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct SZeroWords_48 {                           // 4 words cleared by its own inline constructor (after the vtable store)
    long m_0;
    long m_1;
    long m_2;
    long m_3;
    SZeroWords_48() : m_0(0), m_1(0), m_2(0), m_3(0) { }
};

class CEngineInternalPrimitiveWeatherMask : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A570C; 0xa8 bytes
    SZeroWords_48 m_words48;               // +0x48 .. +0x58
    unsigned char _rest58[0x50];           // +0x58 .. +0xa8 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveWeatherMask() : CEngineInternalPrimitiveBase(0x1f, 0x200000), m_words48() { }
};

class CEnginePrimitiveManagerWeatherMask { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerWeatherMask::CreateInternalPrimitive(long type)
{
    if (type != 0x1f)
        return 0;
    return new CEngineInternalPrimitiveWeatherMask;
}