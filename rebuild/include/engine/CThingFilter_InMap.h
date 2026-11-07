#ifndef FABLE_ENGINE_CTHINGFILTER_INMAP_H
#define FABLE_ENGINE_CTHINGFILTER_INMAP_H
// New header (chunk 851, rows 0x006001E0 / 0x005FE840). Ego_r.pdb CThingFilter_InMap, 8 bytes: CWorldMap* WorldMap +0x0, long MapNo
// +0x4 (struct_layouts_egor.tsv). FableWin.pdb ??0CThingFilter_InMap@@QAE@ABVCWorldMap@@J@Z (inline in retail: the caller stores the
// two words in place, 0x006002E6) and ??RCThingFilter_InMap@@QBE_NABVCThing@@@Z (out of line, 0x006A7930, landed binding).
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
class CThing;
class CWorldMap;
class CThingFilter_InMap : public std::unary_function<CThing,bool> {
private:
    CWorldMap* WorldMap;
    long MapNo;
public:
    CThingFilter_InMap(const CWorldMap& world_map,long map_no):WorldMap(const_cast<CWorldMap*>(&world_map)),MapNo(map_no){}
    bool operator()(const CThing& thing) const;                          // 0x006A7930
};
FABLE_STATIC_ASSERT(sizeof(CThingFilter_InMap)==8);
#endif
