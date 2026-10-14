// CVtable_012fb778::CVtable_012fb778() -- default constructor of a CPSCBase-derived (particle system
// component) class whose vtable 0x012FB778 has no RTTI locator, so it is named by its vtable address.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F344C0, length 39 bytes.
// Manifest label "CAIStateGroup_HoverHornetQueen" is propagated. Callee: CPSCBase::CPSCBase @ 0x0042D9E0.
//
// Shape: base ctor, own vtable, then the embedded index block: six dwords (+0x28, +0x2c, +0x40, +0x54, +0x58, +0x60) set to
// -1 from one register (`or eax,-1`, hoisted above the vtable store): unset parameter/track indices of
// the component. The gaps (+0x30..+0x3c, +0x44..+0x50, +0x5c) are left uninitialised. Names descriptive.
struct CPSCBase {
    virtual void v00();
    CPSCBase();                                 // 0x0042D9E0
};

struct SComponentIndices {                      // embedded block with an inline constructor: that is what keeps
    long m_trackIndexA;                         // +0x28 = -1   the vtable store AHEAD of the member stores
    long m_trackIndexB;                         // +0x2c = -1
    unsigned char _pad1[0x10];                  // +0x30 (uninitialised)
    long m_paramIndexA;                         // +0x40 = -1
    unsigned char _pad2[0x10];                  // +0x44 (uninitialised)
    long m_paramIndexB;                         // +0x54 = -1
    long m_paramIndexC;                         // +0x58 = -1
    long m_pad5c;                               // +0x5c (uninitialised)
    long m_paramIndexD;                         // +0x60 = -1
    SComponentIndices() : m_trackIndexA(-1), m_trackIndexB(-1), m_paramIndexA(-1), m_paramIndexB(-1), m_paramIndexC(-1), m_paramIndexD(-1) { }
};

struct CVtable_012fb778 : CPSCBase {
    unsigned char _pad0[0x24];                  // +0x04 CPSCBase members, untouched here
    SComponentIndices m_indices;                // +0x28
    virtual void v00();
    CVtable_012fb778();
};

CVtable_012fb778::CVtable_012fb778()
    : CPSCBase()
    , m_indices()
{
}