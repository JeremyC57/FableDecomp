// CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& text, long itemDefIndex, float damage, bool fromDef) const -- appends the augmentations line of a weapon item to its description (the names of its filled augmentation slots, comma separated, or "none"; from the def's initial augmentations or the owned item's per-item data), then a newline and the item's details text
// Grade: DIFFER(680v680) -- 4 lines, scheduling only (FABLE_HDR_OVERLAY=$A/hdr): the TEXT_GUI_MENU_NONE block issues its two argument setups in source order (ours `push 0x01254554 ; lea ecx,[ebp+14h]` / `push eax ; mov ecx,esi`, retail `lea ecx,[ebp+14h]` and `mov ecx,esi` first). The nested-else spelling that schedules that block as retail (the twin 0x005C6B26's lever) loses the copy loop's count split (672v680): c2prio.py shows const 0 (priority below i#26 at 24) is what lets `count` split into ecx; a diagnostic extra use of 0 restores it, but no natural spelling found in 17 grades this session (copy-loop do/while/for forms 672-674, size()!=0 guards, i++ v ++i, shared count/i variables, used!=0, NONE-structure variants, resize(n, i)) plus 8 in chunk 1002b.
// Leaf: ?ConstructWeaponDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C71AA, 680 bytes, thiscall `ret 10h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Shape: same TU header as 0x005C7452 (same-TU GetDef/GetSubDef bodies, inline CPerItemData ctor, noinline vector<long>::resize 0x004428F0);
//   the resize count is read through a raw pointer (`a->NumberOfSlots`) as retail reloads it; the copy loop is the guarded do-while that keeps
//   `count` in a frame slot; `damage` is unused (it is in the PDB signature).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWeaponDef.h"
#include "engine/CObjectAugmentationsDef.h"

namespace { extern const CWideString NEWLINE; }                  // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L"\n")

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312,
// GetSubDef<CWeaponDef> 0x0042B1A6, GetSubDef<CObjectAugmentationsDef> 0x0042AF9E): VC7.1 sees that they only write through the
// out pointer, so the CDefPointer locals whose addresses they take stay unaliased.
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
void CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& text, long itemDefIndex, float damage, bool fromDef) const
{
    RefreshActiveWeaponData();
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(itemDefIndex);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    NInventory::CPerItemData data;
    const std::vector<long>* augmentations;
    if (fromDef) {
        CDefPointer<const CObjectAugmentationsDef> augmentationsDef;
        if (def->GetSubDef(&augmentationsDef)) {
            const CObjectAugmentationsDef* a = augmentationsDef.Get();
            data.Augmentations.resize(a->NumberOfSlots, 0);
            unsigned long i = 0;
            if (i < augmentationsDef->InitialAugmentations.size()) {
                unsigned long count = data.Augmentations.size();
                do {
                    if (i >= count)
                        break;
                    data.Augmentations[i] = augmentationsDef->InitialAugmentations[i];
                    ++i;
                } while (i < augmentationsDef->InitialAugmentations.size());
            }
        }
        augmentations = &data.Augmentations;
    } else {
        if (GetNumberOfItemsOfTypeInInventory(itemDefIndex) == 0)
            return;
        augmentations = &GetPerItemDataForItem(itemDefIndex)->Augmentations;
    }
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscDef->SpaceSeparator;
    bool none = true;
    if (!augmentations->empty()) {
        long last = -1;
        long count = augmentations->size();
        long used = 0;
        for (long i = 0; i < count; ++i) {
            if ((*augmentations)[i] > 0) {
                ++used;
                last = i;
            }
        }
        if (used > 0) {
            none = false;
            for (long i = 0; i < count; ++i) {
                if ((*augmentations)[i] > 0) {
                    text += CTCObjectAugmentations::GetAugmentationName((*augmentations)[i]);
                    if (i < last)
                        text += miscDef->CommaSeparator;
                }
            }
        }
    }
    if (none)
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
    text += NEWLINE;
    text += CTCInventoryItem::GetDetails(itemDefIndex);
}
