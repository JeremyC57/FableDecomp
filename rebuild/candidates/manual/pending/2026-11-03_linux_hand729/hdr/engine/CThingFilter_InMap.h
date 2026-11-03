#ifndef FABLE_ENGINE_CTHINGFILTER_INMAP_H
#define FABLE_ENGINE_CTHINGFILTER_INMAP_H
// Ego_r.pdb CThingFilter_InMap (8 bytes: CWorldMap* WorldMap +0, long MapNo +4); ctor ??0CThingFilter_InMap@@QAE@ABVCWorldMap@@J@Z
// (inline in retail: CTCInventoryMap::CreateMiniMapMarkers 0x00603000 stores PeekWorldMap() and the map number in place);
// operator() ??RCThingFilter_InMap@@QBE_NABVCThing@@@Z is retail 0x006A7930.
class CWorldMap; class CThing;
class CThingFilter_InMap {
    CWorldMap* WorldMap;
    long MapNo;
public:
    CThingFilter_InMap(const CWorldMap& worldMap,long mapNo):WorldMap(const_cast<CWorldMap*>(&worldMap)),MapNo(mapNo){}
    bool operator()(const CThing&) const;                                // 0x006A7930
};
#endif
