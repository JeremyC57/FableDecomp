// CTCMapwho::OnDie() -- mapwho component death hook: take the thing out of the world map's thing-map manager while it is in the mapwho
// Leaf: ?OnDie@CTCMapwho@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A7C0, 25 bytes: `test byte [esi+50h],1 ; je ; call 0x00686D30 ; mov ecx,[eax+10h] ; push esi ;
//   call 0x00637A10 ; ret` (RemoveFromMapwho 0x0071A730 expanded, as in the landed ~CTCMapwho 0x0071A790).
// Identity (NF179, PRIORITY q12048): Ego_r.pdb 0x0072DA20 + FableWin.pdb 0x02599EC0 ?OnDie@CTCMapwho@@UAEXXZ (catalog CTCMapwho::OnDie);
//   the CTCMapwho vtable emitted by the landed 0x0071A790 already binds 0x0071A7C0 under this spelling. Was the free stand-in
//   ?CTCMapwho_OnDie@@YIXPAVCTCMapwho@@@Z (no Leaf line) over local HelperObj / Helper2 models, which bound CTCBase::GetWorldMap 0x00686D30
//   as ?helper_686d30@@YAPAUHelperObj@@XZ and NThingMap::CThingMapManager::RemoveThing 0x00637A10 as ?Notify@Helper2@@QAEXPAVCTCMapwho@@@Z;
//   both now bind the PDB spellings the other landed rows use (?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ,
//   ?RemoveThing@CThingMapManager@NThingMap@@QAEXAAVCTCMapwho@@@Z). Shared headers as 0x0071A790.
// Inferred: the body is RemoveFromMapwho's text written out (0x0071A790 expands the same call from a TU-local strong RemoveFromMapwho;
//   repeating that definition here would add a second strong ?RemoveFromMapwho@CTCMapwho@@QAEXXZ beside 0x0071A790's).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER
#include "engine/CWorldMap.h"
#define FABLE_RETAIL_CTHINGMAPMANAGER
#include "engine/CThingMapManager.h"
#define FABLE_RETAIL_CTCMAPWHO
#include "engine/CTCMapwho.h"

void CTCMapwho::OnDie()
{
    if (IsInMap())
        GetWorldMap().GetThingMapManager().RemoveThing(*this);
}
