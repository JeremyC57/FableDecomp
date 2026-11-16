// CParticleSimulation::ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& in, float w) -- read count packed C3DVectors from the stream into a temporary CArray and widen them into the C4DVector array with w as the fourth component
// Leaf: ?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE1730, 391 bytes, protected static __fastcall (vectors ecx, count edx; in and w on
//   the stack), `ret 8`.
// Identity (proven, two sources): (1) Ego_r.pdb `?ReadVectorArray@CParticleSimulation@@KIXPAVC4DVector@@KAAVCDataInputStream@@M@Z`
//   -- the only PDB function taking (C4DVector*, unsigned long, CDataInputStream&, float), exactly retail's register / stack
//   shape (`ret 8`: the stream reference and the float); (2) its two callers are CParticleSimulation::ReadFromFile 0x00AE1BC0
//   (call 0x00AE1D42) and CParticleProgram::ReadFromFile 0x00AE3C30 (call 0x00AE4043, w = 0), and the catalog row is
//   already named ReadVectorArray.
// Callees: STLport vector<C3DVector>(size_type) 0x007C2AB0 (??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@I@Z;
//   CArray<C3DVector>(n) forwards to it inline),
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline Read), free 0x00BFEA14 (~CArray).
// Shape: CArray::GetAsCArray() (`size() > 0 ? _M_start : GetZeroSizeArray()`, the zero-size pointer being the array object
//   itself: `lea edi,[esp+14h]`), GetByteSize() tested `> 0`, and the inline stream Read (64-bit position range check,
//   `size <= SourceChunkBytesRemaining` memcpy fast path, else the private overflow call -- the landed CThingComponentSet
//   0x008707A0 form; reached through the engine header's FABLE_CDATAINPUTSTREAM_FRIEND hook). The loop calls the inline
//   C4DVector::Initialise(const C3DVector&, float) (PDB): with an inline member call in the body VC7.1 unrolls the loop
//   by four (retail's `cmp ebx,4 ; jl` + remainder loop); the same stores written field by field, through a reference
//   local, as a while loop or with a signed index are never unrolled (234v391).
// Ported 2026-11-16: the local std::vector / CArray stand-in (which bound 0x007C2AB0 as a made-up ??0?$CArray@VC3DVector@@@@QAE@K@Z)
//   replaced by the shared engine/CArray.h retail branch, as the landed 0x00B149D0 / 0x00B22B40: CArray<T>(n) forwards to STLport
//   vector<T>(n), which VC7.1 calls out of line; GETASCARRAY_START keeps the _M_start read, GETBYTESIZE is the PDB inline.
#include "fable_compiler.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
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
};

#include "engine/C3DVector.h"
class C4DVector {                                         // Ego_r / FableWin: 16 bytes
public:
    float X, Y, Z, W;
    void Initialise(const C3DVector& v, float w) { X = v.X; Y = v.Y; Z = v.Z; W = w; }   // ?Initialise@C4DVector@@QAEXABVC3DVector@@M@Z
};

#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#include "engine/CArray.h"   // CArray<T> : STLport vector<T>; CArray(n) forwards to vector<C3DVector>(size_type) 0x007C2AB0

class CParticleSimulation {                                // Ego_r: CParticleSimulation (class)
protected:
    static void __fastcall ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& in, float w);
};

void __fastcall CParticleSimulation::ReadVectorArray(C4DVector* vectors, unsigned long count, CDataInputStream& in, float w)
{
    CArray<C3DVector> temp(count);
    C3DVector* data = temp.GetAsCArray();
    long bytes = temp.GetByteSize();
    if (bytes > 0)
        static_cast<CDataInputReader&>(in).Read(data, bytes);
    for (unsigned long i = 0; i < count; ++i)
        vectors[i].Initialise(temp[i], w);
}