// C3DGeneratorObject::LoadBinary(CMemoryDataInputStream*) -- read the generator's matrix, id, name, bank index and local-origin flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2E5B0, length 336 bytes.
// Callees: CDataInputStream::ReadMatrix3x4 @ 0x00995340 (48-byte result by value through the hidden pointer,
//          copied with rep movsd), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined Read's
//          slow path), stream virtual slot 6 (+0x18) ReadNullTerminatedString (CCharString by value),
//          CCharString::operator=(const CCharString&) @ 0x0099EFB0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the matrix temporary is the 0x30-byte frame; the two ulongs and the bool go through inlined
//          by-value stream reads whose locals (and the string temporary) live in the dead stream-argument
//          slot; the bool is read as a byte and normalised (`test al,al ; setne al`).
// Layout: PDB (struct_layouts_egor.tsv) C3DGeneratorObject is 0x40 bytes: 0x34 unnamed bytes, ObjectName +0x34,
//          BankIndex +0x38, UseLocalOrigin +0x3C. The unnamed block is a 3x4 matrix (+0x00, 48 bytes, rep movsd 12)
//          and a ulong read from the stream (+0x30). No engine header exists for the class.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString { public:
    char* m_pString;
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other); // 0x0099EFB0
};

class CMatrix3x4 { public: float M[12]; };                    // 48 bytes: three axis vectors and the position

class CMemoryDataInputStream { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual CCharString ReadNullTerminatedString();    // slot 6 (+0x18), by value
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14
    CMatrix3x4 ReadMatrix3x4();                        // 0x00995340
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
};

class C3DGeneratorObject { public:
    CMatrix3x4    Matrix;                  // +0x00
    unsigned long GeneratorID;             // +0x30
    CCharString   ObjectName;              // +0x34
    unsigned long BankIndex;               // +0x38
    bool          UseLocalOrigin;          // +0x3C
    void LoadBinary(CMemoryDataInputStream* in);
};

void C3DGeneratorObject::LoadBinary(CMemoryDataInputStream* in)
{
    Matrix = in->ReadMatrix3x4();
    GeneratorID = in->ReadULong();
    ObjectName = in->ReadNullTerminatedString();
    BankIndex = in->ReadULong();
    UseLocalOrigin = in->ReadBool();
}