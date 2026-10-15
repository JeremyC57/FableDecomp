// CTCActionUseScriptedHook::GetRegionEntranceName() const -- the name of the region holding the map that the connected entrance's position lies in
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729C50, length 72 bytes.
// Leaf: GetRegionEntranceName
// Callees: CTCBase's world-map accessor @ 0x00686D20 (`[g_pMainGameComponent]+0x28`, called three times; the manifest's
// "CViewBase::GetDisplayEngine" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @
// 0x004C73D0 (`[this+60h]+0Ch`, the physics component's position, as the landed 0x00943AE0 names it), the world map's
// virtual slot 16 (+0x40, map number at a position), CWorldMap::GetRegionNumberMapIsIn(long) @ 0x004FC190 and
// CWorldMap::GetRegion(long) @ 0x004FC180 (0x58-byte regions). Ego_r.pdb: `?GetRegionEntranceName@CTCActionUseScriptedHook
// @@QBEABVCCharString@@XZ`, PEntranceConnectedTo +0x20; CRegion::Name is +0x18 in retail (two 12-byte vectors before it;
// engine/CRegion.h has the PDB's 16-byte vectors, Name +0x20, so CRegion is modelled locally).
// Shape: the first world-map fetch (and its vtable, kept in ebx) happens BEFORE the argument is computed, so the map
// is a named local; the outer two calls evaluate their argument first and fetch the map again each time.
struct CBaseObject;
struct C3DVector { float X, Y, Z; };
struct CCharString { void* m_pData; };

struct CThing {
    const C3DVector& GetPosition() const;      // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                      // +0x04
    CBaseObject* GetPItem() const;             // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CThing* operator->() const { return (CThing*)GetPItem(); }
};

struct CRegion {
    unsigned char _vectors[0x18];              // +0x00 Maps, VisMaps (12-byte retail vectors)
    CCharString Name;                          // +0x18
};

struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAtPosition(const C3DVector& position) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;   // 0x004FC190
    const CRegion& GetRegion(long region) const;         // 0x004FC180
};

struct CTCBase {                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    const CWorldMap& PeekWorldMap() const;     // 0x00686D20
};

struct CTCActionUseScriptedHook : CTCBase {
    unsigned char _pad0c[0x14];                // +0x0c Usable .. +0x18 EntranceConnectedToUID
    CIntelligentPointer PEntranceConnectedTo;  // +0x20
    const CCharString& GetRegionEntranceName() const;
};

const CCharString& CTCActionUseScriptedHook::GetRegionEntranceName() const
{
    const CWorldMap& worldMap = PeekWorldMap();
    long mapNumber = worldMap.GetMapNumberAtPosition(PEntranceConnectedTo->GetPosition());
    return PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(mapNumber)).Name;
}
