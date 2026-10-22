// CParticleProgram::WriteToFile(CDataOutputStream& stream) -- save a particle program: version 333, the raw constraint byte stream, the initial simulation, the render triangle/quad/vertex arrays, the non-simulated positions, the texture coordinates and index arrays, the patch size and bezier flag, the exported particle names, then the simulated and non-simulated groups
// Leaf: WriteToFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE2230, length 736 bytes (`ret 4`).
// Callees: CDataOutputStream::WriteULong @ 0x00993EA0, WriteFloat @ 0x00993EE0, WriteEBOOL @ 0x00993E30,
//   WriteNullTerminatedString @ 0x00995AE0, the stream's virtual Write(const void*, long) (slot 4),
//   CMemoryDataOutputStream::GetSize @ 0x00994B40 (label "GetFramesRemaining" propagated) and PeekData @ 0x00995E70,
//   CParticleSimulation::WriteToFile @ 0x00AE2050, the C4DVector array writer @ 0x00AE18C0 (__fastcall),
//   WriteGroups @ 0x00AE0680 (__fastcall stream, CArray<C3DGroup2>*).
// Layout: Ego_r.pdb CParticleProgram with retail 12-byte CArrays (PDB 16; engine/CParticleProgram.h is the PDB view,
//   so the class is modelled locally): InitialSimulation +0x08 (0x80), ExportParticles +0x88 (vector of
//   {CCharString, ulong}), RenderTris +0x98 (6-byte C3DTriangle2), RenderQuads +0xa4 (8-byte), RenderVertices +0xb0
//   (4-byte), NonSimCount +0xbc, NonSimPositions +0xc0, IndexedTextureCoords +0xc4, TempVertexPositions +0xd0 (not
//   saved), ParticleIndices +0xdc, VertexIndices +0xe8, BezierEnable +0xf4, AveragePatchSize +0xf8, SimGroups +0xfc,
//   NonSimGroups +0x108, Constraints +0x114 (CMemoryDataOutputStream).
// Shape: each CArray goes out as its count and then one raw Write of `size() > 0 ? first : (T*)this` bytes (the inline
//   CArray data accessor). The byte count is read through a may-alias view of the array (`const CArray<T>* const&`)
//   written in the function body itself: retail re-reads first/last after the select's join instead of reusing the
//   select's size. The same view inside a __forceinline template writer is seen through (CSE'd size, 654 bytes).
struct CCharStringData;
struct CCharString { CCharStringData* m_pData; };
struct C4DVector { float X, Y, Z, W; };
struct C2DVector { float X, Y; };
struct C3DTriangle2 { unsigned short Index[3]; };
struct C3DQuad2 { unsigned short Index[4]; };
struct C3DClothRenderVertex { unsigned short Particle; unsigned short TexCoord; };
struct C3DGroup2;

class CDataOutputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);                 // slot 4 (+0x10)
    void WriteEBOOL(bool value);                                       // 0x00993E30
    void WriteULong(unsigned long value);                              // 0x00993EA0
    void WriteFloat(float value);                                      // 0x00993EE0
    void WriteNullTerminatedString(const CCharString& text);           // 0x00995AE0
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned char _body04[0x18];
    long GetSize();                                                    // 0x00994B40
    void* PeekData();                                                  // 0x00995E70
};

template <class T> struct CArray {                                     // retail 12 bytes
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    const T* GetData() const { return size() > 0 ? m_first : (const T*)this; }
};

struct CParticleSimulation {
    unsigned char _body[0x80];
    void WriteToFile(CDataOutputStream& stream);                       // 0x00AE2050
};
struct CExportParticle {                                               // CVectorMap<CCharString, unsigned long> entry
    CCharString   first;
    unsigned long second;
};
void __fastcall WriteVectors_00ae18c0(const C4DVector* vectors, unsigned long count, CDataOutputStream& stream);   // 0x00AE18C0
void __fastcall WriteGroups(CDataOutputStream& stream, const CArray<C3DGroup2>* groups);                            // 0x00AE0680

class CParticleProgram {
public:
    void*                         __vftable;                           // +0x00
    void*                         m_pLink;                             // +0x04
    CParticleSimulation           InitialSimulation;                   // +0x08
    CArray<CExportParticle>       ExportParticles;                     // +0x88
    unsigned long                 ExportParticlesSorted;               // +0x94
    CArray<C3DTriangle2>          RenderTris;                          // +0x98
    CArray<C3DQuad2>              RenderQuads;                         // +0xa4
    CArray<C3DClothRenderVertex>  RenderVertices;                      // +0xb0
    unsigned long                 NonSimCount;                         // +0xbc
    C4DVector*                    NonSimPositions;                     // +0xc0
    CArray<C2DVector>             IndexedTextureCoords;                // +0xc4
    CArray<C4DVector>             TempVertexPositions;                 // +0xd0
    CArray<long>                  ParticleIndices;                     // +0xdc
    CArray<long>                  VertexIndices;                       // +0xe8
    bool                          BezierEnable;                        // +0xf4
    float                         AveragePatchSize;                    // +0xf8
    CArray<C3DGroup2>             SimGroups;                           // +0xfc
    CArray<C3DGroup2>             NonSimGroups;                        // +0x108
    CMemoryDataOutputStream       Constraints;                         // +0x114
    void WriteToFile(CDataOutputStream& stream);
};

void CParticleProgram::WriteToFile(CDataOutputStream& stream)
{
    stream.WriteULong(0x14d);
    stream.WriteULong(Constraints.GetSize());
    stream.Write(Constraints.PeekData(), Constraints.GetSize());
    InitialSimulation.WriteToFile(stream);
    stream.WriteULong(RenderTris.size());
    {
        const void* data = RenderTris.GetData();
        const CArray<C3DTriangle2>* const& array = &RenderTris;
        stream.Write(data, array->size() * sizeof(C3DTriangle2));
    }
    stream.WriteULong(RenderQuads.size());
    {
        const void* data = RenderQuads.GetData();
        const CArray<C3DQuad2>* const& array = &RenderQuads;
        stream.Write(data, array->size() * sizeof(C3DQuad2));
    }
    stream.WriteULong(RenderVertices.size());
    {
        const void* data = RenderVertices.GetData();
        const CArray<C3DClothRenderVertex>* const& array = &RenderVertices;
        stream.Write(data, array->size() * sizeof(C3DClothRenderVertex));
    }
    stream.WriteULong(NonSimCount);
    WriteVectors_00ae18c0(NonSimPositions, NonSimCount, stream);
    stream.WriteULong(IndexedTextureCoords.size());
    {
        const void* data = IndexedTextureCoords.GetData();
        const CArray<C2DVector>* const& array = &IndexedTextureCoords;
        stream.Write(data, array->size() * sizeof(C2DVector));
    }
    stream.WriteULong(ParticleIndices.size());
    {
        const void* data = ParticleIndices.GetData();
        const CArray<long>* const& array = &ParticleIndices;
        stream.Write(data, array->size() * sizeof(long));
    }
    stream.WriteULong(VertexIndices.size());
    {
        const void* data = VertexIndices.GetData();
        const CArray<long>* const& array = &VertexIndices;
        stream.Write(data, array->size() * sizeof(long));
    }
    stream.WriteFloat(AveragePatchSize);
    stream.WriteEBOOL(BezierEnable);
    stream.WriteULong(ExportParticles.size());
    for (unsigned long i = 0; i < ExportParticles.size(); ++i) {
        stream.WriteNullTerminatedString(ExportParticles.m_first[i].first);
        stream.WriteULong(ExportParticles.m_first[i].second);
    }
    WriteGroups(stream, &SimGroups);
    WriteGroups(stream, &NonSimGroups);
}