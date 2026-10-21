#include <cstdio>
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
    CRegion& GetRegion(long index);                      // 0x004FC180
    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210
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



static int g_deleted;
CResource::~CResource() {}
void CResource::DeleteThis() { ++g_deleted; }
static unsigned char g_raw[3 * sizeof(CRegion)]; static CRegion* const g_regions = (CRegion*)g_raw; 
CRegion& CWorldMap::GetRegion(long index) { return g_regions[index]; }
class CCharString { public: long Index; };
long CWorldMap::GetRegionNumberFromName(const CCharString& name) const { return name.Index; }
static CWorldMap g_map;
void CWorld::_slot00() {} void CWorld::_slot01() {} void CWorld::_slot02() {} void CWorld::_slot03() {}
void CWorld::_slot04() {} void CWorld::_slot05() {} void CWorld::_slot06() {} void CWorld::_slot07() {}
void CWorld::_slot08() {} void CWorld::_slot09() {} void CWorld::_slot10() {} void CWorld::_slot11() {}
void CWorld::_slot12() {}
CWorldMap* CWorld::GetWorldMap() { return 0; }
class CTestWorld : public CWorld {
public:
    virtual void _slot00() {} virtual void _slot01() {} virtual void _slot02() {} virtual void _slot03() {}
    virtual void _slot04() {} virtual void _slot05() {} virtual void _slot06() {} virtual void _slot07() {}
    virtual void _slot08() {} virtual void _slot09() {} virtual void _slot10() {} virtual void _slot11() {}
    virtual void _slot12() {}
    virtual CWorldMap* GetWorldMap() { return &g_map; }
};
class CTestDef : public CRegionDef {};
CTestDef* MakeDef(bool allow, unsigned long rc) { CTestDef* d = new CTestDef; d->AllowHenchmen = allow; d->IVRefCount = rc; return d; }
int main()
{
    CTestWorld world; CGameScriptInterface gsi; gsi.World = &world;
    CTestDef* yes = MakeDef(true, 1); CTestDef* no = MakeDef(false, 0);
    g_regions[2].PDef.Data = no;
    CCharString n0 = {0}, n1 = {1}, n2 = {2}, nm = {-1};
    g_regions[0].PDef.Data = yes;
    bool ok = gsi.IsHeroAllowedHenchmenInRegion(n0) && gsi.IsHeroAllowedHenchmenInRegion(nm) && yes->IVRefCount == 1;
    g_regions[1].PDef.Data = 0; ok = ok && gsi.IsHeroAllowedHenchmenInRegion(n1);
    g_regions[1].PDef.Data = yes; ok = ok && gsi.IsHeroAllowedHenchmenInRegion(n1) && yes->IVRefCount == 1 && g_deleted == 0;
    ok = ok && !gsi.IsHeroAllowedHenchmenInRegion(n2) && no->IVRefCount == 0 && g_deleted == 1;
    if (ok) std::printf("00891ef0_TEST PASS\n");
    return 0;
}