// CCreatureGeneratorDef::Transfer(CPersistContext&) -- persist RandomEffects (vector<long>), RandomSoundCriteria (def string), SecondsBetweenEffects and SecondsBetweenSoundEffects
// Leaf: Transfer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074C5F0, length 216 bytes (+ int3 padding), virtual, `ret 4`.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (x4, the empty key "" @ 0x0122D70E), the vector<long> writer @ 0x00466A75 and
// reader @ 0x00466AA7 (`__fastcall` (stream, vector*); manifest labels "GFSerialiseVectorBinaryOut<float>" /
// "GFSerialiseVectorBinaryIn<CRGBColour>" are propagated), CDefString::TransferBinaryIn(CMemoryDataInputStream*) @ 0x0044FC00,
// CDataOutputStream::WriteSLONG @ 0x00993EB0 (manifest "WriteCBYTE"), CPersistTraits<long>::TransferBinaryIn @ 0x0040FE60
// (manifest "<float>" is propagated).
// Shape: four inline CPersistContext transfers (the landed CActivateQuestDef::Transfer 0x007B5740 template): each member
// address is formed before its CRC call; the vector transfer is the two-way form (mode 2 loads, anything else saves, the
// landed 0x00466A47 body), the scalar ones the `sub eax,2 ; je ; dec eax ; jne` switch with the save arm inline; the long
// load uses the empty traits temporary in the dead argument slot (`lea ecx,[esp+18h]`).
// Layout: Ego_r.pdb RandomEffects +0x28 (a 16-byte PDB vector; retail 12), RandomSoundCriteria +0x38, SecondsBetweenEffects
// +0x3c, SecondsBetweenSoundEffects +0x40; retail +0x28 / +0x34 / +0x38 / +0x3c (no engine header for this class).
class CMemoryDataInputStream;

class CDataOutputStream { public:
public:
    void WriteSLONG(long);   // 0x00993EB0
};

class CDefString { public:                                     // a name stored as its def-string table position
    long TablePos;
    void TransferBinaryIn(CMemoryDataInputStream* stream);   // 0x0044FC00
};

struct CLongVector {                                    // std::vector<long> (retail 12 bytes)
    long* _First;
    long* _Last;
    long* _End;
};
void __fastcall GFSerialiseVectorBinaryOutLong(CDataOutputStream* stream, CLongVector* value);     // 0x00466A75
void __fastcall GFSerialiseVectorBinaryInLong(CMemoryDataInputStream* stream, CLongVector* value); // 0x00466AA7

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<long> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, long* value);   // 0x0040FE60
    static void TransferBinaryOut(CDataOutputStream* stream, const long& value) { stream->WriteSLONG(value); }
};
template<> struct CPersistTraits<CDefString> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, CDefString* value) { value->TransferBinaryIn(stream); }
    static void TransferBinaryOut(CDataOutputStream* stream, const CDefString& value) { stream->WriteSLONG(value.TablePos); }
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
    void Transfer(const char* name, CLongVector& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            GFSerialiseVectorBinaryInLong(m_pLoadStream, &value);
            break;
        default:
            GFSerialiseVectorBinaryOutLong(m_pSaveStream, &value);
            break;
        }
    }
};

class CCreatureGeneratorDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad04[0x28 - 4];
    CLongVector   RandomEffects;                        // +0x28
    CDefString    RandomSoundCriteria;                  // +0x34
    long          SecondsBetweenEffects;                // +0x38
    long          SecondsBetweenSoundEffects;           // +0x3c
};

void CCreatureGeneratorDef::Transfer(CPersistContext& context)
{
    context.Transfer("", RandomEffects);
    context.Transfer("", RandomSoundCriteria);
    context.Transfer("", SecondsBetweenEffects);
    context.Transfer("", SecondsBetweenSoundEffects);
}