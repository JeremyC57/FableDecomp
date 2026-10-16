// CTCActionUseScriptedHook::ResolveEntranceConnectedTo() -- resolve the saved connected-entrance UID to its thing and point PEntranceConnectedTo at it (then touch the pointer once more when it is set)
// Grade: DIFFER(ours v retail) -- 111v111 scheduling only: ours forms the member address (add esi,20h) before the temporary's GetPItem call, retail right after it (same as parked 0x007446B0); tried a member-reference local
// Leaf: ResolveEntranceConnectedTo
// Retail: Fable.exe (TLC, VC7.1) @ 0x00729BC0, 111 bytes. Catalog: "CTCPreCalculatedNavigationRoute::OnInitialActivate"
//   (propagated, wrong): the only caller is CTCActionUseScriptedHook::OnInitialActivate (vtable 0x0126664C slot 6, 0x00729F60,
//   call at 0x0072A023), the body sits among CTCActionUseScriptedHook members (0x00729A90 ctor .. 0x00729CF0 OnSerialise) and
//   reads its PDB members; the name is descriptive.
// Callees: CTCBase::GetThingManager @ 0x00686AE0, CThingManager::GetThingByUID(unsigned __int64) @ 0x0051ED80,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10; the temporary's vtable
//   0x01238C6C is CIntelligentPointer<CThing>.
// Layout: PDB CTCActionUseScriptedHook: EntranceConnectedToUID +0x18 (ulong64), PEntranceConnectedTo +0x20.
// Shape: the shape of CTCPreCalculatedNavigationRoute::OnInitialActivate (0x00800460) without the UID test; the closing
//   GetPItem pair is an inline whose remaining work is the second GetPItem call (its result unused).
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
struct CTCActionUseScriptedHook {
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    unsigned char _pad0a[2];
    bool    Usable;                             // +0x0c
    bool    ForceConfirmation;                  // +0x0d
    bool    WaitingForConfirmation;             // +0x0e
    bool    PerformAction;                      // +0x0f
    bool    TeleportToRegionEntrance;           // +0x10
    unsigned __int64 EntranceConnectedToUID;    // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x20
    CThingManager* GetThingManager();           // 0x00686AE0 (CTCBase)
    void ResolveEntranceConnectedTo();
};

void CTCActionUseScriptedHook::ResolveEntranceConnectedTo()
{
    PEntranceConnectedTo = CIntelligentPointer<CThing>(GetThingManager()->GetThingByUID(EntranceConnectedToUID));
    if (PEntranceConnectedTo.GetPItem())
        PEntranceConnectedTo.GetPItem();
}
