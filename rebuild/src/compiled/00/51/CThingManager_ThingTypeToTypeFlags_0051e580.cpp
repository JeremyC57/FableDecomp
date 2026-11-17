// CThingManager::ThingTypeToTypeFlags(long type) const -- the type flags of thing type `type` (ThingTypeInfo[type].ThingTypeFlags)
// Leaf: ?ThingTypeToTypeFlags@CThingManager@@QBEKJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051E580, thiscall `ret 4`.
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[ecx+3Ch] ; lea eax,[eax+eax*2] ; mov eax,[ecx+eax*4+4] ; ret 4
// Identity (agentNF54 name fix; was the descriptive stand-in CEditWorldMap_GetEditMap, a __fastcall free function on a local
//   CEditWorldMap model): Ego_r.pdb 0x00528260 + FableWin.pdb 0x01D33D60 ?ThingTypeToTypeFlags@CThingManager@@QBEKJ@Z. The body
//   indexes CThingManager::ThingTypeInfo (+0x3C, std::vector<CThingTypeInfo>, 12-byte elements) and returns ThingTypeFlags (+4),
//   as engine/CThingManager.h FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS documents; its three retail callers are the CThing
//   constructors ConstructFromParams 0x004CA010, ConstructFromBinarySave 0x004CA390 (landed, binds this spelling) and
//   ConstructFromLevelScript 0x004CAA00, each asking for the new thing's type flags.
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER_THINGTYPEFLAGS
#include "engine/CThingManager.h"

unsigned long CThingManager::ThingTypeToTypeFlags(long type) const
{
    return ThingTypeInfo[type].ThingTypeFlags;
}
