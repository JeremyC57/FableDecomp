#ifndef _MSC_VER
#define FABLE_ENGINE_CWORLD_H
#define FABLE_FASTCALL
class CMeshDataBank;
class CWorld { public:unsigned char beforeBank[0x3c];CMeshDataBank* PMainMeshBank_Data;};
#endif
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

class CMeshDataBank { public:};
#include <stdio.h>
CMainGameComponent* g_pMainGameComponent;
bool g_ValidateAnimationStates;
static CAnimateThing* active;
static CMeshDataBank* expectedBank;
static unsigned answers;
static int calcCalls, checkCalls, assignCalls, invalid;
static const C3DAnimationBlendState* queried[4];
static CIVCountedAnimationData* calculatedStart;
static CIVCountedAnimationData* calculatedEnd;
static CIVCountedAnimationData* replacement;
static bool mutateOnDelete;
static bool advanceStamp;
void CAnimateThing::CalcCachedAnims() {
    ++calcCalls;
    if (this!=active) ++invalid;
    if (calculatedStart) StartAnimState.Data.Object=calculatedStart;
    if (calculatedEnd) EndAnimState.Data.Object=calculatedEnd;
    if (advanceStamp) CurrentAnimRefreshStamp=99;
}
bool FABLE_FASTCALL CMeshDataBankAnimationAccess::ValidateAnimationStates(const C3DAnimationBlendState* state) {
    if ((void*)this!=(void*)expectedBank || calcCalls!=1 || checkCalls>=4) ++invalid;
    if (checkCalls<4) queried[checkCalls]=state;
    int index = state==&active->StartAnimState?0:state==&active->EndAnimState?1:
                state==&active->LastStartAnimState?2:state==&active->LastEndAnimState?3:-1;
    if (index<0) { ++invalid; return false; }
    ++checkCalls;
    return (answers & (1u<<index))!=0;
}
C3DAnimationBlendState& C3DAnimationBlendState::operator=(const C3DAnimationBlendState& other) {
    ++assignCalls;
    if (assignCalls==1 && (this!=&active->StartAnimState || &other!=&active->LastStartAnimState)) ++invalid;
    if (assignCalls==2 && (this!=&active->EndAnimState || &other!=&active->LastEndAnimState)) ++invalid;
    Data=other.Data;
    return *this;
}
void CIVCountedAnimationData::Destroy(int) { ++invalid; }
void CIVCountedAnimationData::Delete() { ++invalid; }
struct Data : CIVCountedAnimationData {
    int deletes;
    Data() { IVRefCount=1; deletes=0; }
    virtual void Delete() {
        ++deletes;
        if (IVRefCount!=0) ++invalid;
        if (mutateOnDelete) { active->StartAnimState.Data.Object=replacement; mutateOnDelete=false; }
    }
};
static void setup(CAnimateThing& a, Data& start, Data& end, Data& oldStart, Data& oldEnd) {
    active=&a; calcCalls=checkCalls=assignCalls=invalid=0;
    calculatedStart=calculatedEnd=replacement=0; mutateOnDelete=advanceStamp=false;
    a.StartAnimState.Data.Object=&start; a.EndAnimState.Data.Object=&end;
    a.LastStartAnimState.Data.Object=&oldStart; a.LastEndAnimState.Data.Object=&oldEnd;
    a.StartAnimState.VTable=(void*)1; a.EndAnimState.VTable=(void*)2;
    a.LastStartAnimState.VTable=(void*)3; a.LastEndAnimState.VTable=(void*)4;
    a.CurrentAnimRefreshStamp=7; a.LastAnimRefreshStamp=6;
    start.IVRefCount=end.IVRefCount=oldStart.IVRefCount=oldEnd.IVRefCount=1;
    start.deletes=end.deletes=oldStart.deletes=oldEnd.deletes=0;
}
int main() {
    CMeshDataBank bank; CWorld world; world.PMainMeshBank_Data=&bank;
    CMainGameComponent component; component.PWorld=&world;
    g_pMainGameComponent=&component; expectedBank=&bank;
    CAnimateThing a; Data start,end,oldStart,oldEnd,other;
    int ok=1;
    // Every validity permutation verifies short-circuit calls and both assignment directions.
    for (unsigned mask=0;mask<16;++mask) {
        setup(a,start,end,oldStart,oldEnd); answers=mask; g_ValidateAnimationStates=true;
        a.UpdateCachedAnims();
        bool current=(mask&3)==3, previous=(mask&12)==12;
        int expectedChecks=(mask&1)?2:1;
        if (!current) expectedChecks+=((mask&4)?2:1);
        ok=ok && invalid==0 && calcCalls==1 && checkCalls==expectedChecks && a.LastAnimRefreshStamp==7;
        ok=ok && queried[0]==&a.StartAnimState;
        if (mask&1) ok=ok && queried[1]==&a.EndAnimState;
        if (current) {
            ok=ok && assignCalls==0 && a.LastStartAnimState.Data.Object==&start && a.LastEndAnimState.Data.Object==&end
                && start.IVRefCount==2 && end.IVRefCount==2 && oldStart.deletes==1 && oldEnd.deletes==1;
        } else if (previous) {
            ok=ok && assignCalls==2 && a.StartAnimState.Data.Object==&oldStart && a.EndAnimState.Data.Object==&oldEnd
                && oldStart.IVRefCount==2 && oldEnd.IVRefCount==2 && start.deletes==1 && end.deletes==1;
        } else {
            ok=ok && assignCalls==0 && a.StartAnimState.Data.Object==&start && a.EndAnimState.Data.Object==&end
                && a.LastStartAnimState.Data.Object==&oldStart && a.LastEndAnimState.Data.Object==&oldEnd
                && start.deletes==0 && end.deletes==0 && oldStart.deletes==0 && oldEnd.deletes==0;
        }
        ok=ok && a.StartAnimState.VTable==(void*)1 && a.EndAnimState.VTable==(void*)2
            && a.LastStartAnimState.VTable==(void*)3 && a.LastEndAnimState.VTable==(void*)4;
    }
    setup(a,start,end,oldStart,oldEnd); a.LastAnimRefreshStamp=7; g_ValidateAnimationStates=true;
    a.UpdateCachedAnims(); ok=ok && calcCalls==0 && checkCalls==0 && assignCalls==0 && start.IVRefCount==1;
    setup(a,start,end,oldStart,oldEnd); g_ValidateAnimationStates=false; advanceStamp=true;
    calculatedStart=&other; other.IVRefCount=1;
    a.UpdateCachedAnims(); ok=ok && checkCalls==0 && a.LastAnimRefreshStamp==99 && a.LastStartAnimState.Data.Object==&other && other.IVRefCount==2;
    setup(a,start,end,oldStart,oldEnd); g_ValidateAnimationStates=false;
    a.LastStartAnimState.Data.Object=&start; a.LastEndAnimState.Data.Object=&end;
    a.UpdateCachedAnims(); ok=ok && start.IVRefCount==1 && end.IVRefCount==1 && start.deletes==0 && end.deletes==0;
    setup(a,start,end,oldStart,oldEnd); g_ValidateAnimationStates=false;
    a.StartAnimState.Data.Object=0; a.EndAnimState.Data.Object=0;
    a.UpdateCachedAnims(); ok=ok && a.LastStartAnimState.Data.Object==0 && a.LastEndAnimState.Data.Object==0 && oldStart.deletes==1 && oldEnd.deletes==1;
    setup(a,start,end,oldStart,oldEnd); g_ValidateAnimationStates=false;
    a.LastStartAnimState.Data.Object=0; oldEnd.IVRefCount=3;
    a.UpdateCachedAnims(); ok=ok && a.LastStartAnimState.Data.Object==&start && start.IVRefCount==2 && oldEnd.IVRefCount==2 && oldEnd.deletes==0;
    setup(a,start,end,oldStart,oldEnd); g_ValidateAnimationStates=false;
    mutateOnDelete=true; replacement=&other; other.IVRefCount=1;
    a.UpdateCachedAnims(); ok=ok && a.LastStartAnimState.Data.Object==&other && other.IVRefCount==2 && start.IVRefCount==1;
    ok=ok && invalid==0;
    printf("0070e520_TEST %s\n",ok?"PASS":"FAIL"); return ok?0:1;
}