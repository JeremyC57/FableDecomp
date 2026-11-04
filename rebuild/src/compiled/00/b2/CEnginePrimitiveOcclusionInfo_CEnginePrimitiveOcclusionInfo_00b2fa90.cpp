// CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo() -- count 1, no pending tests, not seen, stamped with the current occlusion frame
// Leaf: ??0CEnginePrimitiveOcclusionInfo@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2FA90, 48 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012A1284 (RTTI .?AVCEnginePrimitiveOcclusionInfo@@ : CIVCountedPointeeBase);
//   Ego_r.pdb ??0CEnginePrimitiveOcclusionInfo@@QAE@XZ; it follows SetAsOccluded 0x00B2F9F0 / SetAsVisible 0x00B2FA40
//   in the occlusion source file and precedes its ??_G 0x00B2FB10.
// Layout (Ego_r.pdb member names, STLport 12-byte CArray): IVRefCount +4, InvisibleFrames +8, OcclusionTestHandles
//   +0x0C, Seen +0x18, TestFrame +0x1C (retail size 0x20; the PDB's is 0x24).
// Globals: 0x01436E5C the CEngineOcclusionManager (CurrentFrame +0x4010).
// Shape: retail stores the array, then TestFrame, then InvisibleFrames and Seen: TestFrame is a member initialiser
//   (declaration order puts it after the array), InvisibleFrames / Seen are assigned in the body.
#include "engine/CEngineOcclusionManager.h"

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (unused here)

#include "engine/CArray.h"


class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    mutable unsigned long IVRefCount;                // +0x04
};

class CEnginePrimitiveOcclusionInfo : public CIVCountedPointeeBase {
public:
    CEnginePrimitiveOcclusionInfo();
    virtual ~CEnginePrimitiveOcclusionInfo();
    long InvisibleFrames;                            // +0x08
    CArray<unsigned long> OcclusionTestHandles;      // +0x0C
    bool Seen;                                       // +0x18
    unsigned long TestFrame;                         // +0x1C
private:
    void ClearHandles() { OcclusionTestHandles.clear(); }
};

extern CEngineOcclusionManager* g_pOcclusionManager;     // 0x01436E5C

CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo()
    : TestFrame(g_pOcclusionManager->CurrentFrame)
{
    InvisibleFrames = 0;
    Seen = false;
}