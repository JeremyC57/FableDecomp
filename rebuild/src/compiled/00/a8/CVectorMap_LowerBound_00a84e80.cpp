// CVectorMap::LowerBound(const RetailUID& key) const -- Return the first entry not less than an unsigned 64-bit key.
// Leaf: ?LowerBound@?$CVectorMap@_K_NV?$CKeyPairCompareLess@_K_N@@@@QBEPBU?$pair@_K_N@std@@AB_K@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Identity: Own CNavSwitchableLeafNode::IsSwitchableOn00a84e40 is original PDB const virtual API; UniqueIDsOpen+32 original map<UID,bool>. Use const LowerBound/const iterator, not mutable placeholder.
// Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.
#ifdef _MSC_VER
typedef unsigned __int64 RetailUID;
#else
typedef unsigned long RetailUID;
#endif
class CANavQuadTreeNode;class CActiveEntityScriptBase;class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
#include "engine/CVectorMap.h"
typedef CVectorMap<RetailUID,bool > Map;
template Map::const_iterator Map::LowerBound(const RetailUID&) const;