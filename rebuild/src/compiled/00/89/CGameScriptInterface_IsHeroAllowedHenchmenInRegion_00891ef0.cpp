// CGameScriptInterface::IsHeroAllowedHenchmenInRegion(const CCharString& region_name) const -- the named region's def AllowHenchmen flag (true for no region / no def)
// Leaf: ?IsHeroAllowedHenchmenInRegion@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe @ 0x00891EF0, 90 bytes, `ret 4`; Ego_r.pdb ?IsHeroAllowedHenchmenInRegion@CGameScriptInterface@@UBE_NABVCCharString@@@Z.
// Body: World (+0x04)'s world map (CWorld virtual slot 13, called twice) -> GetRegionNumberFromName(name) (0x004FC210,
// Ego_r.pdb ?GetRegionNumberFromName@CWorldMap@@QBEJABVCCharString@@@Z); index <= 0 -> true; else GetRegion(index)
// (0x004FC180) and, as in IsHeroAllowedHenchmenInCurrentRegion 0x00891EA0, the region's PDef copied, AllowHenchmen (+0x89)
// read, the copy released; no def -> true.
// Layout: as 0x00891EA0: retail CRegion PDef +0x24, CRegionDef AllowHenchmen +0x89, CGameScriptInterface World +0x04.
// Shape: /O2 (16-aligned entry).
class CCharString;
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
    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210
public:
    const CRegion& GetRegion(long) const;   // 0x004FC180
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
    virtual bool IsHeroAllowedHenchmenInRegion(const CCharString& region_name) const;
    CWorld* World;                                       // +0x04
};
bool CGameScriptInterface::IsHeroAllowedHenchmenInRegion(const CCharString& region_name) const
{
    long index = World->GetWorldMap()->GetRegionNumberFromName(region_name);
    if (index > 0)
    {
        const CRegion& region = World->GetWorldMap()->GetRegion(index);
        if (region.PDef)
        {
            CDefPointer<const CRegionDef> def = region.PDef;
            return def->AllowHenchmen;
        }
    }
    return true;
}