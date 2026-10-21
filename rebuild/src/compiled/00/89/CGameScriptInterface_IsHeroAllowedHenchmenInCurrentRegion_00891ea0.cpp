// CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion() const -- the loaded region's def AllowHenchmen flag (true when the region has no def)
// Leaf: ?IsHeroAllowedHenchmenInCurrentRegion@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe @ 0x00891EA0, 77 bytes, plain `ret`; Ego_r.pdb ?IsHeroAllowedHenchmenInCurrentRegion@CGameScriptInterface@@UBE_NXZ.
// Body: World (+0x04)'s world map (CWorld virtual slot 13, +0x34, called twice) -> GetRegion(GetLoadedRegion())
// (0x004FC180 / 0x004FB150, Ego_r.pdb ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z / ?GetLoadedRegion@CWorldMap@@QBEJXZ);
// the region's PDef (+0x24, CDefPointer<const CRegionDef>) copied (IVRefCount +4 up), AllowHenchmen (+0x89) read, the copy
// released (IVRefCount down, at zero the def's virtual slot 1); no def -> true.
// Layout: retail CRegion (0x58): PDef +0x24 (Ego_r.pdb +0x2c); CRegionDef AllowHenchmen +0x89 (Ego_r.pdb +0x95); the
// world-map accessor's name is not recovered (slot 13 of CWorld). CGameScriptInterface World +0x04 (engine/CGameScriptInterface.h).
// Shape: /O2 (16-aligned entry); the region bound to a reference, PDef null-tested before the counted copy; AddRef / Release
// as CResource inline members (the def is a CResource: IVRefCount +4, slot 1 frees it).
class CResource {
public:
    virtual ~CResource();
    virtual void DeleteThis();                           // slot 1 (+0x04)
    unsigned long IVRefCount;                            // +0x04
    void AddRef() { ++IVRefCount; }
    void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
class CRegionDef : public CResource {
public:
    unsigned char _pad08[0x89 - 8];                      // CResource tail (+0x08..+0x1b) and the def fields before AllowHenchmen
    bool AllowHenchmen;                                  // +0x89
};
template <class T> class CDefPointer {
public:
    T* Data;
    CDefPointer(const CDefPointer& other) : Data(other.Data) { if (Data) const_cast<CResource*>(static_cast<const CResource*>(Data))->AddRef(); }
    ~CDefPointer() { if (Data) const_cast<CResource*>(static_cast<const CResource*>(Data))->Release(); }
    T* operator->() const { return Data; }
    operator bool() const { return Data != 0; }
};
class CRegion {
public:
    unsigned char                  _pad00[0x24];
    CDefPointer<const CRegionDef>  PDef;                 // +0x24
};
class CWorldMap {
public:
    CRegion& GetRegion(long index);                      // 0x004FC180
    long GetLoadedRegion() const;                        // 0x004FB150
};
class CWorld {
public:
    virtual void _slot00(); virtual void _slot01(); virtual void _slot02(); virtual void _slot03();
    virtual void _slot04(); virtual void _slot05(); virtual void _slot06(); virtual void _slot07();
    virtual void _slot08(); virtual void _slot09(); virtual void _slot10(); virtual void _slot11();
    virtual void _slot12();
    virtual CWorldMap* GetWorldMap();                    // slot 13 (+0x34)
};
class CGameScriptInterface {
public:
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const;
    CWorld* World;                                       // +0x04
};
bool CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion() const
{
    const CRegion& region = World->GetWorldMap()->GetRegion(World->GetWorldMap()->GetLoadedRegion());
    if (region.PDef)
    {
        CDefPointer<const CRegionDef> def = region.PDef;
        return def->AllowHenchmen;
    }
    return true;
}