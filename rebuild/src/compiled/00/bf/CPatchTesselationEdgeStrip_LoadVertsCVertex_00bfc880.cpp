// CPatchTesselationEdgeStrip::LoadVerts<CVertex>(CEdgeStipVertexArray<CVertex>& verts, CDataInputStream& stream, CMovableResource* resource) -- read the vertex count and resize the array (false when that fails); with vertices, read the compressed block's size and bytes and range-decompress them into the array
// Leaf: ??$LoadVerts@UCVertex@CPatchTesselationEdgeStrip@@@CPatchTesselationEdgeStrip@@AAE_NAAV?$CEdgeStipVertexArray@UCVertex@CPatchTesselationEdgeStrip@@@@AAVCDataInputStream@@PAVCMovableResource@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bfc880, 402 bytes, thiscall (`this` unused), `ret 0Ch`.
// Identity (proven): it resizes through CEdgeStipVertexArray<CVertex>::Resize 0x00BFBE40 (bound) and decompresses with a stride of 0x14; the landed CPatchTesselationEdgeStrip::Load 0x00BFC600 calls it at 0x00BFC71F for Verts (Ego_r.pdb member at +0x08, in member order); Ego_r.pdb ??$LoadVerts@UCVertex@CPatchTesselationEdgeStrip@@@CPatchTesselationEdgeStrip@@AAE_NAAV?$CEdgeStipVertexArray@UCVertex@CPatchTesselationEdgeStrip@@@@AAVCDataInputStream@@PAVCMovableResource@@@Z (reloc_bindings spells it public with pointer / CMemoryDataInputStream / CMovableResourceHandler parameters).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline reads),
//   CEdgeStipVertexArray<CVertex>::Resize(long, CMovableResource*) 0x00BFBE40, CArray<unsigned char>::CArray(unsigned int) 0x00514E20,
//   CRangeCompressor::Decompress(const unsigned char*, long count, long stride, void*) 0x00F39ED0, free 0x00BFEA14.
// Shape (family L117): the inline CDataInputStream::Read of the landed Load 0x00BFC600 (64-bit position range check, the
//   chunk fast path with memcpy, else the overflow call) with a `size > 0` guard; the count and size locals are homed in
//   dead argument slots, the empty compressor in the resource slot; CArray<unsigned char> as in the landed SaveVerts
//   0x00BFD0B0 (GetData() is begin or the array itself when empty; the destructor frees the block). The vertex pointer and
//   count are read into named locals before the decompress call (direct member arguments compute GetData first and swap
//   the begin/end registers).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CDataInputStream { public:
    virtual void v0();
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        if (size <= 0)
            return;
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline long ReadSLONG() { long value; Read(&value, 4); return value; }
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    CArray(unsigned int size);                          // 0x00514E20 for unsigned char
    ~CArray() { if (First) free(First); }
    unsigned long Size() const { return Last - First; }
    T* GetData() { return Size() > 0 ? First : (T*)this; }
};

class CRangeCompressor {
public:
    void Decompress(const unsigned char* data, long count, long stride, void* output);   // 0x00F39ED0
};

class CMovableResource;
template <class T> class CEdgeStipVertexArray {
public:
    T* Data;                                            // +0x00
    long Count;                                         // +0x04
    bool Resize(long count, CMovableResource* resource);
};

class CPatchTesselationEdgeStrip {
public:
    struct CVertex {                                      // 0x14 bytes (the decompress stride)
        unsigned char _bytes[0x14];
    };
private:
    template <class T> bool LoadVerts(CEdgeStipVertexArray<T>& verts, CDataInputStream& stream, CMovableResource* resource);
};

template <class T>
bool CPatchTesselationEdgeStrip::LoadVerts(CEdgeStipVertexArray<T>& verts, CDataInputStream& stream, CMovableResource* resource)
{
    if (!verts.Resize(stream.ReadSLONG(), resource))
        return false;
    if (verts.Count > 0) {
        long size = stream.ReadSLONG();
        CArray<unsigned char> buffer(size);
        stream.Read(buffer.GetData(), size);
        CRangeCompressor compressor;
        T* vertices = verts.Data;
        long count = verts.Count;
        compressor.Decompress(buffer.GetData(), count, sizeof(T), vertices);
    }
    return true;
}

template bool CPatchTesselationEdgeStrip::LoadVerts<CPatchTesselationEdgeStrip::CVertex>(
    CEdgeStipVertexArray<CPatchTesselationEdgeStrip::CVertex>& verts, CDataInputStream& stream, CMovableResource* resource);