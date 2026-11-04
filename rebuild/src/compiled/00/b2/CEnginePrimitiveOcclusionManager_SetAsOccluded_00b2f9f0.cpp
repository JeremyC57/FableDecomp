// CEnginePrimitiveOcclusionManager::SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const -- mark a primitive occluded for the next PrimitiveOccludeDelay+1 frames, drop its pending pixel-count tests, stamp the occlusion frame
// Leaf: ?SetAsOccluded@CEnginePrimitiveOcclusionManager@@QBEXAAVCEnginePrimitiveOcclusionInfo@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2F9F0, 67 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetAsOccluded@CEnginePrimitiveOcclusionManager@@QBEXAAVCEnginePrimitiveOcclusionInfo@@@Z;
//   donor order (CheckVisibilityResult, SetAsOccluded, SetAsVisible, GetOccludedFrames, ??_G Info) matches retail
//   0x00B2F9F0 / 0x00B2FA40 (SetAsVisible: InvisibleFrames = 0, the same clear and stamp) / 0x00B2FA90 (Info ctor) /
//   0x00B2FB10 (??_G, slot 0 of the Info vtable 0x012A1284). this+0x0C is PrimitiveOccludeDelay (the manager ctor
//   0x00B2F760 stores 4 there); info +0x08 InvisibleFrames, +0x0C OcclusionTestHandles, +0x1C TestFrame (PDB).
// Globals: 0x01436E5C is the CEngineOcclusionManager (Ego_r.pdb CurrentFrame at +0x4010).
// Callees: memmove 0x00BFEAE6 (the dead arm of the inlined STLport __copy_trivial).
// Shape: the private ClearHandles() inline = OcclusionTestHandles.clear() = STLport 4.6 erase(begin(), end()):
//   copy(last, _M_finish, first) folds to __copy_trivial(_M_finish, _M_finish, _M_start), keeping retail's
//   `cmp eax,eax` and the never-taken memmove arm; destroying the unsigned long tail is empty.
#define CEnginePrimitiveOcclusionManager CEnginePrimitiveOcclusionManager_HeaderLayout
#include "engine/CEnginePrimitiveOcclusionManager.h"
#undef CEnginePrimitiveOcclusionManager
#include "engine/CEngineOcclusionManager.h"

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

#include "engine/CArray.h"


class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    mutable unsigned long IVRefCount;                // +0x04
};

class CEnginePrimitiveOcclusionInfo : public CIVCountedPointeeBase {
public:
    virtual ~CEnginePrimitiveOcclusionInfo();
    long InvisibleFrames;                            // +0x08
    CArray<unsigned long> OcclusionTestHandles;      // +0x0C
    bool Seen;                                       // +0x18
    unsigned long TestFrame;                         // +0x1C
private:
    void ClearHandles() { OcclusionTestHandles.clear(); }
    friend class CEnginePrimitiveOcclusionManager;
};

extern CEngineOcclusionManager* g_pOcclusionManager;     // 0x01436E5C

class CEnginePrimitiveOcclusionManager : public CEnginePrimitiveOcclusionManager_HeaderLayout {
public:
    void SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const;
};

void CEnginePrimitiveOcclusionManager::SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const
{
    info.InvisibleFrames = PrimitiveOccludeDelay + 1;
    info.ClearHandles();
    info.TestFrame = g_pOcclusionManager->CurrentFrame;
}