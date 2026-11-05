// C3DMeshFileTransformationChunk::CreateNewInstance() -- registers `new C3DMeshFileTransformationChunk` (inline default ctor: base chunk "TRFM" + identity Transform, 0x5c bytes) through virtual slot 10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AB9CE0, length 108 bytes.
// Callees: 0x00BFEA1A operator new, 0x00AA8360 CChunkedFileSuperChunk ctor, vslot +0x28 RegisterNewChunk.
// Shape: the default constructor (landed 0x00AB9BE0) is inlined into the factory: the vtable store
// precedes the twelve identity stores of the embedded CMatrix3x4 (its own inline constructor), nine
// zeros from edi in descending order then the three 1.0f. The null arm of `new` passes 0 to the
// registrar (tail-duplicated call). Same factory shape as the landed 0x00AA4B00.
// Leaf: CreateNewInstance
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
//
// The PDB header rebuild/include/engine/<Class>.h comes from Ego_r.pdb, a later build in which the
// CChunkedFileSuperChunk base is 0x34 bytes and CArray<T> is 0x10 bytes; in retail TLC the base is 0x2c
// (every derived member sits 8 bytes lower, the vtable slots are the same) and CArray<T> is three
// pointers (0x0c: the XUVTangent factory allocates 0x28 bytes for two CArrays behind a 0x10 base).
// The bytes decide, so this file models the retail layout with the PDB member names and says so here
// instead of including the disagreeing header.
class CCharString { public:
    CCharString();                                      // 0x0099E4B0 default
    CCharString(const char* text, long length);         // 0x0099EBF0 (-1 = strlen)
    CCharString(const CCharString& other);              // 0x0099EC30 copy
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

#include "engine/C3DVector.h"
class CMatrix3x4 { public:                                     // 48 bytes: three axis vectors and the position, copied with rep movsd
    C3DVector XAxis, YAxis, ZAxis, Position;
    CMatrix3x4() { SetIdentity(); }
    void SetIdentity()                                  // chained assignments: the stores run right to left
    {
        XAxis.Y = XAxis.Z = YAxis.X = YAxis.Z = ZAxis.X = ZAxis.Y = Position.X = Position.Y = Position.Z = 0.0f;
        XAxis.X = YAxis.Y = ZAxis.Z = 1.0f;
    }
};

#include "engine/CArray.h"

class CChunkedFileSaver;
class CChunkedFileLoader;
class CDataOutputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
    void WriteUShort(unsigned short value);             // 0x00993E80
    void WriteULong(unsigned long value);               // 0x00993EA0
    void WriteLong(long value);                         // 0x00993EB0
    void WriteFloat(float value);                       // 0x00993EE0
    void WriteBool(bool value);                         // 0x00993E30
    void WriteMatrix3x4(const CMatrix3x4& m);           // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
};
class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                // slot 2 (+0x08)
    CMatrix3x4 ReadMatrix3x4();                         // 0x00995340 (by value through the hidden pointer)
};

// CChunkedFileChunk (0x10: vptr, ChunkType[5], Parent) is the base of the X* chunks.
class CChunkedFileChunk { public:
    virtual void v0();
    CChunkedFileChunk(const char* chunkType);           // 0x00AA7590
    unsigned char _base_body[0x0c];                     // ChunkType[5] + pad + Parent  (sizeof == 0x10)
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
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // slot 7, 0x00AA8830 (CChunkedFileSuperChunk)
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0 (CChunkedFileSuperChunk)
    protected:
    CChunkedFileSuperChunk(const char* chunkType);                     // 0x00AA8360 (Ego_r / FableWin protected IAE)
    public:
    unsigned char _base_body[0x28];                              // +0x04 .. +0x2b (chunk type, parent, child list)
};

// The chunk factory that owns CreateNewInstance(): a `new T` registered through virtual slot 10.
// (rtti-vtables.tsv lists CreateNewInstance at slot 9 of the chunk's own vtable: the prototype chunk
// creates its sibling and registers it through slot 10; both models give the same bytes.)
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual CChunkedFileChunk* RegisterNewChunk(CChunkedFileChunk* chunk);   // slot 10 (+0x28)
};
// ------------------------------------------------------------------------------------------------

class C3DMeshFileTransformationChunk : public CChunkedFileSuperChunk { public:
    CMatrix3x4 Transform;          // +0x2c (PDB +0x34)   sizeof == 0x5c
    C3DMeshFileTransformationChunk() : CChunkedFileSuperChunk("TRFM") {}
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

struct C3DMeshFileTransformationChunkFactory : CChunkedFileChunkFactory {
    CChunkedFileChunk* CreateNewInstance();
};
CChunkedFileChunk* C3DMeshFileTransformationChunkFactory::CreateNewInstance()
{
    return RegisterNewChunk((CChunkedFileChunk*)(void*)new C3DMeshFileTransformationChunk);
}