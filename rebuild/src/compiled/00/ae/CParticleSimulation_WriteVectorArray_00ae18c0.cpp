// CParticleSimulation::WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out) -- narrow count C4DVectors into a temporary CArray<C3DVector> and write it to the stream as one raw block
// Leaf: ?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE18C0, 436 bytes, protected static __fastcall (vectors ecx, count edx; the stream on the
//   stack), `ret 4`.
// Identity (proven, two sources): (1) Ego_r.pdb and FableWin.pdb both name
//   `?WriteVectorArray@CParticleSimulation@@KIXPBVC4DVector@@KAAVCDataOutputStream@@@Z`, the only PDB function taking
//   (const C4DVector*, unsigned long, CDataOutputStream&), exactly retail's shape; it is the writing twin of the landed
//   ReadVectorArray 0x00AE1730; (2) its two callers are CParticleSimulation::WriteToFile 0x00AE2050 (call 0x00AE208C) and
//   CParticleProgram::WriteToFile 0x00AE2230 (call 0x00AE238E), which bind it under the invented
//   `?WriteVectors_00ae18c0@@YIXPBVC4DVector@@KAAVCDataOutputStream@@@Z` (NAME_FIXES.tsv). Manifest label
//   "WriteAnimationData" is propagated.
// Callees: STLport vector<C3DVector>(size_type) 0x007C2AB0 (??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@I@Z;
//   CArray<C3DVector>(n) forwards to it inline), the stream's
//   virtual Write(const void*, long) (slot 4), free 0x00BFEA14 (~CArray).
// Shape: the landed ReadVectorArray model reversed: the loop body calls the inline C4DVector::ToC3DVector() (PDB, returns by
//   value; the 3-dword frame temporary), which makes VC7.1 unroll it by four like retail; GetAsCArray / GetByteSize for the
//   Write. First grade.
// Ported 2026-11-16: the local std::vector / CArray stand-in (which bound 0x007C2AB0 as a made-up ??0?$CArray@VC3DVector@@@@QAE@K@Z)
//   replaced by the shared engine/CArray.h retail branch, as the landed 0x00B149D0 / 0x00B22B40: CArray<T>(n) forwards to STLport
//   vector<T>(n), which VC7.1 calls out of line; GETASCARRAY_START keeps the _M_start read, GETBYTESIZE is the PDB inline.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CDataOutputStream {                                   // Layout: local model (engine/CDataOutputStream.h flattens the vptr)
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);      // slot 4 (+0x10)
};

class C3DVector { public: float X, Y, Z; };               // 12 bytes
class C4DVector {                                         // Ego_r / FableWin: 16 bytes
public:
    float X, Y, Z, W;
    C3DVector ToC3DVector() const { C3DVector v; v.X = X; v.Y = Y; v.Z = Z; return v; }   // ?ToC3DVector@C4DVector@@QBE?AVC3DVector@@XZ
};

#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#include "engine/CArray.h"   // CArray<T> : STLport vector<T>; CArray(n) forwards to vector<C3DVector>(size_type) 0x007C2AB0

class CParticleSimulation {                                // Ego_r: CParticleSimulation (class)
protected:
    static void __fastcall WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out);
};

void __fastcall CParticleSimulation::WriteVectorArray(const C4DVector* vectors, unsigned long count, CDataOutputStream& out)
{
    CArray<C3DVector> temp(count);
    for (unsigned long i = 0; i < count; ++i)
        temp[i] = vectors[i].ToC3DVector();
    out.Write(temp.GetAsCArray(), temp.GetByteSize());
}