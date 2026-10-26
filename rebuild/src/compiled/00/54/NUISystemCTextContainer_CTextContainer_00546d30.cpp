// NUISystem::CTextContainer::CTextContainer(long owner) -- CChangingStateComponent(owner), the three vtables of this class, then GetDataFromDef() fills Window from the def
// Leaf: ??0CTextContainer
// Retail: Fable.exe @ 0x00546D30, 46 bytes (+ int3 padding). Vtables 0x01248064 / 0x0124803C / 0x01248034 are
// NUISystem::CTextContainer (rtti-vtables.tsv).
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (manifest "CDraggableInto"),
// NUISystem::CTextContainer::GetDataFromDef @ 0x00546CD0 (on `this`, in the body).
// Shape: the landed CZoomingComponent constructor (0x00546B00) without member initialisers: base call, `mov ecx,esi`,
// the three vtable stores (+0, +4, +0x18), the body call. Bases modelled as in that constructor.
// Retail layout = Ego_r.pdb minus 0x30 (Window +0x15C; retail CChangingStateComponent 0x15C bytes, PDB 0x18C).
namespace NUISystem {

struct CComponentInterface {                       // primary vtable at +0
    virtual void v000();
};

struct CObserver_Retail {                          // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                       // vtable at +0x18
    virtual void v0();
};

struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
};

class CChangingStateComponent : public CComponentInterface, public CObserver_Retail, public CEventSinkInterface { public:   // retail 0x15C bytes
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long owner);           // 0x0052CC50
};

class CTextContainer : public CChangingStateComponent { public:  // vtables 0x01248064 / 0x0124803C / 0x01248034
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    C2DBoxF Window;                                // +0x15C
    CTextContainer(long owner);
    void GetDataFromDef();                         // 0x00546CD0
};

CTextContainer::CTextContainer(long owner)
    : CChangingStateComponent(owner)
{
    GetDataFromDef();
}

}