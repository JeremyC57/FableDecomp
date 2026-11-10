#ifndef FABLE_CTHINGFILTER_BOUNDINGBOXOVERLAPSMAP_H
#define FABLE_CTHINGFILTER_BOUNDINGBOXOVERLAPSMAP_H
#include "stlport/fable_containers.h"
class CThing;class CWorldMap;
class CThingFilter_BoundingBoxOverlapsMap:public std::unary_function<CThing,bool> {
public:CThingFilter_BoundingBoxOverlapsMap(const CWorldMap&,long);
 bool operator()(const CThing& test_thing)const;
private:const CWorldMap& WorldMap;long MapNo;
};
#endif
