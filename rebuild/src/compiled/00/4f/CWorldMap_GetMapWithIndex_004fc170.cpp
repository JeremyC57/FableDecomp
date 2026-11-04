// CWorldMap::GetMapWithIndex(long index) const -- the const reference to Maps[index] (a 0x48-byte CMapInfo)
// Leaf: ?GetMapWithIndex@CWorldMap@@QBEABVCMapInfo@@J@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FC170, 16 bytes, thiscall `ret 4`: mov eax,[esp+4] ; mov ecx,[ecx+20h] ;
//   lea eax,[eax+eax*8] ; lea eax,[ecx+eax*8] (index * 72).
// Identity: probable. The row sits in the CWorldMap accessor run (after PeekMapAt 0x004FC150, before the const
//   GetRegion 0x004FC180). Ego_r.pdb CWorldMap::Maps is CArray<CMapInfo> (+0x24; retail +0x20 behind the 12-byte
//   STLport map) and CMapInfo is 72 bytes with LevelName at +0x18 -- the only caller (0x0082BC00, minimap code) reads
//   that CCharString at +0x18 of the result. Of the PDB accessors returning a CMapInfo by index (GetMapInfo const /
//   non-const, GetMapWithIndex const), Ego_r.pdb keeps an out-of-line body only for
//   ?GetMapWithIndex@CWorldMap@@QBEABVCMapInfo@@J@Z, the name used here.
// Layout: engine/CWorldMap.h is the Ego_r layout (Maps +0x24), so a local model with the retail offset (header debt).
// Shape: STLport `operator[]` as `*(begin() + n)`; the 72-byte stride is built from two `lea`s.
class CMapInfo {
public:
    unsigned char _pad00[0x48];                 // retail/PDB sizeof 0x48 (LevelName +0x18)
};

#include "engine/CArray.h"


class CWorldMap {
public:
    unsigned char    _pad00[0x20];
    CArray<CMapInfo> Maps;                      // +0x20 (Ego_r.pdb +0x24)
    const CMapInfo& GetMapWithIndex(long index) const;
};

const CMapInfo& CWorldMap::GetMapWithIndex(long index) const
{
    return Maps[index];
}