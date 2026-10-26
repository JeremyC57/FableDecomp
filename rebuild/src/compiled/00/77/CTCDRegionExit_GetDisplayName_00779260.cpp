// CTCDRegionExit::GetDisplayName() const -- the display name of the region holding the map the connected entrance stands in (CRegion::GetDisplayName), or the shared empty name when no entrance is connected
//
// Retail 0x00779260, 100 bytes (plain `ret`; the region call is a tail `jmp`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (called twice: the null test, then the position), CTCBase's world-map accessor @ 0x00686D20
// (`[g_pMainGameComponent]+0x28`; manifest "CViewBase::GetDisplayEngine" is propagated), CThing::
// GetPosition @ 0x004C73D0, the world map's virtual slot 16 (+0x40, map number at a position),
// CWorldMap::GetRegionNumberMapIsIn(long) @ 0x004FC190, CWorldMap::GetRegion(long) @ 0x004FC180
// (0x58-byte regions), CRegion::GetDisplayName() const @ 0x006BBF20 (tail-jumped). The null result is the shared empty name 0x013BD804.
// Layout = Ego_r.pdb CTCDRegionExit: PEntranceConnectedTo (CIntelligentPointer<CThing>, pack 1) +0x2a.
// Shape: same body as the landed CTCActionUseScriptedHook::GetRegionEntranceName (0x00729C50): the first
// world-map fetch is a named local (its vtable is loaded before the argument), the two outer calls fetch
// the map again each time; the pointer member's address stays in ebx for both GetPItem calls.
class CBaseObject;
class C3DVector { public: float X, Y, Z; };
class CCharString { public: void* m_pData; };

extern CCharString g_EmptyName;                // 0x013BD804

class CThing { public:
    const C3DVector& GetPosition() const;      // 0x004C73D0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseObject* GetPItem() const;             // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CThing* operator->() const { return (CThing*)GetPItem(); }
};
#pragma pack(pop)

class CRegion { public:
    unsigned char _vectors[0x18];              // +0x00 Maps, VisMaps (12-byte retail vectors)
    CCharString Name;                          // +0x18
    const CCharString& GetDisplayName() const; // 0x006BBF20
};

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAtPosition(const C3DVector& position) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;   // 0x004FC190
    const CRegion& GetRegion(long region) const;         // 0x004FC180
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    const CWorldMap& PeekWorldMap() const;     // 0x00686D20
};

#pragma pack(push, 1)
class CTCDRegionExit : public CTCBase { public:
    bool                Active;                             // +0x0c
    bool                InLetterBoxMode;                    // +0x0d
    bool                WaitingForConfirmation;             // +0x0e
    bool                WaitingForActivationOrPlayerToLeave;// +0x0f
    bool                WaitingForPlayerToLeaveMessageArea; // +0x10
    unsigned char       _pad11[3];
    float               Radius;                             // +0x14
    float               MessageRadius;                      // +0x18
    unsigned char       _pad1c[4];
    unsigned __int64    EntranceConnectedToUID;             // +0x20
    bool                ReversedOnMiniMap;                  // +0x28
    bool                HiddenOnMiniMap;                    // +0x29
    CIntelligentPointer PEntranceConnectedTo;               // +0x2a
    const CCharString& GetDisplayName() const;
};
#pragma pack(pop)

const CCharString& CTCDRegionExit::GetDisplayName() const
{
    if (PEntranceConnectedTo.GetPItem())
    {
        const CWorldMap& worldMap = PeekWorldMap();
        long mapNumber = worldMap.GetMapNumberAtPosition(PEntranceConnectedTo->GetPosition());
        return PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(mapNumber)).GetDisplayName();
    }
    return g_EmptyName;
}