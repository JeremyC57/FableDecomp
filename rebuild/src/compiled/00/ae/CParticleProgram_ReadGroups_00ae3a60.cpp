// CParticleProgram::ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups) -- private static load helper: read the group count, resize the array, then per group its bone index, its vertex-blend count (resizing the blend array) and the raw blend array
// Leaf: ?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE3A60, 456 bytes, private static __fastcall (stream in ecx, groups in edx), plain `ret`.
// Identity (two sources): Ego_r.pdb `?ReadGroups@CParticleProgram@@CIXAAVCDataInputStream@@AAV?$CArray@VC3DGroup2@@@@@Z`
//   (the only PDB function taking (CDataInputStream&, CArray<C3DGroup2>&), exactly retail's ecx/edx shape) and its two
//   callers 0x00AE4518 / 0x00AE4525 inside CParticleProgram::ReadFromFile 0x00AE3C30 (SimGroups / NonSimGroups), the
//   mirror of the landed WriteGroups 0x00AE0680 (WriteToFile's two tail calls).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline Read), vector<C3DGroup2>::resize
//   0x00AE8540 (landed), C3DGroup2::SetBoneIndex 0x00AC4070, C3DGroup2::Resize 0x00AC41D0, C3DGroup2::GetVertexBlends
//   0x00AC4080 (Ego_r donor order SetBoneIndex / GetVertexBlends / PeekVertexBlends / ... / AddVertexBlend / Resize maps to
//   retail 0x00AC4070 / 0x00AC4080 / 0x00AC40A0 / ... / 0x00AC4110 / 0x00AC41D0; bodies agree).
// Layout: retail CArray<T> is an STLport vector (12 bytes); C3DGroup2 {long BoneIndex; CArray<C3DVertexBlend2>} 16 bytes.
// Shape: the inline stream Read of the landed ReadVectorArray 0x00AE1730 (64-bit range check, memcpy fast path, private
//   overflow call); the count loop is a down-counter like WriteGroups.
#include "fable_compiler.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#define FABLE_CDATAINPUTSTREAM_FRIEND class CDataInputReader
#include "engine/CDataInputStream.h"
class CDataInputReader : public CDataInputStream {         // inline Read helper (no extra state)
public:
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> FABLE_FORCEINLINE T ReadType() { T value; Read(&value, sizeof(T)); return value; }   // PDB ReadType<T>
    FABLE_FORCEINLINE unsigned long ReadULONG() { return ReadType<unsigned long>(); }                       // PDB ReadULONG
    FABLE_FORCEINLINE long ReadSLONG() { return ReadType<long>(); }                                         // PDB ReadSLONG
};

#include "engine/CArray.h"


class C3DVertexBlend2 { public: long Vertex; float Weight; };   // 8 bytes

class C3DGroup2 {                                          // 16 bytes in retail
public:
    long                    BoneIndex;                     // +0x0
    CArray<C3DVertexBlend2> VertexBlends;                  // +0x4
    void SetBoneIndex(long bone);                          // 0x00AC4070
    C3DVertexBlend2* GetVertexBlends();                    // 0x00AC4080
    void Resize(unsigned long size);                       // 0x00AC41D0
};

class CParticleProgram {
private:
    static void __fastcall ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups);
};

void __fastcall CParticleProgram::ReadGroups(CDataInputStream& stream, CArray<C3DGroup2>& groups)
{
    CDataInputReader& in = static_cast<CDataInputReader&>(stream);
    unsigned long count = in.ReadULONG();
    groups.resize(count);
    for (unsigned long i = 0; i < count; ++i) {
        C3DGroup2& group = groups[i];
        group.SetBoneIndex(in.ReadSLONG());
        unsigned long blends = in.ReadULONG();
        group.Resize(blends);
        long bytes = blends * sizeof(C3DVertexBlend2);
        C3DVertexBlend2* data = group.GetVertexBlends();
        if (bytes > 0)
            in.Read(data, bytes);
    }
}