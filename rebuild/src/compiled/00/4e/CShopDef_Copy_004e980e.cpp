// CShopDef::Copy(const CDefClassBase* other) -- copy every member of another CShopDef: `*this = *(const CShopDef*)other;`
// Leaf: ?Copy@CShopDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E980E, 5 bytes: a thiscall tail call, `jmp` to CShopDef::operator= 0x004E9813.
// Identity (NF425, 2026-11-25): self ?Copy@CShopDef@@UAEXPBVCDefClassBase@@@Z: RTTI placement + Ego_r/FableWin; target
//   ??4CShopDef@@QAEAAV0@ABV0@@Z: bound at 0x004E9813 (owner Leaf); Ego_r/FableWin name set; its body opens with a call of the ancestor
//   operator= ??4CDefClassBase (0x00431F10): a derived class's assignment.
// Respell (NF425): was the stand-in self _CShopDef_Copy; now the real virtual member, so the RTTI-placed name is defined.
// Header: the class's own header engine/CShopDef.h with the prelude of 00/77/CTCShop_BuyItem_00774d20.cpp (the header declares Copy and
//   operator=).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CSTOCKITEMDEF
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE
#define FABLE_RETAIL_CTCSHOP_BUYITEM
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCSHOP_ADDSALEITEMPLACES
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CTCHEROSTATS_MORALITYMAX
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CATEGORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#include "fable_string.h"
#include <math.h>
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CShopDef.h"

void CShopDef::Copy(const CDefClassBase* other)
{
    *this = *(const CShopDef*)other;
}
