// CAIStateGroup_GoToBed::CAIStateGroup_GoToBed() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FEB6B, length 44 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255 (out of line), CBaseIntelligentPointer::
// CBaseIntelligentPointer @ 0x00A01B10 (the CIntelligentPointer<CThing> member's inlined constructor:
// base ctor + its vtable 0x01238C6C), CCharString::CCharString() @ 0x0099E4B0 for the name member at +0x30.
// Vtable 0x0128B104 = CAIStateGroup_GoToBed (rtti-vtables.tsv).
//
// Shape: base ctor, own vtable store, then the members in declaration order: the intelligent pointer
// PBed at +0x14 (constructed inline, 8 bytes: vptr + item), and a CCharString at +0x30 (constructed out
// of line). Members +0x1c..+0x2f (bed times, sleep offsets used by InitStateGroup / IsInSleepTime) are
// left uninitialised by the constructor. No engine header: CAIStateGroup_GoToBed.h does not exist, so
// the layout is modelled locally on CAIStateGroupBase (0x14 bytes).
struct CThing;

struct CAIStateGroupBase {
    virtual void v00();
    CAIStateGroupBase();                        // 0x008FD255
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04 the tracked object (set by the base ctor)
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }      // inlined: base ctor + vtable 0x01238C6C
};

struct CCharString {
    CCharString();                              // 0x0099E4B0
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    unsigned char _base[0x10];                  // +0x04 CAIStateGroupBase members (PBrain, priority, name, flags)
    CIntelligentPointer<CThing> PBed;           // +0x14
    long m_sleepTimeOffset;                     // +0x1c (uninitialised here)
    long m_bedTime;                             // +0x20 random 0..9 minutes (InitStateGroup)
    long m_wakeTime;                            // +0x24
    long m_pad28;                               // +0x28
    long m_state;                               // +0x2c (-1 in InitStateGroup)
    CCharString m_bedMarkerName;                // +0x30
    virtual void v00();
    CAIStateGroup_GoToBed();
};

CAIStateGroup_GoToBed::CAIStateGroup_GoToBed()
    : CAIStateGroupBase()
    , PBed()
    , m_bedMarkerName()
{
}