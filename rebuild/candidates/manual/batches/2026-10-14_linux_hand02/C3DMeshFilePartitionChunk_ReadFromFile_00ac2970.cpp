// C3DMeshFilePartitionChunk::ReadFromFile(CDataInputStream&, long, CChunkedFileLoader&) -- new partition element (inline ctor zeroes its four pointer pairs), read it, hand it to the counted pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2970, length 80 bytes.
// Callees: 0x00BFEA1A operator new, 0x00AC21E0 C3DMeshPartitionElement::ReadFromFile, 0x00AC38A0 CCountedPointer::Reset
// Only the stream is used; the other two arguments are the chunk reader's standard signature (ret 0Ch).
//
#include "engine/C3DMeshFilePartitionChunk.h"   // PDB layout agrees with retail here (counted pointer at +0x10)
struct CChunkedFileChunk {
    virtual void v0();
    CChunkedFileChunk(const char* chunkType);           // 0x00AA7590
    unsigned char _base_body[0x0c];                     // ChunkType[5] + pad + Parent  (sizeof == 0x10)
};
struct CDataInputStream;
struct C3DMeshPartitionElement {
    unsigned char _bounds[0x18];                       // +0x00 bounding volume, read from the stream
    void* m_pTriangles;   void* m_pTrianglesInfo;      // +0x18 counted pointer
    void* m_pVertices;    void* m_pVerticesInfo;       // +0x20 counted pointer
    void* m_pChildA;      void* m_pChildAInfo;         // +0x28 counted pointer
    void* m_pChildB;      void* m_pChildBInfo;         // +0x30 counted pointer   (sizeof == 0x38)
    // The eight null stores are emitted children first, then vertices, then triangles: body
    // assignments in that order (member initialisers would follow the declaration order).
    C3DMeshPartitionElement()
    {
        m_pChildA = 0;    m_pChildAInfo = 0;
        m_pChildB = 0;    m_pChildBInfo = 0;
        m_pVertices = 0;  m_pVerticesInfo = 0;
        m_pTriangles = 0; m_pTrianglesInfo = 0;
    }
    void ReadFromFile(CDataInputStream& in);           // 0x00AC21E0
};
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T* m_pData; CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pData(0), m_pInfo(0) {}
    void Reset(T* p);                                   // 0x00AC38A0
};
struct C3DMeshFilePartitionChunkRetail : CChunkedFileChunk {
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    CCountedPointer<C3DMeshPartitionElement> PartitionTree;   // +0x10 / +0x14
    C3DMeshFilePartitionChunkRetail() : CChunkedFileChunk("MPRT") {}
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFilePartitionChunkRetail) == sizeof(C3DMeshFilePartitionChunk));
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual CChunkedFileChunk* RegisterNewChunk(CChunkedFileChunk* chunk);   // slot 10 (+0x28)
};

struct CChunkedFileLoader;
struct C3DMeshFilePartitionChunk_Methods : C3DMeshFilePartitionChunkRetail {
    void ReadFromFile(CDataInputStream& in, long chunkSize, CChunkedFileLoader& loader);
};
void C3DMeshFilePartitionChunk_Methods::ReadFromFile(CDataInputStream& in, long chunkSize, CChunkedFileLoader& loader)
{
    C3DMeshPartitionElement* element = new C3DMeshPartitionElement;
    element->ReadFromFile(in);
    PartitionTree.Reset(element);
}
