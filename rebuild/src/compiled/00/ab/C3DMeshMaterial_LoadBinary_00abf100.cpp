// C3DMeshMaterial::LoadBinary(CMemoryDataInputStream*, CBankFile*) -- read index, name, decal id, the four bank indices, map flags, self illumination and the four flags; when the file carries texture filenames, read them and (with a bank) resolve each to its bank index
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABF100, 1456 bytes, ret 8.
// Callees: stream vtable slot 6 (+0x18) ReadNullTerminatedString (hidden CCharString result), CCharString() 0x0099E4B0,
//   operator= 0x0099EFB0, ~CCharString 0x0099EAE0, ReadWithSrcChunkOverflow 0x00993CA0,
//   CBankFile::FindIndexByFilename 0x009CCDF0. C3DMeshMaterial::CFilenames vtable 0x0129EDB4.
// Shape: inlined CDataInputStream::Read per scalar (landed 0x007AD7F0 pattern). The CFilenames local is a
//   CIVCountedPointeeBase (vptr, IVRefCount = 1) holding four CCharStrings; its generated destructor runs only the
//   member destructors (the base vptr store is dead), and the base vptr store in its constructor is dead too.
// Layout: C3DMeshMaterial per Ego_r.pdb (MaterialIndex +4 .. DegenerateTriangles +0x2b). Local model: no engine header.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharString { public:
    void* m_pStringData;
    CCharString();                                        // 0x0099E4B0
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
class C3DVector { public: float X, Y, Z; };

class CMemoryDataInputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Read(void* buffer, unsigned long size);  // slot 4 (+0x10)
    virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    C3DVector Read3DVector();                             // 0x009950F0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void ReadInline(void* buffer, long size)
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
    __forceinline bool ReadBool() { unsigned char value; ReadInline(&value, sizeof(value)); return value != 0; }
    __forceinline unsigned long ReadULong() { unsigned long value; ReadInline(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; ReadInline(&value, sizeof(value)); return value; }
    __forceinline unsigned short ReadUShort() { unsigned short value; ReadInline(&value, sizeof(value)); return value; }
};


class CBankFile { public:
    unsigned long FindIndexByFilename(const CCharString& name, long flags);   // 0x009CCDF0
};

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() {}
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) {}
};

class C3DMeshMaterial { public:
    class CFilenames : public CIVCountedPointeeBase { public:
        CCharString DiffuseMapName;
        CCharString BumpMapName;
        CCharString ReflectionMapName;
        CCharString IlluminationMapName;
    };
    void* __vftable;
    long MaterialIndex;                    // +0x04
    CCharString Name;                      // +0x08
    long DecalID;                          // +0x0c
    unsigned long DiffuseMapBankIndex;     // +0x10
    unsigned long BumpMapBankIndex;        // +0x14
    unsigned long ReflectionMapBankIndex;  // +0x18
    unsigned long IlluminationMapBankIndex;// +0x1c
    unsigned long MapFlags;                // +0x20
    unsigned long SelfIllumination;        // +0x24
    bool TwoSided;                         // +0x28
    bool Transparent;                      // +0x29
    bool BooleanAlpha;                     // +0x2a
    bool DegenerateTriangles;              // +0x2b
    void LoadBinary(CMemoryDataInputStream* in, CBankFile* bank);
};

void C3DMeshMaterial::LoadBinary(CMemoryDataInputStream* in, CBankFile* bank)
{
    MaterialIndex = in->ReadULong();
    Name = in->ReadNullTerminatedString();
    DecalID = in->ReadULong();
    DiffuseMapBankIndex = in->ReadULong();
    BumpMapBankIndex = in->ReadULong();
    ReflectionMapBankIndex = in->ReadULong();
    IlluminationMapBankIndex = in->ReadULong();
    MapFlags = in->ReadULong();
    SelfIllumination = in->ReadULong();
    TwoSided = in->ReadBool();
    Transparent = in->ReadBool();
    BooleanAlpha = in->ReadBool();
    DegenerateTriangles = in->ReadBool();
    if (in->ReadBool()) {
        CFilenames filenames;
        filenames.DiffuseMapName = in->ReadNullTerminatedString();
        filenames.BumpMapName = in->ReadNullTerminatedString();
        filenames.ReflectionMapName = in->ReadNullTerminatedString();
        filenames.IlluminationMapName = in->ReadNullTerminatedString();
        if (bank) {
            DiffuseMapBankIndex = bank->FindIndexByFilename(filenames.DiffuseMapName, 0);
            BumpMapBankIndex = bank->FindIndexByFilename(filenames.BumpMapName, 2);
            ReflectionMapBankIndex = bank->FindIndexByFilename(filenames.ReflectionMapName, 0);
            IlluminationMapBankIndex = bank->FindIndexByFilename(filenames.IlluminationMapName, 0);
        }
    }
}