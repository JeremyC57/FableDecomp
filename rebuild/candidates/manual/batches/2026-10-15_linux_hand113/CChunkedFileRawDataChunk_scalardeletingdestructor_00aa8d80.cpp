// CChunkedFileRawDataChunk::`scalar deleting destructor'(unsigned int) -- stores the CChunkedFileRawDataChunk vtable, delete[]s the raw data block (+0x10), the inlined CChunkedFileChunk destructor stores its vtable, then CBase::~CBase and a free on bit 0
// Leaf: ??_GCChunkedFileRawDataChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA8D80, length 56 bytes (vtable 0x0129E4B8 slot 0).
// Callees: operator delete[] @ 0x00BFEB1C, CBase::~CBase @ 0x0099A300, operator delete @ 0x00BFE9BC.
// The catalog label "CThingTrackNode::vector_deleting_destructor" is propagated: the only reference is
// slot 0 of the CChunkedFileRawDataChunk vtable 0x0129E4B8.
// Layout: engine/CChunkedFileRawDataChunk.h (vptr, opaque +0x04..+0x0f, Data +0x10, ByteSize +0x14); the
// class is modelled locally as a polymorphic chain over the same offsets.
// Shape: the `delete[] Data` of a byte array is a bare `operator delete[]` (no null test, no element
// destructors); the data word is loaded and pushed before the vtable store. The intermediate
// CChunkedFileChunk (vtable 0x0129E474) has an inline empty destructor (its vtable store follows).
struct CBase {                                     // root polymorphic base
    CBase();
    virtual ~CBase();                              // 0x0099A300
};

struct CChunkedFileChunk : CBase {                 // vtable 0x0129E474
    unsigned char _pad_0x04[0x0c];                 // +0x04
    CChunkedFileChunk() { }
    virtual ~CChunkedFileChunk() { }
};

struct CChunkedFileRawDataChunk : CChunkedFileChunk {   // vtable 0x0129E4B8
    unsigned char* Data;                           // +0x10
    unsigned long  ByteSize;                       // +0x14
    CChunkedFileRawDataChunk() : Data(0), ByteSize(0) { }
    __forceinline virtual ~CChunkedFileRawDataChunk() { delete[] Data; }
};

CChunkedFileRawDataChunk* Create_CChunkedFileRawDataChunk()
{
    return new CChunkedFileRawDataChunk;
}
