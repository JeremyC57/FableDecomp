// CEngineOcclusionManager::GetFlareTestResult(unsigned long handle) -- the visible fraction of a flare test: 0 for an unknown handle, else the occluded pixel count over the entry's MaxPixelCount clamped to 1
// Leaf: ?GetFlareTestResult@CEngineOcclusionManager@@QAEMK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8D280, 108 bytes, thiscall, `ret 4`, float in st0.
// Identity (proven): Ego_r.pdb ?GetFlareTestResult@CEngineOcclusionManager@@QAEMK@Z, which the PDB places right before
//   GetPrevFrameFlareTestResult; this body reads FlareTests (+0x24024) where its byte twin 0x00B8D2F0 reads
//   PrevFrameFlareTests (+0x24030), and its only caller tail-jumps here from 0x00B8D3AF (GetFlareTestResultFromRetrivalID,
//   the twin of the landed GetPrevFrameFlareTestResultFromRetrivalID 0x00B8D3C0 that jumps to 0x00B8D2F0).
// Layout: engine/CEngineOcclusionManager.h (retail = Ego_r - 4 per narrower CArray): FlareTests +0x24024, a 12-byte CArray
//   of the 28-byte COccluderFlareEntry {Position, Radius, CircleSegments, MaxPixelCount +0x14, OccluderHandle +0x18}.
// Shape: the default 0.0f is loaded before the size test (one `fstp st(0)` drops it on the hit path); size() is the signed
//   /28 (`imul 92492493h ; sar 4`) compared unsigned with the handle; operator[] re-reads the array start;
//   `(float)MaxPixelCount` is the unsigned `fild` + 2^32 fix-up. The occlusion-query pixel count is folded to the 0.0f
//   pool constant in the PC build (`fdivr [0.0f]`): the retail PC engine has no occlusion queries, so the count read
//   through the PDB's GetResult stub is a constant 0 -- written here as the literal it compiles to.
#define CEngineOcclusionManager CEngineOcclusionManagerLayout
#include "engine/CEngineOcclusionManager.h"
#undef CEngineOcclusionManager

struct COccluderFlareEntry {                    // Ego_r.pdb CEngineOcclusionManager::COccluderFlareEntry, 28 bytes
    float         Position[3];                  // +0x00 C3DVector
    float         Radius;                       // +0x0c
    unsigned long CircleSegments;               // +0x10
    unsigned long MaxPixelCount;                // +0x14
    unsigned long OccluderHandle;               // +0x18
};

template <class T> class CArray {               // retail CArray: the STLport 4.6 vector words
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int i) { return _M_start[i]; }
};

class CEngineOcclusionManager : public CEngineOcclusionManagerLayout {
public:
    CArray<COccluderFlareEntry>& GetFlareTests() { return *(CArray<COccluderFlareEntry>*)FlareTests; }
    float GetFlareTestResult(unsigned long handle);
};

float CEngineOcclusionManager::GetFlareTestResult(unsigned long handle)
{
    float result = 0.0f;
    if (handle < GetFlareTests().size()) {
        COccluderFlareEntry& entry = GetFlareTests()[handle];
        result = 0.0f / (float)entry.MaxPixelCount;
        if (result > 1.0f)
            result = 1.0f;
    }
    return result;
}