// CLocalDetailPrimitiveBase::CanRelocateData(long flags) const -- permit relocation while the owning detail map is unlocked.
// Leaf: ?CanRelocateData@CLocalDetailPrimitiveBase@@UBE_NJ@Z
// Identity: proven; own RTTI 012A7DB0 slot2 and both PDB CanRelocateData(long)const bool API; original CMovableResource base4; own Map+0x34 and original Map.LockedFromMemoryRelocation byte0.
// Layout: shared CLocalDetailCacheMap data header reused unchanged under alias, including actual lock byte0; outer class adds nested naming scopes only. CMovableResource header flattens virtual interface, so genuine virtual base4 and required slot2 are modeled locally. Unused record prefix preserves actual Map member offsets.
#define CLocalDetailCacheMap CLocalDetailCacheMap_HeaderFields
#include "engine/CLocalDetailCacheMap.h"
#undef CLocalDetailCacheMap
class CMovableResource{public:virtual ~CMovableResource(){}virtual void UnusedSlot1(){}virtual bool CanRelocateData(long)const=0;};class CLocalDetailCacheMap:public CLocalDetailCacheMap_HeaderFields{public:class CObjectTypeCollection;class CObjectCacheGroupCollection;};
class CLocalDetailPrimitiveBase:public CMovableResource{public:unsigned char _unmodeled04[48];CLocalDetailCacheMap* Map;virtual bool CanRelocateData(long)const;};
bool CLocalDetailPrimitiveBase::CanRelocateData(long)const{return !Map->LockedFromMemoryRelocation;}