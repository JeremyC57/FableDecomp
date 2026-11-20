// CWorldMap::GetMapUID(long map) const -- the UID of map number `map` (CMapInfo::MapUID of Maps[map])
// Leaf: ?GetMapUID@CWorldMap@@QBEKJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FCE00, 17 bytes: `mov eax,[esp+4] ; mov ecx,[ecx+20h] ; lea eax,[eax+eax*8] ;
//   mov eax,[ecx+eax*8+20h] ; ret 4` (Maps start +0x20, 0x48-byte CMapInfo, MapUID +0x20).
// Naming (NF108, 2026-11-19, q4250): was a stand-in S::GetEditMap(int) (catalog CWorldMap::GetMapUIDPrefix). Ego_r.pdb 0x004D03C0 +
//   FableWin.pdb 0x01C94F90 ?GetMapUID@CWorldMap@@QBEKJ@Z (const, long map -> unsigned long UID); engine/CWorldMap.h declares it at
//   0x004FCE00; NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary 0x00521620 calls it on PeekWorldMap() and binds it.
//   Re-authored on engine/CWorldMap.h (FABLE_RETAIL_CWORLDMAP: Maps = CArray<CMapInfo> +0x20) + engine/CMapInfo.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"

unsigned long CWorldMap::GetMapUID(long map) const
{
    return Maps[map].MapUID;
}
