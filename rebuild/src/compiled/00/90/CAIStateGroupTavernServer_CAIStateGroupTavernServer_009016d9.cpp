// CAIStateGroup_TavernServer::CAIStateGroup_TavernServer() -- base ctor, own vtable, four CThingObject pointers and the pub pointer
// Leaf: ??0CAIStateGroup_TavernServer
// Retail 0x009016D9, 0x5B bytes. Manifest label "CAIStateGroup_MultiStateAttacker_Constructor" (_global) is wrong: the
// vtable stored at +0 is 0x0128CD14, RTTI `.?AVCAIStateGroup_TavernServer@@`.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Member vtables (RTTI): CIntelligentPointer<CThingObject> 0x0125A858 (+0x14/+0x1c/+0x24/+0x2c, held in edi),
// CIntelligentPointer<CThingBuilding> 0x01244344 (+0x34).
// Layout from engine/CAIStateGroup_TavernServer.h (PJug, PBar, PTable, PTankard, PPub, then indices / positions /
// right-handed sets left uninitialised). Local model with the real inheritance; the header flattens the bases.
// Pragma: optimize("s") -- the size TU loads a vtable constant used twice or more into a callee-saved register.
#pragma optimize("s", on)

struct CThingObject;
struct CThingBuilding;

struct CAIStateGroupBase {
    virtual void v00();
    unsigned char _base[0x10];                  // +0x04 PBrain, ScriptAIPriority, Name, Terminated/Priority/Enabled
    CAIStateGroupBase();                        // 0x008FD255
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThingObject>   PJug;                   // +0x14
    CIntelligentPointer<CThingObject>   PBar;                   // +0x1c
    CIntelligentPointer<CThingObject>   PTable;                 // +0x24
    CIntelligentPointer<CThingObject>   PTankard;               // +0x2c
    CIntelligentPointer<CThingBuilding> PPub;                   // +0x34
    long          JugIndex;                                     // +0x3c
    long          ServingActionPointIndex;                      // +0x40
    unsigned char _rest[0x58];                                  // +0x44 ServingPos .. AverageWalkSpeed (to 0x9c)
    virtual void v00();
    CAIStateGroup_TavernServer();
};

CAIStateGroup_TavernServer::CAIStateGroup_TavernServer()
    : CAIStateGroupBase()
    , PJug()
    , PBar()
    , PTable()
    , PTankard()
    , PPub()
{
}