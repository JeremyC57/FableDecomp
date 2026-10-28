// CShoveHaPennyRegionDef::Transfer(CPersistContext& context) -- persist the region's Start and Score floats (empty CRC keys), load through the float traits, save through WriteFloat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D0070, length 125 bytes (virtual, `ret 4`).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (x2, key "" @ 0x0122D70E), CPersistTraits<float>::TransferBinaryIn @ 0x0040EFB0,
// CDataOutputStream::WriteFloat @ 0x00993EE0 (manifest "WriteCBYTE" is a propagated name; GOTCHAS wave 25 chunk 150).
// Layout: PDB CShoveHaPennyRegionDef (0x30): Start +0x28, Score +0x2c.
// Shape: the landed CActivateQuestDef::Transfer (0x007B5740) template -- two inline CPersistContext::Transfer<float>,
// the load traits an empty temporary in the dead argument slot, the second save arm ending in its own epilogue.
// Leaf: Transfer
class CMemoryDataInputStream;

class CDataOutputStream { public:
    void WriteFloat(float value);                       // 0x00993EE0
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<float> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, float* value);   // 0x0040EFB0
    static void TransferBinaryOut(CDataOutputStream* stream, const float& value) { stream->WriteFloat(value); }
};

class CPersistContext { public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    protected: void CheckCRC(const char* name); public:                    // 0x00404500
    template<class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

class CShoveHaPennyRegionDef { public:
    virtual void v00();
    unsigned char _pad04[0x24];
    float Start;                                        // +0x28
    float Score;                                        // +0x2c
    void Transfer(CPersistContext& context);
};

void CShoveHaPennyRegionDef::Transfer(CPersistContext& context)
{
    context.Transfer("", Start);
    context.Transfer("", Score);
}