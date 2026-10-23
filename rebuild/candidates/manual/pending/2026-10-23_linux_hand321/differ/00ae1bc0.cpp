// Grade: DIFFER(1158v1158) -- one instruction pair: the first 4-byte read after the alpha-array read (GravityStrength) loads StreamPos as 'mov eax,[ebx+4]; lea ecx,[eax+4]' in retail but 'mov ecx,[ebx+4]; add ecx,4' in ours (register choice at the join after the array read); every other byte matches. Tried: named read temporaries (frame 0x24 -> fixed by the inline ReadValue<T> template), size test inside Read (wrong compare polarity -> separate ReadArray with size <= remaining), nested-if ReadArray
// NParticleEngine::CParticleSimulation::ReadFromFile(CDataInputStream* in, CMovableResourceMemoryPool* pool) -- read the timestep settings and the particle count, allocate the arrays from the pool (fail if that fails), read the positions and copy them into the last and saved positions, read the per-particle alphas and the gravity / wind / dragging / acceleration / damping settings, and mark the bounding box invalid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE1BC0, length 1168 bytes, `ret 8`.
// Callees: every read is the inlined CDataInputStream::Read (64-bit position range check, direct copy while the source
//   chunk has room, else CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0), as in the landed
//   NParticleEngine::CPSCOrbit::ReadBinary 0x00B16F60; AllocateArrays 0x00ADF2A0, ReadVectorArray 0x00AE1730 (__fastcall),
//   std::copy<C4DVector*> 0x00AE61E0 (/Gr __fastcall: first in ecx, last in edx).
// Layout: Ego_r.pdb CParticleSimulation (no shift): Timestep +0x0c, TimestepChanged +0x10, TimestepMultiplier +0x14,
//   Size +0x18, Positions +0x1c, LastPositions +0x20, SavedPositions +0x24, SimulationAlphas +0x2c, GravityStrength
//   +0x34, WindStrength +0x38, DraggingEnable +0x3c, DraggingRotational +0x3d, DraggingStrength +0x40,
//   AccelerationEnable +0x44, GlobalDamping +0x48, BoundingBoxValid +0x70.
// Shape: the two flags stored from a 4-byte / 1-byte read go through a non-bool temporary (`setne`); the alpha array
//   read tests its variable size before the range check.
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
    __forceinline void ReadArray(void* dst, long size) {
        if (size <= 0) return;
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    template <class T> __forceinline T ReadValue() { T value; Read(&value, sizeof(T)); return value; }
};
struct C4DVector { float X, Y, Z, W; };
C4DVector* __fastcall copy(C4DVector* first, C4DVector* last, C4DVector* dest);   // 0x00AE61E0
void __fastcall ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream* in, float w);   // 0x00AE1730
struct CMovableResourceMemoryPool;
namespace NParticleEngine {
struct CParticleSimulation {
    void* __vftable;
    void* RenderData[2];                                // +0x04 CCountedPointer<CParticleSimulationRenderData>
    float Timestep;                                     // +0x0c
    bool  TimestepChanged;                              // +0x10
    float TimestepMultiplier;                           // +0x14
    unsigned long Size;                                 // +0x18
    C4DVector* Positions;                               // +0x1c
    C4DVector* LastPositions;                           // +0x20
    C4DVector* SavedPositions;                          // +0x24
    C4DVector* Accelerations;                           // +0x28
    float* SimulationAlphas;                            // +0x2c
    float MaximumAdjustment;                            // +0x30
    float GravityStrength;                              // +0x34
    float WindStrength;                                 // +0x38
    bool  DraggingEnable;                               // +0x3c
    bool  DraggingRotational;                           // +0x3d
    float DraggingStrength;                             // +0x40
    bool  AccelerationEnable;                           // +0x44
    float GlobalDamping;                                // +0x48
    unsigned char _pad4c[0x70 - 0x4c];
    bool  BoundingBoxValid;                             // +0x70
    bool AllocateArrays(CMovableResourceMemoryPool* pool);   // 0x00ADF2A0
    bool ReadFromFile(CDataInputStream* in, CMovableResourceMemoryPool* pool);
};
bool CParticleSimulation::ReadFromFile(CDataInputStream* in, CMovableResourceMemoryPool* pool)
{
    Timestep = in->ReadValue<float>();
    TimestepChanged = in->ReadValue<long>() != 0;
    TimestepMultiplier = in->ReadValue<float>();
    Size = in->ReadValue<unsigned long>();
    if (!AllocateArrays(pool))
        return false;
    ReadVectorArray(Positions, Size, in, 0.0f);
    copy(Positions, Positions + Size, LastPositions);
    copy(Positions, Positions + Size, SavedPositions);
    in->ReadArray(SimulationAlphas, Size * 4);
    GravityStrength = in->ReadValue<float>();
    WindStrength = in->ReadValue<float>();
    DraggingEnable = in->ReadValue<unsigned char>() != 0;
    DraggingRotational = in->ReadValue<unsigned char>() != 0;
    DraggingStrength = in->ReadValue<float>();
    AccelerationEnable = in->ReadValue<unsigned char>() != 0;
    GlobalDamping = in->ReadValue<float>();
    BoundingBoxValid = false;
    return true;
}
}
