// Grade: DIFFER(496v496) -- register allocation only: totalIndices/new buffer in ebx/edi swapped vs retail, the degenerate flag loaded into bl instead of cmp byte ptr in the copy/clear loops
// C3DPrimitive2::CreateDegenerateTriangleIndexBuffer() -- copy the index runs of every degenerate static block into a new index buffer, rebase those blocks, clear the others and replace the primitive's index buffer
// Retail @ 0x00A90260, 496 bytes (`ret`, bool true in al).
// Callees: CIndexBuffer::GetSize 0x00AC1F10, CIndexBuffer() 0x00AC1EC0, Initialise(count, format, usage, indexSize,
//   primitiveType) 0x00AC1F70, Lock(offset, flags, size) 0x00AC1FB0, Unlock 0x00AC1F00, operator new 0x00BFEA1A,
//   vector<unsigned long>(size_type) 0x00A96100 (out of line), free 0x00BFEA14 (the vector's storage release), the
//   old buffer's virtual deleting destructor (slot 0).
// Shape: per-block index counts (next block's start, or the buffer total for the last) in a local vector; the
// degenerate total; when non-zero a new buffer of twice that many bytes (32-bit-index flag picks the format) is
// filled block by block with memcpy (rep movsd/movsb) while the degenerate blocks get their new start; the other
// blocks are zeroed; the scoped_ptr reset deletes the old buffer (or swaps in null when nothing was degenerate).
// Layout: retail C3DPrimitive2 (0xA0 bytes, the PDB's CArray is 4 bytes larger): IndexBuffer +0x34, StaticBlockCount
// +0x40, StaticBlocks +0x44 (32-byte CStaticBlock: count +0, start index +4, degenerate flag +9), InitFlags +0x68.
extern "C" void __cdecl free(void* p);
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
void* operator new(unsigned int size);

struct CIndexBuffer {
    virtual ~CIndexBuffer();
    void* PD3DIndexBuffer;                 // +4
    unsigned long ByteLength;              // +8
    long PrimitiveType;                    // +C
    void* PAllocatedBuffer;                // +10
    CIndexBuffer();                                                         // 0x00AC1EC0
    unsigned long GetSize();                                                // 0x00AC1F10
    void Initialise(unsigned long count, unsigned long format, long usage, long indexSize, long primitiveType);   // 0x00AC1F70
    unsigned char* Lock(unsigned long offset, unsigned long flags, unsigned long size);   // 0x00AC1FB0
    void Unlock();                                                          // 0x00AC1F00
};

template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
};

struct CULongVector {                      // STLport vector<unsigned long>
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    CULongVector(unsigned long n);         // 0x00A96100
    ~CULongVector() { if (_M_start) free(_M_start); }
    unsigned long& operator[](unsigned long n) { return _M_start[n]; }
};

struct CStaticBlock {
    unsigned long NoTriangles;             // +0
    unsigned long StartIndex;              // +4
    unsigned char Flags;                   // +8
    bool IsDegenerate;                     // +9
    unsigned char _pad[0x20 - 0x0A];
};

struct C3DPrimitive2 {
    unsigned char _pad0[0x34];
    scoped_ptr<CIndexBuffer> IndexBuffer;  // +0x34
    unsigned char _pad38[0x40 - 0x38];
    unsigned long StaticBlockCount;        // +0x40
    CStaticBlock* StaticBlocks;            // +0x44
    unsigned char _pad48[0x68 - 0x48];
    unsigned char InitFlags;               // +0x68
    bool CreateDegenerateTriangleIndexBuffer();
};

bool C3DPrimitive2::CreateDegenerateTriangleIndexBuffer()
{
    unsigned long totalIndices = IndexBuffer->GetSize() / 2;
    CULongVector blockSizes(StaticBlockCount);
    unsigned long i;
    for (i = 0; i < StaticBlockCount; ++i) {
        const CStaticBlock& block = StaticBlocks[i];
        blockSizes[i] = (i + 1 < StaticBlockCount ? (&block)[1].StartIndex : totalIndices) - block.StartIndex;
    }
    unsigned long degenerateIndices = 0;
    for (i = 0; i < StaticBlockCount; ++i)
        if (StaticBlocks[i].IsDegenerate)
            degenerateIndices += blockSizes[i];
    CIndexBuffer* newBuffer = 0;
    if (degenerateIndices > 0) {
        CIndexBuffer* buffer = new CIndexBuffer();
        newBuffer = buffer;
        unsigned long format = 8;
        long indexSize = 1;
        if (InitFlags & 1) {
            format = 0x18;
            indexSize = 2;
        }
        buffer->Initialise(degenerateIndices * 2, format, 0x65, indexSize, 4);
        unsigned short* source = (unsigned short*)IndexBuffer->Lock(0, 0, totalIndices * 2);
        unsigned short* dest = (unsigned short*)buffer->Lock(0, 0, degenerateIndices * 2);
        unsigned long sourceIndex = 0;
        unsigned long destIndex = 0;
        for (i = 0; i < StaticBlockCount; ++i) {
            CStaticBlock& block = StaticBlocks[i];
            unsigned long size = blockSizes[i];
            if (block.IsDegenerate) {
                block.StartIndex = destIndex;
                memcpy(dest + destIndex, source + sourceIndex, size * 2);
                destIndex += size;
            }
            sourceIndex += size;
        }
        buffer->Unlock();
        IndexBuffer->Unlock();
    }
    for (i = 0; i < StaticBlockCount; ++i) {
        CStaticBlock& block = StaticBlocks[i];
        if (!block.IsDegenerate) {
            block.StartIndex = 0;
            block.NoTriangles = 0;
        }
    }
    IndexBuffer.reset(newBuffer);
    return true;
}
