// C3DMeshFileClothChunk::CreateNewInstance() const -- `new C3DMeshFileClothChunk` (inline constructor, 0x44 bytes) handed to the virtual InitNewInstance
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC66E0, 112 bytes. Callees: operator new 0x00BFEA1A, CChunkedFileSuperChunk(const char*)
// 0x00AA8360 ("CLTH" @ 0x0129DF30), CCharString() 0x0099E4B0 (x2), this->InitNewInstance (slot 10, +0x28).
// Vtable: C3DMeshFileClothChunk 0x0129F008. Shared retail model of the landed C3DMeshFile*Chunk family
// (C3DMeshFileConvexVolumeChunk::CreateNewInstance 0x00AAAA60): Name +0x2C, MaterialIndex +0x30 (left
// uninitialised), ParticleCode +0x34, CompiledCode CArray<char> +0x38 (zeroed with immediates). The null
// `new` result path duplicates the virtual call tail.
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
    virtual CChunkedFileSuperChunk* CreateNewInstance() const;                                 // slot 9
    virtual CChunkedFileSuperChunk* InitNewInstance(CChunkedFileSuperChunk* chunk) const;            // slot 10 (+0x28)
    protected:
    CChunkedFileSuperChunk(const char* chunkType);                     // 0x00AA8360 (Ego_r / FableWin protected IAE)
    public:
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// ------------------------------------------------------------------------------------------------

#include "engine/CArray.h"

class C3DMeshFileClothChunk : public CChunkedFileSuperChunk { public:
    CCharString  Name;           // +0x2c
    long         MaterialIndex;  // +0x30
    CCharString  ParticleCode;   // +0x34
    CArray<char> CompiledCode;   // +0x38 .. +0x43   sizeof == 0x44
    C3DMeshFileClothChunk() : CChunkedFileSuperChunk("CLTH") {}
    virtual CChunkedFileSuperChunk* CreateNewInstance() const;
};

CChunkedFileSuperChunk* C3DMeshFileClothChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DMeshFileClothChunk);
}