// CTCPreCalculatedNavigationRoute::GetRouteDestination() -- the position of the thing the route is calculated to, by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008004D0, length 39 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() @ 0x00A01B50 (on the +0x0c pointer, through the typed `operator->`),
// CThing::GetPosition() @ 0x004C73D0 (returns a const reference; the manifest's "CVertexBufferWin32::DoSizeof" label is
// propagated, the landed GetTrackPosition 0x0075C1E0 names it).
// Shape: `add ecx,0Ch` addresses the member pointer as `this` for GetPItem, its result goes straight into ecx for GetPosition,
// and the returned reference is copied into the hidden result through C3DVector's memberwise user copy constructor (result
// pointer in ecx, handed back with `mov eax,ecx`; `ret 4`).
// Layout = Ego_r.pdb CTCPreCalculatedNavigationRoute: PThingToCalculateRouteTo (CIntelligentPointer<const CThing>) +0x0c.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CThing {
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

struct CBaseObject;
#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* PData;                                            // +0x04
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template<class T>
struct CIntelligentPointer : CBaseIntelligentPointer {
    T* operator->() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

struct CTCPreCalculatedNavigationRoute {
    virtual void v00();
    void* ParentThing;                                      // +0x04 (CTCBase)
    bool  ToBeRemoved;                                      // +0x08
    bool  IsInGlobalUpdateTCs;                              // +0x09
    unsigned char _pad0a[2];
    CIntelligentPointer<const CThing> PThingToCalculateRouteTo;   // +0x0c
    C3DVector GetRouteDestination() const;
};

C3DVector CTCPreCalculatedNavigationRoute::GetRouteDestination() const
{
    return PThingToCalculateRouteTo->GetPosition();
}