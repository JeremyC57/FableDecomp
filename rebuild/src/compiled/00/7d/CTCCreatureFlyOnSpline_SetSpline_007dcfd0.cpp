// CTCCreatureFlyOnSpline::SetSpline(CCountedPointer<CCardinalSpline> spline) -- take the new flight spline and snap the creature's physics to the spline position at the current progress
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pCount->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 35c (queue land0411j 0x00A567A0): calls 0x00A567A0 as CCardinalSpline::GetSplinePosition(C3DVector&, float) const (Ego_r.pdb ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z; the out vector by reference).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DCFD0, length 130 bytes (`ret 8`: the 8-byte counted pointer by value).
// Callees: the count block's destroy function (+4, object at +8 in ecx), operator delete @ 0x00BFE9BC,
// CCardinalSpline::GetSplinePosition(C3DVector*, float) @ 0x00A567A0, CTCPhysicsBase::ResetPosition (slot 31, +0x7C).
// Layout: Ego_r.pdb CTCCreatureFlyOnSpline: PSpline +0xC (CCountedPointer: object, count block), EngineHandles +0x14 (retail
// 12-byte vector), Progress +0x20, DeltaProgress +0x24, Looping +0x28 (PDB +0x24/+0x28/+0x2C). Retail CThing keeps its physics
// component at +0x60.
// Shape: the member assignment is the counted pointer's inline operator= (self test on the block, release re-reading the member
// block after the decrement, object then block stores, count up); the by-value parameter is released by this callee at the end
// (`dec ; jne`, block kept in edi).
void __cdecl operator delete(void* block);              // 0x00BFE9BC

#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
class CCardinalSpline { public:
    void GetSplinePosition(C3DVector& position, float progress) const;   // 0x00A567A0
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);         // slot 31 (+0x7c)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
class CEnginePrimitiveHandle;
class CTCCreatureFlyOnSpline { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CCountedPointer<CCardinalSpline> PSpline;           // +0x0c
    CEnginePrimitiveHandle* EngineHandles[3];           // +0x14 vector<CEnginePrimitiveHandle>
    float Progress;                                     // +0x20
    float DeltaProgress;                                // +0x24
    bool  Looping;                                      // +0x28
    void SetSpline(CCountedPointer<CCardinalSpline> spline);
};

void CTCCreatureFlyOnSpline::SetSpline(CCountedPointer<CCardinalSpline> spline)
{
    PSpline = spline;
    C3DVector position;
    PSpline->GetSplinePosition(position, Progress);
    ParentThing->PhysicsTC->ResetPosition(position);
}