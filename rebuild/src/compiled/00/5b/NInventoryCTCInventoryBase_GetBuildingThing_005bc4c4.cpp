// NInventory::CTCInventoryBase::GetBuildingThing() const -- the building (shop / house) thing this inventory screen belongs to
// Leaf: ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC4C4, 7 bytes (`mov eax,[ecx+0B8h] ; ret`).
// Identity: Ego_r.pdb + FableWin.pdb ?GetBuildingThing@CTCInventoryBase@NInventory@@QBEPAVCThing@@XZ; engine/NInventory_CTCInventoryBase.h
//   declares it at this address (FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING) over pBuildingThing (+0xB8, Ego_r.pdb member). Its retail
//   callers are CTCInventoryTrade members (NF85's 0x005A731F port binds this spelling). The old row's free function DrawGetEnvironment and
//   the catalog label GetMember_dword0xb8_005bc4c4 were placeholders.
// NF87: re-authored as the member on the shared header (was a free function over an offset cast).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/NInventory_CTCInventoryBase.h"

CThing* NInventory::CTCInventoryBase::GetBuildingThing() const
{
    return pBuildingThing;
}
