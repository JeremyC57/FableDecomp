// CAIStateGroup_FlyingThingBase::CAIStateGroup_FlyingThingBase() -- default constructor.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009126C0, length 57 bytes.
// Callees: CBase::CBase @ 0x0099A2D0 and CCharString::CCharString() @ 0x0099E4B0, both reached through the
// INLINED CAIStateGroupBase constructor (base CBase ctor, CAIStateGroupBase vtable 0x01288C60, then the Name
// member at +0x0c -- the same three steps as the out-of-line copy @ 0x008FD255). Own vtable 0x0128F9BC =
// CAIStateGroup_FlyingThingBase (rtti-vtables.tsv).
//
// Shape: after the inlined base constructor the derived vtable is stored and four float members are
// initialised: m_14 = 0, m_18 = 2.0f, m_1c = 10.0f, m_20 = 10.0f. The retail scheduler hoists the shared
// 10.0f constant (one `mov eax` for +0x1c/+0x20) above the vtable store; the source is a plain initialiser
// list in declaration order. No engine header exists for this class; the layout is modelled locally on
// CAIStateGroupBase (0x14 bytes).
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2D0
}; } using N_0099a2d0::CBase;

class CCharString { public:
    CCharString();                              // 0x0099E4B0
};

class CAIStateGroupBase : public CBase { public:
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    CCharString Name;                           // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    virtual void v00();
    CAIStateGroupBase() : CBase(), Name() { }   // inlined here (out-of-line copy @ 0x008FD255)
};

struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    float m_heightAboveGround;                  // +0x14 = 0
    float m_speed;                              // +0x18 = 2.0f
    float m_minHeight;                          // +0x1c = 10.0f
    float m_maxHeight;                          // +0x20 = 10.0f
    virtual void v00();
    CAIStateGroup_FlyingThingBase();
};

CAIStateGroup_FlyingThingBase::CAIStateGroup_FlyingThingBase()
    : CAIStateGroupBase()
    , m_heightAboveGround(0.0f)
    , m_speed(2.0f)
    , m_minHeight(10.0f)
    , m_maxHeight(10.0f)
{
}