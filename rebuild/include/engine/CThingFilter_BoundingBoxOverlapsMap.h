#ifndef FABLE_CTHINGFILTER_BOUNDINGBOXOVERLAPSMAP_H
#define FABLE_CTHINGFILTER_BOUNDINGBOXOVERLAPSMAP_H
#include "stlport/fable_containers.h"
class CThing;class CWorldMap;
class CThingFilter_BoundingBoxOverlapsMap:public std::unary_function<CThing,bool> {
#ifdef FABLE_RETAIL_MAP_CACHED_VOLUMES_PASS26
public:
    CThingFilter_BoundingBoxOverlapsMap(const CWorldMap& world_map,long map_no) : WorldMap(world_map),MapNo(map_no) {}
#else
public:CThingFilter_BoundingBoxOverlapsMap(const CWorldMap&,long);
#endif
 bool operator()(const CThing& test_thing)const;
private:const CWorldMap& WorldMap;long MapNo;
};
#endif
