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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CThing;

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

struct CTargetPoint {
    CIntelligentPointer<CThing> PThing;           // +0x00
    long                       Index;            // +0x08
    C3DVector                  TargetingPosition;// +0x0c
    long                       FrameRejected;    // +0x18
    CTargetPoint();
};

CTargetPoint::CTargetPoint()
    : PThing(), Index(-1), FrameRejected(-1)
{
}