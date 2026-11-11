// CTCInventoryWeapons::ConstructWeaponItemEntriesPC(std::vector<std::pair<long,long> >& items, NUISystem::CList* list, CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager, NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage, CCountedPointer<NUISystem::CComponent>& title) -- fills a PC weapon sub-list: splits the weapon items by the first item's kind (melee: light weapons, then the rest; ranged: weapons of type 3, then the rest) and hands both groups to FinalConstructWeaponItemEntriesPC with an empty wield entry, the second with the entry offset
// Leaf: ?ConstructWeaponItemEntriesPC@CTCInventoryWeapons@@AAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@AAV?$CCountedPointer@VCComponent@NUISystem@@@@PAVCComponent@5@PAVCManager@5@PAVCViewport@5@AAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@M2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C8176, 737 bytes, thiscall `ret 24h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructWeaponItemEntriesPC@CTCInventoryWeapons@@AAE... (private; called by the landed
//   ConstructScreenPC 0x005C9A95); Ego_r donor 0x0062C1D1. The PC twin of ConstructWeaponItemEntries 0x005C66B2: the same body with
//   a default-constructed (empty) wield entry instead of ConstructWieldEntry and the PC builder 0x005C47D5.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CWeaponDef> 0x0042B1A6,
//   CTCWeapon::IsMeleeWeaponClass 0x005DA510, vector<pair<long,long> >::push_back 0x005947B6, FinalConstructWeaponItemEntriesPC
//   0x005C47D5, CCountedPointer<CComponent>::ReleaseData 0x004291DE, free 0x00BFEA14.
// Shape: as 0x005C66B2 (same-TU template bodies; an MSVC-only noinline stand-in for the PC builder that only reads the items keeps
//   the second group's end in ebx across the first call; the empty wield entry lives in its own block).
// Name fix (agentNF7 2026-11-10): 0x0042AF3C is CDefinitionManager::GetDef<CThingBaseDef> (FableWin.pdb; NF5/NF6 STATUS): the out handle retyped CDefPointer<const CThingBaseDef> (shared engine/CThingBaseDef.h), so the template binds that instantiation.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCWeapon.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingBaseDef.h"

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CWeaponDef>
// 0x0042B1A6): VC7.1 sees that they only write through the out pointer, so the CDefPointer locals stay unaliased.
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

#if defined(_MSC_VER)
// Same-TU stand-in (MSVC build only) for the builder retail's TU defines above this row; its real body is the row 0x005C47D5.
void StandInBody();     // never defined: an empty stand-in would be folded away with its calls
#pragma auto_inline(off)
void CTCInventoryWeapons::FinalConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >& items, NUISystem::CList*,
    CCountedPointer<NUISystem::CComponent>&, NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*,
    CDefPointer<const NUISystem::CUIMiscThingsDef>&, float, CCountedPointer<NUISystem::CComponent>&, CCountedPointer<NUISystem::CComponent>&,
    unsigned long)
{
    if (items.size() > 0)
        StandInBody();
}
#pragma auto_inline(on)
#endif

void CTCInventoryWeapons::ConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >& items, NUISystem::CList* list,
    CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager,
    NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage,
    CCountedPointer<NUISystem::CComponent>& title)
{
    if (items.empty())
        return;
    CDefPointer<const CThingBaseDef> def;
    GetDefinitionManager().GetDef(items[0].first, &def);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        // melee: the light weapons first, then the heavy ones
        std::vector<std::pair<long, long> > lightItems;
        std::vector<std::pair<long, long> > otherItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            CDefPointer<const CThingBaseDef> itemDef;
            GetDefinitionManager().GetDef(items[i].first, &itemDef);
            CDefPointer<const CWeaponDef> itemWeaponDef;
            itemDef->GetSubDef(&itemWeaponDef);
            if (itemWeaponDef->Class == WC_LIGHT)
                lightItems.push_back(items[i]);
            else
                otherItems.push_back(items[i]);
        }
        {
            CCountedPointer<NUISystem::CComponent> wieldEntry;
            FinalConstructWeaponItemEntriesPC(lightItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry, 0);
            FinalConstructWeaponItemEntriesPC(otherItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry,
                                            lightItems.size());
        }
    } else {
        // ranged: the weapons of type 3 first, then the rest
        std::vector<std::pair<long, long> > typeItems;
        std::vector<std::pair<long, long> > otherItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            CDefPointer<const CThingBaseDef> itemDef;
            GetDefinitionManager().GetDef(items[i].first, &itemDef);
            CDefPointer<const CWeaponDef> itemWeaponDef;
            itemDef->GetSubDef(&itemWeaponDef);
            if (itemWeaponDef->Type == 3)
                typeItems.push_back(items[i]);
            else
                otherItems.push_back(items[i]);
        }
        {
            CCountedPointer<NUISystem::CComponent> wieldEntry;
            FinalConstructWeaponItemEntriesPC(typeItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry, 0);
            FinalConstructWeaponItemEntriesPC(otherItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry,
                                            otherItems.size());
        }
    }
}