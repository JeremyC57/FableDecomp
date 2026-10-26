// NUISystem::CZoomingComponent::CZoomingComponent(long owner) -- CChangingStateComponent(owner), the three vtables of this class, InputDelay and ScrollingSpeed cleared, then GetDataFromDef()
// Retail: Fable.exe (TLC, VC7.1) @ 0x00546B00, 60 bytes. Manifest label "CScrollBarOutside::CScrollBarOutside"
// is propagated from the byte-similar shell; the vtables 0x01247DC4 / 0x01247DA0 / 0x01247D98 are
// NUISystem::CZoomingComponent (rtti-vtables.tsv).
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (as in the landed
// CParticleEffect 0x00544010; manifest "CDraggableInto"), GetDataFromDef @ 0x00546AA0 (on `this`, in the
// body; manifest "CTextContainer::GetDataFromDef" is propagated).
//
// Shape: base constructor, `xor eax,eax ; mov ecx,esi`, the three vtable stores, two zero stores from the
// shared register (the float zeroes of an init list share the integer zero register here, like the
// landed CDraggable's PositionBeforeDrag), then the body call. LastInputTime, the three C2DVectors and the
// two flags are left to GetDataFromDef.
// Retail layout = Ego_r.pdb minus 0x30 (retail CChangingStateComponent 0x15C bytes, PDB 0x18C):
// LastInputTime +0x15C, ZoomCenter +0x160, MinZoom +0x168, MaxZoom +0x170, Panning +0x178, Zooming +0x179,
// InputDelay +0x17C, ScrollingSpeed +0x180. Bases modelled as in the landed CParticleEffect constructor.
// Leaf: ??0CZoomingComponent

namespace NUISystem {

struct CComponentInterface {                     // primary vtable at +0
    virtual void v000();
};

struct CObserver_Retail {                        // vtable at +4, then its 0x10 bytes of state
    virtual void ProcessEvent();
    unsigned char _body[0x10];
};

struct CEventSinkInterface {                     // vtable at +0x18
    virtual void v0();
};

struct C2DVector {
    float X, Y;
};

class CChangingStateComponent : public CComponentInterface, public CObserver_Retail, public CEventSinkInterface { public:   // retail 0x15C bytes
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long owner);         // 0x0052CC50
};

class CZoomingComponent : public CChangingStateComponent { public:   // vtables 0x01247DC4 / 0x01247DA0 / 0x01247D98
    virtual void v000();
    virtual void ProcessEvent();
    virtual void v0();
    float     LastInputTime;                     // +0x15C
    C2DVector ZoomCenter;                        // +0x160
    C2DVector MinZoom;                           // +0x168
    C2DVector MaxZoom;                           // +0x170
    bool      Panning;                           // +0x178
    bool      Zooming;                           // +0x179
    float     InputDelay;                        // +0x17C
    float     ScrollingSpeed;                    // +0x180
    CZoomingComponent(long owner);
    void GetDataFromDef();                       // 0x00546AA0
};

CZoomingComponent::CZoomingComponent(long owner)
    : CChangingStateComponent(owner)
    , InputDelay(0.0f)
    , ScrollingSpeed(0.0f)
{
    GetDataFromDef();
}

}