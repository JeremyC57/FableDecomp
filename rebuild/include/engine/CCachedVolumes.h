#ifndef FABLE_ENGINE_CCACHEDVOLUMES_H
#define FABLE_ENGINE_CCACHEDVOLUMES_H
#ifdef FABLE_RETAIL_MAP_CACHED_VOLUMES_PASS26
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
class CContainmentVolume;
// Ego_r CCachedVolumes has one member at offset zero. Its 20 donor bytes become 16 in the retail allocator ABI.
class CCachedVolumes {
public:
    void InitialiseNavigationContainmentVolumes(const CVectorMap<long,CCountedPointer<CContainmentVolume> >& volumes);
private:
    CVectorMap<long,CCountedPointer<CContainmentVolume> > NavigationContainmentVolumes;
};
FABLE_STATIC_ASSERT_AT(901,sizeof(CCachedVolumes)==16);
#endif
#endif
