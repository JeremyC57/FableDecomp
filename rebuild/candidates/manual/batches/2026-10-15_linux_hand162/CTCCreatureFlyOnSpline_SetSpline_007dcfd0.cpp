// CTCCreatureFlyOnSpline::SetSpline(CCountedPointer<CCardinalSpline> spline) -- take the new flight spline and snap the creature's physics to the spline position at the current progress
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

struct SCountBlock {
    long m_references;                                  // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4 per-type deleter, object in ecx
    void* m_pObject;                                    // +8
};
template <class T>
struct CCountedPointer {
    T* Data;                                            // +0
    SCountBlock* m_pCount;                              // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pCount && --m_pCount->m_references == 0) {
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
            operator delete(m_pCount);
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (m_pCount != other.m_pCount) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    operator delete(m_pCount);
                }
            }
            Data = other.Data;
            m_pCount = other.m_pCount;
            if (m_pCount) ++m_pCount->m_references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};
struct C3DVector { float X, Y, Z; };
struct CCardinalSpline {
    void GetSplinePosition(C3DVector* position, float progress);    // 0x00A567A0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& position);         // slot 31 (+0x7c)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
};
struct CEnginePrimitiveHandle;
struct CTCCreatureFlyOnSpline {
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
    PSpline->GetSplinePosition(&position, Progress);
    ParentThing->PhysicsTC->ResetPosition(position);
}
