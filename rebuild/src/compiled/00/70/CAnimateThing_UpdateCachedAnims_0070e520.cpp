// CAnimateThing::UpdateCachedAnims() -- refresh animation states, retaining valid previous states while streaming.
// Retail: 0x0070E520, 290 bytes. Leaf: UpdateCachedAnims
// CAnimateThing has no shared header; PDB donor puts StartAnimState at +90, retail +68.
// The four 8-byte blend states and refresh stamps are shifted by -28 from the donor.
// C3DAnimationBlendState has no shared header. Its +4 intrusive pointer is confirmed
// by landed assignment 00710600. CIVCountedPointeeBase.h is data-only: this shell
// adds the two verified virtual slots while preserving IVRefCount +4.
// Calls: CalcCachedAnims 0070E2A0, AnimationBank_ValidateAnimationStates 0070C770
// (__fastcall bank in ecx/state in edx), blend-state assignment 00710600.
#include "engine/CWorld.h"
struct CIVCountedAnimationData {
    virtual void Destroy(int);
    virtual void Delete();
    unsigned long IVRefCount;
};
struct CIVAnimationDataPointer {
    CIVCountedAnimationData* Object;
    CIVAnimationDataPointer& operator=(const CIVAnimationDataPointer& other) {
        CIVCountedAnimationData* old=Object;
        if (old != other.Object) {
            if (old) {
                if (--old->IVRefCount == 0) old->Delete();
                Object=0;
            }
            Object=other.Object;
            if (Object) ++Object->IVRefCount;
        }
        return *this;
    }
};
class C3DAnimationBlendState { public:
    void* VTable;
    CIVAnimationDataPointer Data;
    C3DAnimationBlendState& operator=(const C3DAnimationBlendState&); // 00710600
};
inline CMeshDataBank* GetMainMeshBank(CWorld* world) { return world->PMainMeshBank_Data; }
class CMainGameComponent { public:
    unsigned char _beforeWorld[0x24];
    CWorld* PWorld;
    CWorld* GetWorld() const { return PWorld; }
};
extern CMainGameComponent* g_pMainGameComponent; // 013B86A0
extern bool g_ValidateAnimationStates; // 013B86C9
// CMeshDataBank.h has data only; this method-only view accesses no members.
struct CMeshDataBankAnimationAccess { bool FABLE_FASTCALL ValidateAnimationStates(const C3DAnimationBlendState*); }; // 0070C770
class CAnimateThing { public:
    unsigned char _currentAndDelayedAnimations[0x68];
    C3DAnimationBlendState StartAnimState, EndAnimState, LastStartAnimState, LastEndAnimState;
    float Scale, ChangeAlphaBy;
    long AnimDelay;
    unsigned long Interruptable;
    unsigned long CurrentAnimRefreshStamp, LastAnimRefreshStamp;
    void CalcCachedAnims(); // 0070E2A0
    void UpdateCachedAnims();
};
void CAnimateThing::UpdateCachedAnims()
{
    if (LastAnimRefreshStamp != CurrentAnimRefreshStamp) {
        CalcCachedAnims();
        if (g_ValidateAnimationStates &&
            (!reinterpret_cast<CMeshDataBankAnimationAccess*>(GetMainMeshBank(g_pMainGameComponent->GetWorld()))->ValidateAnimationStates(&StartAnimState) ||
             !reinterpret_cast<CMeshDataBankAnimationAccess*>(GetMainMeshBank(g_pMainGameComponent->GetWorld()))->ValidateAnimationStates(&EndAnimState))) {
            if (reinterpret_cast<CMeshDataBankAnimationAccess*>(GetMainMeshBank(g_pMainGameComponent->GetWorld()))->ValidateAnimationStates(&LastStartAnimState) &&
                reinterpret_cast<CMeshDataBankAnimationAccess*>(GetMainMeshBank(g_pMainGameComponent->GetWorld()))->ValidateAnimationStates(&LastEndAnimState)) {
                StartAnimState=LastStartAnimState;
                EndAnimState=LastEndAnimState;
            }
        } else {
            LastStartAnimState.Data=StartAnimState.Data;
            LastEndAnimState.Data=EndAnimState.Data;
        }
        LastAnimRefreshStamp=CurrentAnimRefreshStamp;
    }
}