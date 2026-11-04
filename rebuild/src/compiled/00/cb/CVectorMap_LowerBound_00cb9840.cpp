// CVectorMap::LowerBound(const RetailUID& key) -- Return the first entry not less than an unsigned 64-bit key.
// Leaf: ?LowerBound@?$CVectorMap@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@V?$CKeyPairCompareLess@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@@@@QAEPAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@AB_K@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Identity: Original own script callers00cb7bc0/00cb88b0 and established map<UID,CCountedPointer<CActiveEntityScriptBase>> binding prove mutable LowerBound.
// Layout: original CArray/STLport vector12 with pair16, 8-byte-aligned UID. Comparer+12/Dirty+13.
#ifdef _MSC_VER
typedef unsigned __int64 RetailUID;
#else
typedef unsigned long RetailUID;
#endif
class CANavQuadTreeNode;class CActiveEntityScriptBase;class CCPPointerInfo;template<class T>class CCountedPointer{public:T* Data;CCPPointerInfo* Info;};
#include "engine/CVectorMap.h"
typedef CVectorMap<RetailUID,CCountedPointer<CActiveEntityScriptBase> > Map;
template Map::iterator Map::LowerBound(const RetailUID&);