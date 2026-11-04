// CWorldMap::HeightAreaChanged(const C2DBoxI*) -- a height-map area changed (null = the whole world): tell the engine, every overlapping map (in its local coordinates), rebuild the map edges, then update each overlapping navigation map's area
// Grade: DIFFER(260v261) -- retail keeps 0 in ebx from the area test to the vector release (cmp ebp,ebx; re-zeroed after loop 1) and runs loop 2 in ebp; ours zeroes via eax, loop 2 in ebx, test ebx,ebx; 11 grades (vector model, iterator reuse, ternary area, decl order)
// Leaf: ?HeightAreaChanged@CWorldMap@@UAEXPBVC2DBoxI@@@Z
// Flags: none
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DBoxI& box) : TLX((float)box.TLX), TLY((float)box.TLY), BRX((float)box.BRX), BRY((float)box.BRY) { }
};
class CMap { public:
    void HeightAreaChanged(const C2DBoxI* area);       // 0x0081E810
};
class CMapInfo { public:                                      // 0x48 bytes
    C2DBoxI       Area;                                // +0x00
    CMap*         PMap;                                // +0x10 (CCountedPointer<CMap> data word)
    void*         PMap_Info;                           // +0x14
    unsigned char _body[0x30];
};
class CEngineManager { public:
    static CEngineManager& __fastcall Get();           // 0x00436FB0 CTBaseSingleton<CEngineManager>::Get
    void AddChangedArea(const C2DBoxI& area);          // 0x00640560
};
class CNavigatorManager { public:
    void UpdateNavigationMapArea(long map, const C2DBoxF& area);   // 0x00A14E40
};
class CWorld { public:
    unsigned char      _pad00[0x48];
    CNavigatorManager* PNavigationManager;             // +0x48
};
class CWorldMap {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual const C2DBoxI& GetDimensions() const;      // slot 13 (+0x34) 0x0051CDE0
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void GetAllMapsInArea(const C2DBoxI& area, std::vector<unsigned long>& maps) const;   // slot 20 (+0x50) 0x004FF150
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27();
    virtual void HeightAreaChanged(const C2DBoxI* area);   // slot 28
    unsigned char _pad04[4];
    CWorld*       World;                               // +0x08
    unsigned char _pad0c[0x14];
    CMapInfo*     Maps;                                // +0x20 (CArray<CMapInfo> start)
    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long map, const C2DBoxI& area) const;   // 0x004FC4D0
protected:
    void UpdateAllEdges();                             // 0x0063DFD0
};
void CWorldMap::HeightAreaChanged(const C2DBoxI* area)
{
    C2DBoxI dimensions = GetDimensions();
    std::vector<unsigned long> maps;
    if (!area)
        area = &dimensions;
    CEngineManager::Get().AddChangedArea(*area);
    GetAllMapsInArea(*area, maps);
    for (unsigned long* it = maps.begin(); it < maps.end(); ++it) {
        long map = *it;
        C2DBoxI local = ClipAndConvertWorldMapAreaToLocalArea(map, *area);
        CMap* pmap = Maps[map].PMap;
        if (pmap)
            pmap->HeightAreaChanged(&local);
    }
    UpdateAllEdges();
    for (unsigned long* it2 = maps.begin(); it2 < maps.end(); ++it2)
        World->PNavigationManager->UpdateNavigationMapArea(*it2, C2DBoxF(*area));
}
