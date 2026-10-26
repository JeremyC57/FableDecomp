// CTCDRegionExit::SetUpPEntranceConnectedToFromUID() -- resolve the saved connected-entrance UID to its thing; when that thing is not alive and the game allows it, forget a UID whose map is unknown, unused or already loaded
// Leaf: SetUpPEntranceConnectedToFromUID
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779340, length 224 bytes. Called only by the tail jump in
// CTCDRegionExit::OnInitialActivate (vtable 0x0126AB54 slot 6, 0x00779420: `if (EntranceConnectedToUID) ...`).
// Label correction: the catalog class "CTCActionUseScriptedHook" is wrong -- the UID at +0x20 and the unaligned
// intelligent pointer at +0x2A are Ego_r.pdb CTCDRegionExit (EntranceConnectedToUID +0x20, PEntranceConnectedTo +0x2A).
// Callees: CTCBase::GetThingManager @ 0x00686AE0, CThingManager::GetThingByUID(unsigned __int64) @ 0x0051ED80,
// CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (the temporary's
// vtable 0x01238C6C is CIntelligentPointer<CThing>), the CTCBase subsystem accessor @ 0x00686D60 (virtual slot 9, bool),
// __aullshr @ 0x00BFECF0, CTCBase::GetWorldMap @ 0x00686D30, CWorldMap::GetMapNumberFromMapUID @ 0x004FB920 and the
// CWorldMap virtuals IsMapLoaded (slot 10, +0x28) / IsMapUsed (slot 11, +0x2C).
// Shape: the shape of CTCActionUseScriptedHook::ResolveEntranceConnectedTo (0x00729BC0) with the member address formed
// AFTER the temporary's GetPItem (the intelligent-pointer operator= reads the item into a local first); the map UID is
// the top 24 bits of the thing UID (`>> 40`); the zero register for the clear doubles as the map-number test.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
#pragma pack(push, 1)
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for <CThing>
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        T* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
#pragma pack(pop)
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

class CThingManager { public:
    CThing* GetThingByUID(unsigned __int64 uid);   // 0x0051ED80
};
struct CGameSubsystem {                         // [0x013B8A1C]+0x1C, not identified (landed CTCCrateStack 0x007B6A80)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsSuppressed();                // slot 9 (+0x24)
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long mapNumber) const;   // slot 10 (+0x28) 0x004FC050
    virtual bool IsMapUsed(long mapNumber) const;     // slot 11 (+0x2C) 0x0051CE00
    long GetMapNumberFromMapUID(unsigned long uid) const;   // 0x004FB920
};

class CTCDRegionExit {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    bool    Active;                             // +0x0c
    bool    InLetterBoxMode;                    // +0x0d
    bool    WaitingForConfirmation;             // +0x0e
    bool    WaitingForActivationOrPlayerToLeave;   // +0x0f
    bool    WaitingForPlayerToLeaveMessageArea;    // +0x10
    float   Radius;                             // +0x14
    float   MessageRadius;                      // +0x18
    unsigned __int64 EntranceConnectedToUID;    // +0x20
    bool    ReversedOnMiniMap;                  // +0x28
    bool    HiddenOnMiniMap;                    // +0x29
    CIntelligentPointer<CThing> PEntranceConnectedTo;   // +0x2a
    CThingManager*  GetThingManager();          // 0x00686AE0 (CTCBase)
    CGameSubsystem* GetGameSubsystem();         // 0x00686D60 (CTCBase)
    CWorldMap*      GetWorldMap();              // 0x00686D30 (CTCBase)
    void SetUpPEntranceConnectedToFromUID();
};

void CTCDRegionExit::SetUpPEntranceConnectedToFromUID()
{
    PEntranceConnectedTo = CIntelligentPointer<CThing>(GetThingManager()->GetThingByUID(EntranceConnectedToUID));
    if (IsThingAlive(PEntranceConnectedTo))
        return;
    if (!GetGameSubsystem()->IsSuppressed())
        return;
    unsigned __int64 mapUID = EntranceConnectedToUID >> 40;
    long mapNumber = GetWorldMap()->GetMapNumberFromMapUID((unsigned long)mapUID);
    if (mapNumber == 0)
        EntranceConnectedToUID = 0;
    else if (!GetWorldMap()->IsMapUsed(mapNumber) || GetWorldMap()->IsMapLoaded(mapNumber))
        EntranceConnectedToUID = 0;
}