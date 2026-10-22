// NParticleEngine::CPSCAttractor::ReadBinary(CDataInputStream*) -- read the attractor settings and its user points back (the mirror of WriteBinary 0x00B17F70; the position-param name word is skipped)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18390, 785 bytes (vtable slot 15 of CPSCAttractor, 0x012A04AC).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (every read is the inlined CDataInputStream::Read,
//   as in the landed CPSCOrbit::ReadBinary 0x00B16F60), CArray<C3DVector>::resize(n, const C3DVector&) 0x0075C040
//   (as in the landed CPSCAttractor::DatabaseCopy 0x00B18060), CDataInputStream::Read3DVector 0x009950F0 (C3DVector by
//   hidden pointer, as in the landed C3DMeshFileConvexVolumeChunk::ReadFromFile 0x00AAAB10).
// Layout: PDB (Ego_r) CPSCAttractor (retail offsets equal the PDB, the point CArray is a 12-byte STLport vector):
//   flag bits +0x0c, AttractorInfluenceFallOff +0x10, AttractorPositionParam +0x14, AttractorPositionParamName +0x18,
//   radius/force +0x1c/+0x20, AttractorUserPointsArray +0x24. CDataInputStream as in the landed readers.
// Shape: the eight inlined reads in WriteBinary order; the name word is read and dropped (no store survives); the flags
//   are bool bit-field assignments from a one-byte read (`test ; setne ; xor ; and ; xor`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    C3DVector Read3DVector();                           // 0x009950F0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};
struct CVector3D {                                      // STLport vector<C3DVector> (12 bytes)
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() { return _M_start; }
    C3DVector& operator[](unsigned long n) { return begin()[n]; }
    void resize(unsigned int newSize, const C3DVector& value = C3DVector());   // 0x0075C040
};
namespace NParticleEngine {
struct CPSCAttractor {
    unsigned char _base[0xc];                           // CPSCBase
    bool AttractorEnabled : 1;                          // +0x0c bit 0
    bool AttractorUseParamPosition : 1;                 // +0x0c bit 1
    long AttractorInfluenceFallOff;                     // +0x10
    unsigned long AttractorPositionParam;               // +0x14
    unsigned long AttractorPositionParamName;           // +0x18
    float AttractorInfluenceRadius;                     // +0x1c
    float AttractorInfluenceForce;                      // +0x20
    CVector3D AttractorUserPointsArray;                 // +0x24
    void ReadBinary(CDataInputStream* in);
};
void CPSCAttractor::ReadBinary(CDataInputStream* in)
{
    AttractorEnabled = in->ReadBool();
    AttractorUseParamPosition = in->ReadBool();
    AttractorInfluenceFallOff = in->ReadLong();
    AttractorPositionParam = in->ReadULong();
    in->ReadULong();
    AttractorInfluenceRadius = in->ReadFloat();
    AttractorInfluenceForce = in->ReadFloat();
    unsigned long count = in->ReadULong();
    AttractorUserPointsArray.resize(count);
    for (unsigned long i = 0; i < count; ++i)
        AttractorUserPointsArray[i] = in->Read3DVector();
}
}