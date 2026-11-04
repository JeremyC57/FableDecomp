// CVectorMap::LowerBound(const RetailUID& key) -- Return the first entry not less than an unsigned 64-bit key.
// Leaf: ?LowerBound@?$CVectorMap@_KPAVCANavQuadTreeNode@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@@QAEPAU?$pair@_KPAVCANavQuadTreeNode@@@std@@AB_K@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Identity: Own CNavQuadTree::SetSwitchableNodesAsBlocked00a79240 and SwitchableNodes+64 prove mutable map<UID,CANavQuadTreeNode*>; prior pair class-key V corrected to original struct U.
// Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.
#ifdef _MSC_VER
typedef unsigned __int64 RetailUID;
#else
typedef unsigned long RetailUID;
#endif
class CANavQuadTreeNode;class CActiveEntityScriptBase;class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
#include "engine/CVectorMap.h"
typedef CVectorMap<RetailUID,CANavQuadTreeNode* > Map;
template Map::iterator Map::LowerBound(const RetailUID&);