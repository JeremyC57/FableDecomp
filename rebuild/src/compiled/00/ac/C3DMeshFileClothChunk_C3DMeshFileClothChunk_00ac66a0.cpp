// C3DMeshFileClothChunk::C3DMeshFileClothChunk() -- base chunk "CLTH", Name and ParticleCode strings, empty CompiledCode CArray; MaterialIndex untouched
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC66A0, length 64 bytes.
// Callees: 0x00AA8360 CChunkedFileSuperChunk ctor, 0x0099E4B0 CCharString::CCharString() x2
//
// ---- shared retail model for the C3DMeshFile*Chunk family (TLC Fable.exe) -----------------------
// The PDB header rebuild/include/engine/<Class>.h comes from Ego_r.pdb, a later build in which the
// CChunkedFileSuperChunk base is 0x34 bytes; in retail TLC it is 0x2c (every derived member sits 8 bytes
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

// CChunkedFileChunk (0x10) -> CChunkedFileSuperChunk -> CChunkedFileSuperChunk (0x2c in retail).
class CChunkedFileSuperChunk { public:      // RTTI direct base of the mesh/anim chunks (was the invented CChunkedFileSuperChunk)
    virtual ~CChunkedFileSuperChunk();                                 // slot 0 (scalar deleting dtor)
    virtual void v1();
    virtual CCharString GetChunkInfo() const;                    // slot 2
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0
    protected:
    CChunkedFileSuperChunk(const char* chunkType);                     // 0x00AA8360 (Ego_r / FableWin protected IAE)
    public:
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// The chunk factory that owns CreateNewInstance(): a `new T` registered through virtual slot 10.
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual CChunkedFileSuperChunk* RegisterNewChunk(CChunkedFileSuperChunk* chunk);   // slot 10 (+0x28)
};
// ------------------------------------------------------------------------------------------------

#include "engine/CArray.h"

class C3DMeshFileClothChunk : public CChunkedFileSuperChunk { public:
    CCharString  Name;            // +0x2c (PDB +0x34)
    long         MaterialIndex;   // +0x30 (PDB +0x38) not initialised here
    CCharString  ParticleCode;    // +0x34 (PDB +0x3c)
    CArray<char> CompiledCode;    // +0x38 (PDB +0x40)
    C3DMeshFileClothChunk();
};
C3DMeshFileClothChunk::C3DMeshFileClothChunk()
    : CChunkedFileSuperChunk("CLTH")
{
}