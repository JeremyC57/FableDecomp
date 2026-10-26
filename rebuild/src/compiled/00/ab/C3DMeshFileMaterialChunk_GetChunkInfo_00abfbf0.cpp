// C3DMeshFileMaterialChunk::GetChunkInfo() const -- "Name: " + GetName() into a named local, copied to the return slot (VC7.1 has no NRVO)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFBF0, length 79 bytes.
// Callees: 0x0099EC30 CCharString copy ctor x2, 0x0099F690 __fastcall operator+(const char*, const CCharString&), 0x0099EAE0 ~CCharString x2
//
// ---- shared retail model for the C3DMeshFile*Chunk family (TLC Fable.exe) -----------------------
// The PDB header rebuild/include/engine/<Class>.h comes from Ego_r.pdb, a later build in which the
// C3DMeshFileChunk base is 0x34 bytes; in retail TLC it is 0x2c (every derived member sits 8 bytes
// lower, and the vtable slots are the same).  The bytes decide, so this file models the retail layout
// with the PDB member names and says so here instead of including the disagreeing header.
class CCharString { public:
    CCharString();                                      // 0x0099E4B0 default
    CCharString(const char* text, long length);         // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);              // 0x0099EC30 copy
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830

class CMatrix3x4 { public: float m_row[3][4]; };               // 48 bytes, copied with rep movsd
class C3DVector  { public: float X, Y, Z; C3DVector() : X(0.0f), Y(0.0f), Z(0.0f) {} };

class CChunkedFileSaver;
class CDataOutputStream { public:
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteBool(bool value);                         // 0x00993E30
    void WriteMatrix3x4(const CMatrix3x4& m);           // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};

// CChunkedFileChunk (0x10) -> CChunkedFileSuperChunk -> C3DMeshFileChunk (0x2c in retail).
struct C3DMeshFileChunk {
    virtual ~C3DMeshFileChunk();                                 // slot 0 (scalar deleting dtor)
    virtual void v1();
    virtual CCharString GetChunkInfo() const;                    // slot 2
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0
    C3DMeshFileChunk(const char* chunkType);                     // 0x00AA8360
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// The chunk factory that owns CreateNewInstance(): a `new T` registered through virtual slot 10.
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual C3DMeshFileChunk* RegisterNewChunk(C3DMeshFileChunk* chunk);   // slot 10 (+0x28)
};
// ------------------------------------------------------------------------------------------------

class CRGBColour { public: unsigned long m_argb; };
class C3DMeshFileMaterialChunk : public C3DMeshFileChunk { public:
    long          MaterialIndex;     // +0x2c (PDB +0x34)
    bool          TwoSided;          // +0x30
    CRGBColour    Ambient;           // +0x34
    CRGBColour    Diffuse;           // +0x38
    CRGBColour    Specular;          // +0x3c
    float         Shiny;             // +0x40
    float         ShinyStrength;     // +0x44
    float         Transparency;      // +0x48
    CCharString   Name;              // +0x4c (PDB +0x54)   sizeof == 0x50
    C3DMeshFileMaterialChunk();
    void Reset();                    // 0x00AC0250 -- fills the colour/shininess defaults
    CCharString GetName() const { return Name; }
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileMaterialChunk::GetChunkInfo() const
{
    CCharString info = "Name: " + GetName();
    return info;
}