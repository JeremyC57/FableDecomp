// CAIStateGroup_CarryCrateAndFollow::CAIStateGroup_CarryCrateAndFollow() -- base ctor, own vtable, three CIntelligentPointer<CThing> members
// Leaf: ??0CAIStateGroup_CarryCrateAndFollow
// Retail 0x00901658, 0x3F bytes. Manifest label "CAIStateGroup_IceTrollSmash_Constructor" (_global) is wrong: the vtable
// stored at +0 is 0x0128CCD8, RTTI `.?AVCAIStateGroup_CarryCrateAndFollow@@`.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Member vtable: CIntelligentPointer<CThing> 0x01238C6C for all three members (loaded once into edi).
// Layout from engine/CAIStateGroup_CarryCrateAndFollow.h: PCrate +0x14, PPreviouslyCarriedItem +0x1c,
// PThingToFollow +0x24, GotActionPoint +0x2c, ActionPointIndex +0x30, PickUpActionID +0x34 (the scalars are not
// initialised by the constructor). Local model with the real inheritance; the header flattens the bases.
// Pragma: optimize("s") -- the size TU loads a vtable constant used twice or more into a callee-saved register.
#pragma optimize("s", on)

class CThing;

class CAIStateGroupBase { public:
    virtual void v00();
    unsigned char _base[0x10];                  // +0x04 PBrain, ScriptAIPriority, Name, Terminated/Priority/Enabled
    CAIStateGroupBase();                        // 0x008FD255
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    CIntelligentPointer<CThing> PCrate;                         // +0x14
    CIntelligentPointer<CThing> PPreviouslyCarriedItem;         // +0x1c
    CIntelligentPointer<CThing> PThingToFollow;                 // +0x24
    bool          GotActionPoint;                               // +0x2c
    long          ActionPointIndex;                             // +0x30
    long          PickUpActionID;                               // +0x34
    virtual void v00();
    CAIStateGroup_CarryCrateAndFollow();
};

CAIStateGroup_CarryCrateAndFollow::CAIStateGroup_CarryCrateAndFollow()
    : CAIStateGroupBase()
    , PCrate()
    , PPreviouslyCarriedItem()
    , PThingToFollow()
{
}