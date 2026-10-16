// CTCPreCalculatedNavigationRoute::OnInitialActivate() -- when a target UID was saved, resolve it to the thing and point PThingToCalculateRouteTo at it
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00800460, 97 bytes. CTCPreCalculatedNavigationRoute vtable 0x01242444 slot 6 (OnInitialActivate).
// Callees: CTCBase::GetThingManager @ 0x00686AE0, CThingManager::GetThingByUID(unsigned __int64) @ 0x0051ED80 (catalog
//   "CStopWatch::GetTicks" in older rows), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 /
//   dtor 0x00A01C10; the temporary's vtable 0x01237F1C is CIntelligentPointer<CThing const>.
// Layout: PDB CTCPreCalculatedNavigationRoute: PThingToCalculateRouteTo +0x0c, ThingToCalculateRouteToUID +0x18 (ulong64), Route +0x20.
// Shape: `if (uid)` on the 64-bit value (`or edx,ecx ; je`); `P = CIntelligentPointer<const CThing>(GetThingManager()->
//   GetThingByUID(uid))` -- the UID pushes precede the manager call, the temporary is built in the frame, assigned through the
//   inline operator= (SetPItem(GetPItem())) and destroyed.
struct CThing;
struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CThingManager {
    CThing* GetThingByUID(unsigned __int64 uid);   // 0x0051ED80
};
struct CTCPreCalculatedNavigationRoute {
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CIntelligentPointer<const CThing> PThingToCalculateRouteTo;   // +0x0c
    unsigned __int64 ThingToCalculateRouteToUID;// +0x18
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void OnInitialActivate();
};

void CTCPreCalculatedNavigationRoute::OnInitialActivate()
{
    if (ThingToCalculateRouteToUID)
        PThingToCalculateRouteTo = CIntelligentPointer<const CThing>(GetThingManager()->GetThingByUID(ThingToCalculateRouteToUID));
}