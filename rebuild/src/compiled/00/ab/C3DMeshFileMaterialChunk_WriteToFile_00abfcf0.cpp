// C3DMeshFileMaterialChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const -- name, index, two-sided flag, the three colours, the shininess values and transparency, then the sub-chunks
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABFCF0, 128 bytes. Callees: CDataOutputStream::WriteNullTerminatedString
// 0x00995AE0, WriteLONG 0x00993EB0, WriteBOOL 0x00993E30, WriteColour 0x00993F30, WriteFLOAT 0x00993EE0,
// CChunkedFileSuperChunk::WriteToFile 0x00AA78C0.
// The PDB header (engine/C3DMeshFileMaterialChunk.h) sits 8 bytes high; retail layout with its names as the landed
// GetChunkInfo 0x00ABFBF0: MaterialIndex +0x2C, TwoSided +0x30, Ambient/Diffuse/Specular +0x34/+0x38/+0x3C,
// Shiny/ShinyStrength/Transparency +0x40..+0x48, Name +0x4C. The bool is zero-extended for the call.
class CChunkedFileSaver;

class CCharString { public:
    void* m_pData;                          // CBasicString<char> data block
    CCharString();                          // 0x0099E4B0
};

class CMatrix3x4 { public: float m_row[3][4]; };                  // 48 bytes
class CRGBColour { public: unsigned long m_argb; };                 // as the landed GetChunkInfo 0x00ABFBF0

class CDataOutputStream { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void WriteBytes(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteULONG(unsigned long value);                          // 0x00993EA0
    void WriteUBYTE(unsigned char value);                          // 0x00993E50
    void WriteColour(const CRGBColour& colour);                    // 0x00993F30
    void WriteMatrix3x4(const CMatrix3x4& m);                      // 0x00994070
    void WriteNullTerminatedString(const CCharString& text);       // 0x00995AE0
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteFloat(float);   // 0x00993EE0
    void WriteSLONG(long);   // 0x00993EB0
};

#include "engine/CArray.h"

#include "engine/CCountedPointer.h"


class CChunkedFileSuperChunk;

class CChunkedFileChunk { public:                  // PDB sizeof 0x10
    virtual void v00();                     // slot 0: deleting destructor
    virtual void v01();                     // slot 1: GetChunkDescription
    virtual void v02();                     // slot 2: GetChunkInfo
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();                     // slot 7: ReadFromFile
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // slot 8
    virtual CChunkedFileChunk* CreateNewInstance() const;                                // slot 9
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;          // slot 10 (0x00AA8460 in the super chunk)
    char ChunkType[5];                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk* Parent;         // +0x0c
    CChunkedFileChunk(const char* type);    // 0x00AA7590
};

class CChunkedFileSuperChunk : public CChunkedFileChunk { public:   // retail sizeof 0x2c
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                 // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;   // +0x1c
    unsigned long CurrentChunkIndex;                                       // +0x28
    CChunkedFileSuperChunk(const char* type);                              // 0x00AA8360
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;   // 0x00AA78C0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const; // 0x00AA7F40
};

class C3DMeshFileMaterialChunk : public CChunkedFileSuperChunk { public:
    long          MaterialIndex;     // +0x2c
    bool          TwoSided;          // +0x30
    CRGBColour    Ambient;           // +0x34
    CRGBColour    Diffuse;           // +0x38
    CRGBColour    Specular;          // +0x3c
    float         Shiny;             // +0x40
    float         ShinyStrength;     // +0x44
    float         Transparency;      // +0x48
    CCharString   Name;              // +0x4c
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DMeshFileMaterialChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteNullTerminatedString(Name);
    out.WriteSLONG(MaterialIndex);
    out.WriteEBOOL(TwoSided);
    out.WriteColour(Ambient);
    out.WriteColour(Diffuse);
    out.WriteColour(Specular);
    out.WriteFloat(Shiny);
    out.WriteFloat(ShinyStrength);
    out.WriteFloat(Transparency);
    CChunkedFileSuperChunk::WriteToFile(out, saver);
}