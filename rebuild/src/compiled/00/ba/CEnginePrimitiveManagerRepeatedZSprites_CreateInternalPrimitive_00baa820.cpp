// CEnginePrimitiveManagerRepeatedZSprites::CreateInternalPrimitive(long type) -- allocate and construct a repeatedzsprite primitive (0x94 bytes: base type 0x2b, flags 0x10, the words at +0x48..+0x70 cleared); null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAA820.
// Callees: operator new @ 0x00BFEA1A (non-throwing), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase @ 0x00B8FF00 (0x2b, 0x10). Vtable 0x012A524C =
// CEngineInternalPrimitiveRepeatedZSprite (rtti-vtables.tsv). Shape: no type test; the zero register edi clears the words after the base constructor; `this` is unused. The two words at +0x48/+0x4c are stored before the vtable, the four at +0x64..+0x70 after (VC7.1 schedules the vtable among the init-list stores).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

class CEngineInternalPrimitiveBase { public:              // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes): the base constructor takes (type, flags)
    virtual void v00();
    unsigned char _pad04[0x44];                    // +0x04 .. +0x48
    CEngineInternalPrimitiveBase(long type, unsigned long flags);   // 0x00B8FF00
};

struct SZeroWords_64 {                           // 4 words cleared by its own inline constructor (after the vtable store)
    long m_0;
    long m_1;
    long m_2;
    long m_3;
    SZeroWords_64() : m_0(0), m_1(0), m_2(0), m_3(0) { }
};

class CEngineInternalPrimitiveRepeatedZSprite : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A524C; 0x94 bytes
    long m_48;                             // +0x48
    long m_4c;                             // +0x4c
    unsigned char _pad50[0x14];            // +0x50 .. +0x64 (not initialised here)
    SZeroWords_64 m_words64;               // +0x64 .. +0x74
    unsigned char _rest74[0x20];           // +0x74 .. +0x94 (not initialised here)
    virtual void v00();
    CEngineInternalPrimitiveRepeatedZSprite() : CEngineInternalPrimitiveBase(0x2b, 0x10), m_48(0), m_4c(0), m_words64() { }
};

class CEnginePrimitiveManagerRepeatedZSprites { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);
};

CEngineInternalPrimitiveBase* CEnginePrimitiveManagerRepeatedZSprites::CreateInternalPrimitive(long type)
{
    return new CEngineInternalPrimitiveRepeatedZSprite;
}