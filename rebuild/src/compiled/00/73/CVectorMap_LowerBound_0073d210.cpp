// CVectorMap::LowerBound(const unsigned long& key) const -- Find the first entry whose key is at least key.
// Leaf: ?LowerBound@?$CVectorMap@KVCRegisteredModeInfo@CTCCreatureModeManager@@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@@QBEPBU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@ABK@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local same-TU LowerBound body removed (the header defines the same-TU noinline body).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Identity: Own Count0073da50 and AllocateMode0073a8e0; PDB CRegisteredModeInfo::AllocFunc, const registry lookup.
#ifndef _MSC_VER
#define __cdecl
#endif
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
// Layout: retail CVectorMap has12-byte Data at+0, empty Compare+12 and Dirty+13; key/value element is8bytes. Own callers and loop stride confirm these offsets.
class CThing;class CModeBase;
// Layout: CTCCreatureModeManager supplies only a nested name scope. CRegisteredModeInfo is the original4-byte AllocFunc pointer record; no outer object is accessed.
class CTCCreatureModeManager {public:class CRegisteredModeInfo {public:CModeBase* (__cdecl* AllocFunc)(CThing*);};};
typedef CVectorMap<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> > Map;
template class CVectorMap<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo,CKeyPairCompareLess<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo> >;