// C3DMeshFileXVertexDataChunk::CreateNewInstance() -- registers `new C3DMeshFileXVertexDataChunk` (inline ctor: CChunkedFileChunk("XVBD"), unit position scale, zero bias, empty vertex array; 0x4c bytes) through virtual slot 10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA59C0, length 105 bytes.
// Callees: 0x00BFEA1A operator new, 0x00AA7590 CChunkedFileChunk ctor, vslot +0x28 RegisterNewChunk.
// Shape: the four format words (+0x10..+0x1c) are left uninitialised; every store follows the vtable store, so
// the compression parameters and the vertex array are member objects with inline constructors
// (CVertexCompressionParams: scale 1.0f x4 then bias 0 x4, in member order; CArray: three null pointers).
// Layout: the PDB header agrees on the offsets but its CArray is 0x10 wide; the retail allocation is 0x4c,
// so this file models the retail layout with the PDB names (see 0x00AA58C0).
// Leaf: CreateNewInstance
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
//
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
    protected:
    CChunkedFileChunk(const char* chunkType);           // 0x00AA7590 (Ego_r / FableWin: protected ??0...@@IAE@PBD@Z)
    public:
    unsigned char _base_body[0x0c];                     // ChunkType[5] + pad + Parent  (sizeof == 0x10)
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
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // slot 7, 0x00AA8830 (CChunkedFileSuperChunk)
    virtual void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;  // slot 8, 0x00AA78C0 (CChunkedFileSuperChunk)
    C3DMeshFileChunk(const char* chunkType);                     // 0x00AA8360
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

class CVertexCompressionParams { public:        // 32 bytes (PDB)
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;
    CVertexCompressionParams()
        : PosScaleX(1.0f), PosScaleY(1.0f), PosScaleZ(1.0f), PosScaleW(1.0f),
          PosBiasX(0.0f), PosBiasY(0.0f), PosBiasZ(0.0f), PosBiasW(0.0f) {}
};

class C3DMeshFileXVertexDataChunk : public CChunkedFileChunk { public:
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    unsigned long            FormatID;       // +0x10
    unsigned long            VertexCount;    // +0x14
    unsigned long            VertexSize;     // +0x18
    unsigned long            InitFlags;      // +0x1c
    CVertexCompressionParams VBCParams;      // +0x20
    CArray<char>             VertexData;     // +0x40   sizeof == 0x4c (PDB 0x50)
    C3DMeshFileXVertexDataChunk() : CChunkedFileChunk("XVBD") {}
};

struct C3DMeshFileXVertexDataChunkFactory : CChunkedFileChunkFactory {
    CChunkedFileChunk* CreateNewInstance();
};
CChunkedFileChunk* C3DMeshFileXVertexDataChunkFactory::CreateNewInstance()
{
    return RegisterNewChunk(new C3DMeshFileXVertexDataChunk);
}