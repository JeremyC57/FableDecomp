// CTCStealth::OnSetInLimbo(bool on) -- reset the region-based danger-thing limit; when switched off, take it from the current region's def (MaxNumAttackersOnHero) if the region has one
// Leaf: vfunc_16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747110, length 137 bytes (`ret 4`); CTCStealth vtable 0x012689EC slot 16 (a CTCBase bool
// notification, no catalog name).
// Callees: CTCBase::GetWorld() @ 0x00686A70 (`[[0x13B8A1C]+0x24]`; manifest label "CViewBase::GetDisplayEngine" is a byte twin) twice,
// CWorld virtual slot 12 GetWorldMap (+0x30) twice, CWorldMap::GetCurrentRegionNumber @ 0x004FB150 (manifest "CWorld::DrawGetEnvironment"
// propagated), CWorldMap::GetRegion @ 0x004FB490 (manifest "CTCInventoryBase::GetCategoryWithIndex" propagated), the region def's virtual
// slot 1 (destroy) when a copied CDefPointer drops the last reference.
// Layout: CTCStealth RegionBasedMaxNumberOfDangerThings +0x48 (retail = PDB). CWorldMap::CRegion PDef at retail +0x24 (PDB +0x2C, two
// 12-byte vectors); CRegionDef MaxNumAttackersOnHero at retail +0x98 (PDB +0xA4; retail CSoundMap is 12 bytes smaller, as the landed
// CGameScriptInterface rows).
// Shape: `GetWorld()->GetWorldMap()->GetRegion(GetWorld()->GetWorldMap()->GetCurrentRegionNumber())` (both GetWorld calls first); the
// def is copied twice: a by-value `HasDef()` style test (copy, null test, release with its null test) and a copy dereferenced for the
// value whose release has no null test.
class CRegionDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x98 - 0x08];
    long MaxNumAttackersOnHero;                 // +0x98 (PDB +0xA4)
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
template<class T> struct CDefPointerTest {      // the null-safe release of the presence test
    T* Object;
    CDefPointerTest(const CDefPointer<T>& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointerTest() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
class CRegion { public:                                // CWorldMap::CRegion
    unsigned char _pad00[0x24];
    CDefPointer<CRegionDef> PDef;               // +0x24 (PDB +0x2C)
};
class CWorldMap { public:
    long GetCurrentRegionNumber();              // 0x004FB150
public:
    CRegion& GetRegion(long);   // 0x004FB490
};
class CWorld { public:
    virtual void w000(); virtual void w001(); virtual void w002(); virtual void w003(); virtual void w004(); virtual void w005();
    virtual void w006(); virtual void w007(); virtual void w008(); virtual void w009(); virtual void w010(); virtual void w011();
    virtual CWorldMap* GetWorldMap();           // slot 12 (+0x30)
};
class CThing;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCStealth : public CTCBase { public:
    unsigned char _pad0c[0x48 - 0x0c];
    long RegionBasedMaxNumberOfDangerThings;    // +0x48
    void OnSetInLimbo(bool on);
};

void CTCStealth::OnSetInLimbo(bool on)
{
    RegionBasedMaxNumberOfDangerThings = -1;
    if (!on) {
        CRegion& region = GetWorld().GetWorldMap()->GetRegion(GetWorld().GetWorldMap()->GetCurrentRegionNumber());
        if (CDefPointerTest<CRegionDef>(region.PDef).Object != 0)
            RegionBasedMaxNumberOfDangerThings = CDefPointer<CRegionDef>(region.PDef)->MaxNumAttackersOnHero;
    }
}