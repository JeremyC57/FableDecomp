// CLocalDetailPrimitiveMesh::Load(CDataInputStream* in) -- read the primitive's bounding box (+0x08), bounding sphere (+0x20), placement matrix (+0x38) and scale (+0x68) from the binary stream; always succeeds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFA0F0, length 432 bytes (`ret 4`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) @ 0x00993CA0 (the slow path of the inline read).
// Layout: Ego_r.pdb CLocalDetailPrimitiveMesh: Matrix (CMatrix3x4) +0x38, Scale +0x68; the 0x18/0x10-byte blocks at +0x08/+0x20 are the
//   C3DBoundingBox/C3DBoundingSphere of the CLocalDetailPrimitive base (no engine header). CDataInputStream as in the landed
//   CPSCOrbit::ReadBinary 0x00B16F60 (StreamPos +4, CurrentSourcePtr +0xc, SourceChunkBytesRemaining +0x14).
// Shape: the stream's inline template Read<T>(T&) (64-bit position check, in-chunk memcpy or the overflow call); the scale is read
//   into a local (homed in the dead `in` argument slot) and then stored.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    template <class T> __forceinline void Read(T& value) { Read(&value, sizeof(T)); }
};
struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector MinCoord, MaxCoord; };             // 0x18
struct C3DBoundingSphere { C3DVector Centre; float Radius; };        // 0x10
struct CMatrix3x4 { float E[12]; };                                  // 0x30
struct CLocalDetailPrimitiveMesh {
    void*             __vftable;                    // +0x00
    unsigned char     _pad04[4];
    C3DBoundingBox    BBox;                         // +0x08
    C3DBoundingSphere BSphere;                      // +0x20
    unsigned char     _pad30[8];
    CMatrix3x4        Matrix;                       // +0x38
    float             Scale;                        // +0x68
    bool Load(CDataInputStream* in);
};

bool CLocalDetailPrimitiveMesh::Load(CDataInputStream* in)
{
    in->Read(BBox);
    in->Read(BSphere);
    in->Read(Matrix);
    float scale;
    in->Read(scale);
    Scale = scale;
    return true;
}