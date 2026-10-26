// CAIStateGroup_TavernKeeper::CAIStateGroup_TavernKeeper() -- base ctor, own vtable, jug/bar/pub/payee intelligent pointers
// Leaf: ??0CAIStateGroup_TavernKeeper
// Retail 0x00901783, 0x53 bytes. Manifest label "CAIStateGroup_WerewolfAttacker_Constructor" (_global) is wrong: the
// vtable stored at +0 is 0x0128CD50, RTTI `.?AVCAIStateGroup_TavernKeeper@@`.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Member vtables (RTTI): CIntelligentPointer<CThingObject> 0x0125A858 (+0x14, +0x1c, held in ebx),
// CIntelligentPointer<CThingBuilding> 0x01244344 (+0x24), CIntelligentPointer<CThingAICreature> 0x0123E7E0 (+0x2c).
// Layout from engine/CAIStateGroup_TavernKeeper.h (PJug, PBar, PPub, PPayee; the jug slot, position, walk speed and
// payment message are left uninitialised). Local model with the real inheritance; the header flattens the bases.
// Pragma: optimize("s") -- the size TU loads a vtable constant used twice or more into a callee-saved register.
#pragma optimize("s", on)

class CThingObject;
class CThingBuilding;
class CThingAICreature;
class CMessageEvent;

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

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CIntelligentPointer<CThingObject>     PJug;                 // +0x14
    CIntelligentPointer<CThingObject>     PBar;                 // +0x1c
    CIntelligentPointer<CThingBuilding>   PPub;                 // +0x24
    CIntelligentPointer<CThingAICreature> PPayee;               // +0x2c
    long           JugActionPointIndex;                         // +0x34
    float          JugPos[3];                                   // +0x38 C3DVector
    unsigned char  JugRHSet[0x18];                              // +0x44 CRightHandedSet
    long           JugSlot;                                     // +0x5c
    float          AverageWalkSpeed;                            // +0x60
    CMessageEvent* PaymentRequestMessage;                       // +0x64
    virtual void v00();
    CAIStateGroup_TavernKeeper();
};

CAIStateGroup_TavernKeeper::CAIStateGroup_TavernKeeper()
    : CAIStateGroupBase()
    , PJug()
    , PBar()
    , PPub()
    , PPayee()
{
}