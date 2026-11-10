// CTCInventoryClothing::OnInitialActivate() -- on a fresh start (not loaded from a save game) gives the owner every clothing item of its creature def's initial appearance modifiers: creates the item at the owner's position for the owner's player, flags it as a default item and adds it to this inventory (marking a live item as created by its owner), or kills it when it has no inventory-item component; then the base inventory activation
// Leaf: ?OnInitialActivate@CTCInventoryClothing@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B3AEF, 422 bytes, thiscall plain `ret`, `mov ebp,esp ; sub esp,20h` (size TU, optimize("s")).
// Identity: slot 6 of the CTCInventoryClothing vtable 0x0124356C (CTCBase::OnInitialActivate); Ego_r.pdb donor 0x0055F38A / FableWin.pdb
//   0x01E1B4B1 ?OnInitialActivate@CTCInventoryClothing@@UAEXXZ; it ends in NInventory::CTCInventoryBase::OnInitialActivate 0x005BD1EB (qualified).
// Callees: GetSubDef<CCreatureDef> 0x00485815, CTCBase::GetDefinitionManager 0x006869D0, GetDef<CThingObjectDef> 0x0042B040,
//   GetSubDef<CAppearanceModifierDef> 0x0057FF92, CCharString 0x0099EBF0 / 0x0099EAE0, CThing::GetPos 0x004C73D0, CThingObject::Create
//   0x00703210, bitset test 0x00410DE0 + CVectorMap::LowerBound 0x0040F020 (GetTC<CTCInventoryItem>, interface 0x1A),
//   CTCInventoryItem::AddToInventory 0x005D8D50, CThing::Kill 0x004C9B80.
// Data: CCreatureDef::InitialAppearanceModifiers (vector<long> +0x54), CAppearanceModifierDef::AppearanceType (+0x50, APPEARANCE_CLOTHING 2),
//   CTCInventoryItem::DefaultItem (+0x20), CThing Dead / CreatedByOwnerThingCreation (bits 0 / 1 of +0x91).
// Shape: the script name of the created thing is a named CCharString in a block of its own around the Create call (as the landed
//   CCreatureActionUndress::PerformAction 0x00846410): retail builds it first, then loads ParentThing once for the player and GetPos
//   (0x005B3BA1..0x005B3BC4) and keeps `this` in memory with 0 in ebx; a CCharString("") temporary argument lets VC7.1 read the player
//   before the constructor, reload ParentThing, and give `this` ebx instead (DIFFER 429v422). The def lookups have same-TU bodies under
//   auto_inline(off) (as ConstructScreenPC 0x005B8743): the modifier pointer stays known-null when GetDef fails (retail skips its release).
// Header: engine/CTCInventoryClothing.h _ACTIVATE, engine/CCreatureDef.h _INITIALAPPEARANCEMODIFIERS, engine/CThingObject.h _CREATE,
//   engine/CTCInventoryItem.h _ADDTOINVENTORY, engine/CThing.h _CREATEDBYOWNER; graded with FABLE_HDR_OVERLAY=$A/hdr.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_CREATEDBYOWNER
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ONINITIALACTIVATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_ACTIVATE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ADDTOINVENTORY
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_INITIALAPPEARANCEMODIFIERS
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CAppearanceModifierDef.h"

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetSubDef<CCreatureDef> 0x00485815,
// GetDef<CThingObjectDef> 0x0042B040, GetSubDef<CAppearanceModifierDef> 0x0057FF92).
#pragma auto_inline(off)
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

void CTCInventoryClothing::OnInitialActivate()
{
    if (!LoadedFromSaveGame) {
        CDefPointer<const CCreatureDef> creatureDef;
        if (ParentThing->GetPBaseDef()->GetSubDef(&creatureDef)) {
            for (unsigned long i = 0; i < creatureDef->InitialAppearanceModifiers.size(); ++i) {
                long defIndex = creatureDef->InitialAppearanceModifiers[i];
                CDefPointer<const CThingObjectDef> def;
                CDefPointer<const CAppearanceModifierDef> modifier;
                if (GetDefinitionManager().GetDef(defIndex, &def)) {
                    if (def->GetSubDef(&modifier) && modifier->AppearanceType == APPEARANCE_CLOTHING) {
                        CThing* thing;
                        {
                            CCharString name("");
                            thing = CThingObject::Create(defIndex, ParentThing->GetPos(), ParentThing->GetPlayerNumber(), 0, 0, name);
                        }
                        CTCInventoryItem* item;
                        if (thing->GetTC(&item)) {
                            item->DefaultItem = true;
                            item->AddToInventory(*this, true, true, 0, true, false);
                            if (!thing->IsDead())
                                thing->SetAsCreatedByOwnerThingCreation(true);
                        } else {
                            thing->Kill(false);
                        }
                    }
                }
            }
        }
    }
    NInventory::CTCInventoryBase::OnInitialActivate();
}