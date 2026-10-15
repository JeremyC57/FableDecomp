// CTargetPoint::CTargetPoint() -- build the target-thing intelligent pointer and mark the point unindexed / never rejected
// Pragma: #pragma optimize("s", on) -- the `or dword ptr [..],-1` stores mark a size-optimised TU.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D7A76, length 26 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (PThing at +0; the inlined
// CIntelligentPointer<CThing> constructor then stores its vtable 0x01238C6C).
// Layout from Ego_r.pdb (CTargetPoint, 28 bytes, not polymorphic): PThing +0x00 (CIntelligentPointer<CThing>,
// 8 bytes), Index +0x08, TargetingPosition +0x0c (C3DVector, no constructor body), FrameRejected +0x18.
// Shape: the two `-1` initialisers are plain initialisers, so VC7.1 schedules their `or [..],-1` stores before
// the member's vtable store (wave 24i rule); the C3DVector is left uninitialised.
#pragma optimize("s", on)
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

struct CThing;

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_CThing() : CBaseIntelligentPointer() { }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
};

struct CTargetPoint {
    CIntelligentPointer_CThing PThing;           // +0x00
    long                       Index;            // +0x08
    C3DVector                  TargetingPosition;// +0x0c
    long                       FrameRejected;    // +0x18
    CTargetPoint();
};

CTargetPoint::CTargetPoint()
    : PThing(), Index(-1), FrameRejected(-1)
{
}
