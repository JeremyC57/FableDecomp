// CTCInventoryWeapons::ConstructWeaponDescriptionNoDamageNorWeight(CWideString& text, long itemDefIndex, float damage, bool fromDef) const
//   -- appends a weapon item's augmentations line (the names of its filled augmentation slots, comma separated, or "none"; taken
//   from the def's initial augmentations when fromDef, else from the owned item's per-item data, returning at once when none is
//   owned), a newline and the item's details text. `damage` is unused (the NoDamage variant of ConstructWeaponDescription).
// Leaf: ?ConstructWeaponDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C71AA, 680 bytes, thiscall `ret 10h`, no EH frame; optimize("s",on).
// Identity: Ego_r.pdb ?ConstructWeaponDescriptionNoDamageNorWeight@CTCInventoryWeapons@@QBEXAAVCWideString@@JM_N@Z (egor donor
//   0x00626839, between ConstructWeaponDescription 0x006261B8 and ...Damage 0x00626ACE; the retail order 0x005C6B26 / 0x005C71AA /
//   0x005C7452); only caller GetItemDescriptionNoDamageNorWeight 0x005C7B10 (call at 0x005C7CD4).
// Callees: RefreshActiveWeaponData 0x005C30D3, CCharString ctor/dtor 0x0099EBF0 / 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390, GetDef<NUISystem::CUIMiscThingsDef> 0x0042B312,
//   CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0, GetSubDef<CWeaponDef> 0x0042B1A6,
//   GetSubDef<CObjectAugmentationsDef> 0x0042AF9E, vector<long>::resize 0x004428F0, CTCInventoryBase::GetNumberOfItemsOfTypeInInventory
//   0x005BDF08 / GetPerItemDataForItem 0x005BE8D3, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, operator+(CWideString) 0x0099BE70,
//   CWideString::operator+= 0x0099B8D0 / ~CWideString 0x0099B510, CTCObjectAugmentations::GetAugmentationName 0x00765F90,
//   CTCInventoryItem::GetDetails 0x005D9630, free 0x00BFEA14 (the local vector's storage).
// Data: "UI_MISC_THINGS_DEF" 0x01230018, "TEXT_GUI_MENU_AUGMENTATIONS" 0x01254568, "TEXT_GUI_MENU_NONE" 0x01254554;
//   CMainGameComponent instance 0x013B86A0 (text bank +0x14); NEWLINE 0x013B8BA8.
// Shape: the 0x005C6B26 ConstructWeaponDescription TU header and parts with the damage, value, weight and kills parts removed. The
//   one addition is FABLE_RETAIL_STLP_VECTOR (STLport's _vector.c _M_fill_insert body, which every retail TU calling resize() saw):
//   with resize's whole chain visible, VC7.1 knows the local per-item data's vector is never captured, so (a) the copy loop's
//   `data.Augmentations.size()` is hoisted out of the loop into the dead fromDef slot (0x005C727C `mov [ebp+14h],ecx`) and (b) the
//   not-owned early return jumps straight past the vector's free (0x005C72C3 `je 0x5C740F`: the vector is still empty there).
//   Declaration-only _M_fill_insert re-reads the size every pass and frees through the `cmp [ebp-38h],ebx` test. The "none" text is
//   written in both elses and VC7.1 cross-jumps the two copies (0x005C73A1), as in 0x005C6B26.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
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

// Template callee bodies. CDefClassBase::GetSubDef<T> and CDefinitionManager::GetDef<T> are member templates defined in the
// engine headers, so every retail TU that calls them instantiates them itself as COMDATs (the linker keeps one copy): retail's
// copies sit at 0x0042AF9E / 0x0042B1A6 / 0x0042B312, unaligned among the other GetSubDef/GetDef COMDATs, and are reached by
// `call`, i.e. this TU saw their bodies but did not inline them. The bodies come from the shared header
// engine/CDefinitionManager.h (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY and FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY,
// defined above; no copy in this file) and are the same code as the landed rows
// rebuild/src/compiled/00/42/CDefClassBase_GetSubDefCObjectAugmentationsDef_0042af9e.cpp
//   (??$GetSubDef@VCObjectAugmentationsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCObjectAugmentationsDef@@@@@Z),
// rebuild/src/compiled/00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp
//   (??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z) and
// rebuild/src/compiled/00/42/CDefinitionManager_GetDef0042b312_0042b312.cpp (landed under the placeholder name
//   GetDef_0042b312; PDB: ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z,
//   FableWin.pdb, NAME_FIXES.tsv of this row); the header keeps them out of line.
// Seeing them, VC7.1 knows they only write through the out pointer, so the CDefPointer locals whose addresses they take stay
// unaliased (retail's register/slot schedule depends on that).


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
    const NInventory::CPerItemData* itemData;
    if (fromDef) {
        CDefPointer<const CObjectAugmentationsDef> augmentationsDef;
        if (def->GetSubDef(&augmentationsDef)) {
            const CObjectAugmentationsDef* a = augmentationsDef.Get();
            data.Augmentations.resize(a->NumberOfSlots, 0);
            for (unsigned long i = 0; i < a->InitialAugmentations.size(); ++i) {
                if (i >= data.Augmentations.size())
                    break;
                data.Augmentations[i] = a->InitialAugmentations[i];
            }
        }
        itemData = &data;
    } else {
        if (GetNumberOfItemsOfTypeInInventory(itemDefIndex) == 0)
            return;
        itemData = GetPerItemDataForItem(itemDefIndex);
    }
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    text += bank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENTATIONS") + miscDef->SpaceSeparator;
    const std::vector<long>& augmentations = itemData->Augmentations;
    if (!augmentations.empty()) {
        long last = -1;
        long count = augmentations.size();
        long used = 0;
        for (long i = 0; i < count; ++i) {
            if (augmentations[i] > 0) {
                ++used;
                last = i;
            }
        }
        if (used > 0) {
            for (long i = 0; i < count; ++i) {
                if (augmentations[i] > 0) {
                    text += CTCObjectAugmentations::GetAugmentationName(augmentations[i]);
                    if (i < last)
                        text += miscDef->CommaSeparator;
                }
            }
        } else {
            text += bank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
        }
    } else {
        text += bank.GetTextBySymbol("TEXT_GUI_MENU_NONE");
    }
    text += NEWLINE;
    text += CTCInventoryItem::GetDetails(itemDefIndex);
}