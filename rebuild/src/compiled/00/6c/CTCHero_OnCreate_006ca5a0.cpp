// CTCHero::OnCreate() -- set the hero's weapon and will usability from the def of the region the parent thing stands in (usable unless the region prevents it); usable when the region has no def
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CA5A0, 171 bytes (plain `ret`). Vtable 0x0125EB54 (CTCHero, rtti-vtables.tsv) slot 4.
// Callees: CTCBase::GetWorldMap @ 0x00686D30 (three times), CThing::GetPosition() const @ 0x004C73D0, CWorldMap::GetMapNumberAt
// (virtual slot 16, +0x40, as the landed 0x006AEBF0), CWorldMap::GetRegionNumberMapIsIn(long) @ 0x004FC190, CWorldMap::GetRegion(long)
// @ 0x004FB490 (manifest "CTCInventoryBase::GetCategoryWithIndex", propagated), CTCHero::SetWeaponsAsUsable(bool) @ 0x006CA380; the
// region def's virtual slot 1 on the last release.
// Layout: CTCHero (Ego_r.pdb matches retail, as the landed 0x006CA4E0): WeaponsUsable +0x0C, WillUsable +0x0D; the 0x58-byte
// region record's def pointer +0x24 (PDB 0x2C); CRegionDef PreventHeroWeaponUse +0x70 / PreventHeroWillUse +0x71 (PDB 0x7C/0x7D:
// the retail CSoundMap is 12 bytes smaller), as the landed CGameScriptInterface::SetHeroWeaponsAsUsable 0x00898410.
// Shape: each def access is an inline by-value getter (null-tested add-ref; null-tested destructor calling the def's own Release --
// VC7.1 drops the destructor's null test after the `->` dereference, and folds the first temporary's tests into the `if`). The
// condition reads `.Object` straight (an IsNull()/bool helper materialises a `setne bl`); with the release written inline in the
// destructor the folded count copy lands in edi instead of edx.
class C3DVector { public: float X, Y, Z; };

class CThing {
public:
    const C3DVector& GetPosition() const;                // 0x004C73D0
};

class CRegionDef { public:
    virtual void v00();
    virtual void Destroy();                              // slot 1
    long          m_refCount;                            // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
    unsigned char _pad08[0x70 - 0x08];
    bool          PreventHeroWeaponUse;                  // +0x70
    bool          PreventHeroWillUse;                    // +0x71
};

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};

class CRegion { public:                                         // 0x58-byte world-map region record
    unsigned char           _pad00[0x24];
    CDefPointer<CRegionDef> PDef;                        // +0x24
    CDefPointer<CRegionDef> GetDef() const { return PDef; }
};

class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& position);   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);               // 0x004FC190
    CRegion& GetRegion_004fb490(long region);                     // 0x004FB490
};

class CTCHero {
public:
    void*   __vfptr;                                     // +0x00
    CThing* ParentThing;                                 // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool    WeaponsUsable;                               // +0x0C
    bool    WillUsable;                                  // +0x0D
    CWorldMap* GetWorldMap();                            // 0x00686D30 (CTCBase)
    void SetWeaponsAsUsable(bool usable);                // 0x006CA380
    void OnCreate();
};

void CTCHero::OnCreate()
{
    CRegion& region = GetWorldMap()->GetRegion_004fb490(GetWorldMap()->GetRegionNumberMapIsIn(GetWorldMap()->GetMapNumberAt(ParentThing->GetPosition())));
    if (region.GetDef().Object) {
        SetWeaponsAsUsable(!region.GetDef()->PreventHeroWeaponUse);
        WillUsable = !region.GetDef()->PreventHeroWillUse;
    } else {
        WeaponsUsable = true;
        WillUsable = true;
    }
}