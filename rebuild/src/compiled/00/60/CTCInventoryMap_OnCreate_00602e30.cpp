// CTCInventoryMap::OnCreate() -- registers the map screen with the hero-information screen manager, points the inventory at the map sub-def's inventory def, runs the base OnCreate, then sizes/clears the map-screen primitive handle vectors and caches the last region number
// Retail 0x00602E30, 256 bytes (+ int3), esp frame (speed TU: `push ecx`, the null-safe base conversion as
// `test ; je ; lea ; jmp ; xor`). Catalogued as "CTCInventoryMap::vfunc_4": slot 4 of the CTCInventoryMap vtable
// 0x01257A9C (rtti-vtables.tsv) = OnCreate, and it calls CTCInventoryBase::OnCreate 0x005C0121.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (global player GUI 0x013B8790),
// NHeroInformationScreens::CManager::RegisterScreen(const CBase*) 0x0066EB11, CDefClassBase::GetSubDef<T>
// 0x00485815, CTCInventoryBase::SetPDef(long) 0x005BDB4D, CTCInventoryBase::OnCreate 0x005C0121,
// vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90 and erase(first, last) 0x00440D70, CTCBase::PeekWorld
// 0x00686A80, CWorld slot 13 (+0x34, PeekWorldMap), CWorldMap::GetNoMaps 0x004FB470 (manifest
// "CWorldMap::DrawGetNoMaps"; the result minus one is the last region index), the defs' slot 1 on release.
// Shape: the def dance of the landed CTCInventoryWeapons::OnCreate 0x005C3057 (owner def read into a local, the
// sub-def out pointer zeroed between the load and the null test, hand add-ref/release around GetSubDef, the sub-def
// released by hand at the end without a null test); the three handle vectors are cleared through an inline
// `clear() { erase(begin(), end()); }`.
// Layout (retail, 12-byte STLport vectors; Ego_r.pdb names): the NHeroInformationScreens::CBase secondary base +0x130,
// MapScreenPrimitiveHandles +0x1AC, MapScreenQuestPrimitiveHandles +0x1B8, MapScreenTeleportPrimitiveHandles +0x1C4,
// MapScreenHousePrimitiveHandles +0x1D0, NumRegions +0x23C; the map def's inventory-def index at +0x8C.
// Leaf: ?OnCreate@CTCInventoryMap
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
struct CInventoryMapDef : CDefPointeeBase {
    unsigned char _pad08[0x8c - 0x08];
    long InventoryDefIndex;                     // +0x8c
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CInventoryMapDef>* out);   // 0x00485815
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;            // +0x70
};

namespace NHeroInformationScreens {
class CBase { public:                                  // vtable 0x0123A3E0
    virtual void v00();
    virtual void v01();
    virtual void v02();
};
class CManager { public:
    void RegisterScreen(const CBase* screen);   // 0x0066EB11
};
}
class CPlayerGui { public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790

class CEnginePrimitiveHandle;
struct CPrimitiveHandleVector {                 // vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    void Resize(long count);                                                        // 0x00443F90
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
    void clear() { erase(begin(), end()); }
};
class CWorldMap { public:
    long GetNoMaps() const;                     // 0x004FB470 (PDB ?GetNoMaps@CWorldMap@@QBEJXZ)
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap* PeekWorldMap() const;   // slot 13 (+0x34) (FableWin.pdb)
};

class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
struct CTCInventoryBase_Map : public CTCBase {  // retail 0x130 bytes
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                    // slot 4
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x130 - 0x08];
    void SetPDef(long globalDefIndex);          // 0x005BDB4D
};
class CTCInventoryMap : public CTCInventoryBase_Map, public NHeroInformationScreens::CBase {
public:
    unsigned char _pad134[0x1ac - 0x134];
    CPrimitiveHandleVector MapScreenPrimitiveHandles;          // +0x1ac
    CPrimitiveHandleVector MapScreenQuestPrimitiveHandles;     // +0x1b8
    CPrimitiveHandleVector MapScreenTeleportPrimitiveHandles;  // +0x1c4
    CPrimitiveHandleVector MapScreenHousePrimitiveHandles;     // +0x1d0
    unsigned char _pad1dc[0x238 - 0x1dc];
    long ActiveRegionIndex;                     // +0x238
    long NumRegions;                            // +0x23c
    virtual void OnCreate();
};

void CTCInventoryMap::OnCreate()
{
    g_pPlayerGui->GetPHeroInfoScreensManager()->RegisterScreen(this);
    CDefClassBase* thingDef = ParentThing->PDef.Object;
    CDefPointer<CInventoryMapDef> def;
    def.Object = 0;
    if (thingDef)
        ++thingDef->m_refCount;
    thingDef->GetSubDef(&def);
    if (thingDef && --thingDef->m_refCount == 0)
        thingDef->Destroy();
    CInventoryMapDef* mapDef = def.Object;
    SetPDef(mapDef->InventoryDefIndex);
    CTCInventoryBase_Map::OnCreate();
    MapScreenPrimitiveHandles.Resize(2);
    MapScreenQuestPrimitiveHandles.clear();
    MapScreenTeleportPrimitiveHandles.clear();
    MapScreenHousePrimitiveHandles.clear();
    NumRegions = PeekWorld().PeekWorldMap()->GetNoMaps() - 1;
    if (--mapDef->m_refCount == 0)
        mapDef->Destroy();
}