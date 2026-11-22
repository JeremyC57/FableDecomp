// CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions() -- rebuild the "augmentations left" texts of the weapons list: collect the augmentation items, then write the console and PC descriptions into every matching child
// Leaf: ?RefreshAugmentationsLeftDescriptions@CTCInventoryWeapons@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C69AA, 380 bytes; size TU.
// Callees: NInventory::CTCInventoryBase::GetAllItems 0x005BF482, CTCInventoryItem::IsAugmentation 0x005D9870, vector<pair<long,long> >::push_back
//   0x005947B6 (out of line), list<CComponent*> ctor 0x0042ABAF / dtor 0x0042ABCA (out of line), CComponent::FindAllChildren (slot 5),
//   CWideString() 0x0099AED0 / ~ 0x0099B510, ConstructAugmentationsLeftDescription 0x005C4F42, CText::SetValue (slot 144), free 0x00BFEA14.
// Name fix (agentNF89, q11171): moved off the row-local CTCInventoryWeapons / CItemVector / CComponentList / CComponent / allocator models onto
//   engine/CTCInventoryWeapons.h (METHODS), NInventory_CTCInventoryBase.h and stlport/fable_containers.h, so 0x005BF482 binds
//   ?GetAllItems@CTCInventoryBase@NInventory@@QBEXAAV?$vector@U?$pair@JJ@std@@..@std@@@Z, 0x005947B6 the STLport vector push_back,
//   0x0042ABAF / 0x0042ABCA the STLport list<CComponent*> ctor / dtor and 0x005C4F42 its header spelling.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_ISAUGMENTATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
// NF241 (2026-11-22, the list<CComponent*> family pass): the list calls bind _List_base<CComponent*>'s constructor 0x0042ABAF and
//   destructor 0x0042ABCA (was FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE + _DTOR_OUTOFLINE, binding list's own names there; retail
//   0x0042B6A6 / 0x0042B6D2 are list's size-optimised wrappers, a `call 0x0042ABAF` and a 5-byte `jmp 0x0042ABCA`). Same bytes.
#define FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryItem.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CText.h"
namespace {                                                         // this TU's anonymous-namespace strings (Ego_r.pdb initializer names)
extern const CCharString AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M;   // 0x013B8BB8 "Augmentations_Left_Description_M"
extern const CCharString AUGMENTATIONS_LEFT_DESCRIPTION_STRING_R;   // 0x013B8BBC "Augmentations_Left_Description_R"
}
void CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions()
{
    if (WeaponsList) {
        std::vector<std::pair<long, long> > items;
        std::vector<std::pair<long, long> > augmentations;
        GetAllItems(items);
        for (unsigned long i = 0; i < items.size(); ++i)
            if (CTCInventoryItem::IsAugmentation(items[i].first))
                augmentations.push_back(items[i]);
        std::list<NUISystem::CComponent*> components;
        WeaponsList->FindAllChildren(AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M, components);
        for (std::list<NUISystem::CComponent*>::iterator it = components.begin(), end = components.end(); it != end; ++it) {
            NUISystem::CComponent* component = *it;
            CWideString text;
            ConstructAugmentationsLeftDescription(text, augmentations, true);
            static_cast<NUISystem::CText*>(component)->SetValue(text, true, true);
        }
        WeaponsList->FindAllChildren(AUGMENTATIONS_LEFT_DESCRIPTION_STRING_R, components);
        for (std::list<NUISystem::CComponent*>::iterator it2 = components.begin(), end2 = components.end(); it2 != end2; ++it2) {
            NUISystem::CComponent* component = *it2;
            CWideString text;
            ConstructAugmentationsLeftDescription(text, augmentations, false);
            static_cast<NUISystem::CText*>(component)->SetValue(text, true, true);
        }
    }
}
