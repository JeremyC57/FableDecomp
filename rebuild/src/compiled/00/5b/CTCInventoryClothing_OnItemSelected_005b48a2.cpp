// CTCInventoryClothing::OnItemSelected(long itemDefIndex, bool& handled) -- for a hero suit: collect the hero's worn non-removable clothing, deselect everything, reselect every part of the suit the inventory holds plus the collected clothing; then set up the selected clothes and dress the mannequin (or the owner)
// Leaf: ?OnItemSelected@CTCInventoryClothing@@UAEXJAA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B48A2, 460 bytes, thiscall `leave ; ret 8`, frame 0x1C. Identity: CTCInventoryClothing vtable
//   0x0124356C slot 33 (the CTCInventoryBase::OnItemSelected override), Ego_r.pdb / FableWin.pdb ?OnItemSelected@CTCInventoryClothing@@UAEXJAA_N@Z
//   (Ego_r donor 0x0056017E); the catalog's "CTCShop::GetName" is wrong.
// Header: engine/CTCInventoryClothing.h FABLE_RETAIL_CTCINVENTORYCLOTHING_SELECT, engine/NInventory_CTCInventoryBase.h
//   FABLE_RETAIL_CTCINVENTORYBASE_SELECTION (DeselectAll / SelectItemWithDefIndex).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTION
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SELECT
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroSuitDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CThingObjectDef.h"

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CDefClassBase> 0x0042AF3C,
// GetSubDef<CHeroSuitDef> 0x005935C4, GetSubDef<CAppearanceModifierDef> 0x0057FF92) and STLport's _List_base(const allocator&)
// (0x0042AB58): visible bodies keep the CDefPointer locals uncaptured and home the list's allocator temporary on a dead parameter byte.
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
template <class _Tp, class _Alloc>
std::_List_base<_Tp, _Alloc>::_List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(), 0)
{
    _List_node<_Tp>* n = _M_node.allocate(1);
    n->_M_next = n;
    n->_M_prev = n;
    _M_node._M_data = n;
}
#pragma auto_inline(on)

void CTCInventoryClothing::OnItemSelected(long itemDefIndex, bool& handled)
{
    CDefPointer<const CDefClassBase> def;
    GetDefinitionManager().GetDef(itemDefIndex, &def);
    CDefPointer<const CHeroSuitDef> suit;
    if (def->GetSubDef(&suit)) {
        std::vector<long> wornItems;
        CTCHeroAttachableAppearanceModifiers* modifiers;
        if (ParentThing->GetTC(TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS, &modifiers)) {
            std::list<long> modifierList;
            modifiers->GetAppearanceModifierList(modifierList, APPEARANCE_CLOTHING);
            for (std::list<long>::iterator it = modifierList.begin(); it != modifierList.end(); ++it) {
                CDefPointer<const CDefClassBase> itemDef;
                CDefPointer<const CAppearanceModifierDef> modifier;
                if (GetDefinitionManager().GetDef(*it, &itemDef)) {
                    if (itemDef->GetSubDef(&modifier) && !modifier->IsRemoveable)
                        wornItems.push_back(*it);
                }
            }
        }
        DeselectAll();
        for (std::map<EClothingSuitPart, long>::const_iterator part = suit->SuitParts.begin(); part != suit->SuitParts.end(); ++part) {
            if (GetNumberOfItemsOfTypeInInventory(part->second) > 0)
                SelectItemWithDefIndex(part->second);
        }
        for (std::vector<long>::iterator worn = wornItems.begin(); worn != wornItems.end(); ++worn)
            SelectItemWithDefIndex(*worn);
    }
    SetUpSelectedClothes();
    if (UseMannequin) {
        if (PMannequin)
            SetThingAsWearingSelectedClothes(*PMannequin);
    } else {
        SetThingAsWearingSelectedClothes(*ParentThing);
    }
}