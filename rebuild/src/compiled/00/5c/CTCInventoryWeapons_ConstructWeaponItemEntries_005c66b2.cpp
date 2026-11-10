// CTCInventoryWeapons::ConstructWeaponItemEntries(std::vector<std::pair<long,long> >& items, NUISystem::CList* list, CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager, NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage, CCountedPointer<NUISystem::CComponent>& title) -- fills a console weapon sub-list: splits the weapon items by the first item's kind (melee: light weapons, then the rest; ranged: weapons of type 3, then the rest), builds the wield entry and hands both groups to FinalConstructWeaponItemEntries, the second with the entry offset
// Leaf: ?ConstructWeaponItemEntries@CTCInventoryWeapons@@AAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@AAV?$CCountedPointer@VCComponent@NUISystem@@@@PAVCComponent@5@PAVCManager@5@PAVCViewport@5@AAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@M2@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C66B2, 758 bytes, thiscall `ret 24h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructWeaponItemEntries@CTCInventoryWeapons@@AAE... (private; the landed ConstructScreen
//   0x005C8459 calls it for the melee and ranged sub-lists); Ego_r donor 0x0062BB7A.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, GetDef<CDefClassBase> 0x0042AF3C, GetSubDef<CWeaponDef> 0x0042B1A6,
//   CTCWeapon::IsMeleeWeaponClass 0x005DA510, vector<pair<long,long> >::push_back 0x005947B6, ConstructWieldEntry 0x005C323A,
//   FinalConstructWeaponItemEntries 0x005C3E91, CCountedPointer<CComponent>::ReleaseData 0x004291DE, free 0x00BFEA14.
// Shape: the def lookups are the same-TU template bodies; FinalConstructWeaponItemEntries (which retail's TU defines above this row,
//   Ego_r donor order) gets an MSVC-only noinline stand-in that only reads the items, so VC7.1 keeps the second group's end pointer in
//   edi across the first call (`sub edi,[ebp-24h]`) as retail does; the wield entry lives in its own block (that lets the frame packer
//   overlap it with the loop's def pointers: the 0x30 frame). The two group vectors of each branch are zeroed above the branch
//   (hoisted identical code). The second call's entry offset is the size of the vector it builds in the ranged branch, as retail
//   computes it (`sub edi,[ebp-24h]` on the second group in both branches).
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

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CDefClassBase> 0x0042AF3C, GetSubDef<CWeaponDef>
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
// Same-TU stand-in (MSVC build only) for the builder retail's TU defines above this row; its real body is the row 0x005C3E91.
void StandInBody();     // never defined: an empty stand-in would be folded away with its calls
#pragma auto_inline(off)
void CTCInventoryWeapons::FinalConstructWeaponItemEntries(std::vector<std::pair<long, long> >& items, NUISystem::CList*,
    CCountedPointer<NUISystem::CComponent>&, NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*,
    CDefPointer<const NUISystem::CUIMiscThingsDef>&, float, CCountedPointer<NUISystem::CComponent>&, CCountedPointer<NUISystem::CComponent>&,
    unsigned long)
{
    if (items.size() > 0)
        StandInBody();
}
#pragma auto_inline(on)
#endif

void CTCInventoryWeapons::ConstructWeaponItemEntries(std::vector<std::pair<long, long> >& items, NUISystem::CList* list,
    CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager,
    NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage,
    CCountedPointer<NUISystem::CComponent>& title)
{
    if (items.empty())
        return;
    CDefPointer<const CDefClassBase> def;
    GetDefinitionManager().GetDef(items[0].first, &def);
    CDefPointer<const CWeaponDef> weaponDef;
    def->GetSubDef(&weaponDef);
    if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class)) {
        // melee: the light weapons first, then the heavy ones
        std::vector<std::pair<long, long> > lightItems;
        std::vector<std::pair<long, long> > otherItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            CDefPointer<const CDefClassBase> itemDef;
            GetDefinitionManager().GetDef(items[i].first, &itemDef);
            CDefPointer<const CWeaponDef> itemWeaponDef;
            itemDef->GetSubDef(&itemWeaponDef);
            if (itemWeaponDef->Class == WC_LIGHT)
                lightItems.push_back(items[i]);
            else
                otherItems.push_back(items[i]);
        }
        {
            CCountedPointer<NUISystem::CComponent> wieldEntry = ConstructWieldEntry(true, viewport);
            FinalConstructWeaponItemEntries(lightItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry, 0);
            FinalConstructWeaponItemEntries(otherItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry,
                                            lightItems.size());
        }
    } else {
        // ranged: the weapons of type 3 first, then the rest
        std::vector<std::pair<long, long> > typeItems;
        std::vector<std::pair<long, long> > otherItems;
        for (unsigned long i = 0; i < items.size(); ++i) {
            CDefPointer<const CDefClassBase> itemDef;
            GetDefinitionManager().GetDef(items[i].first, &itemDef);
            CDefPointer<const CWeaponDef> itemWeaponDef;
            itemDef->GetSubDef(&itemWeaponDef);
            if (itemWeaponDef->Type == 3)
                typeItems.push_back(items[i]);
            else
                otherItems.push_back(items[i]);
        }
        {
            CCountedPointer<NUISystem::CComponent> wieldEntry = ConstructWieldEntry(false, viewport);
            FinalConstructWeaponItemEntries(typeItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry, 0);
            FinalConstructWeaponItemEntries(otherItems, list, augmentationEntry, parent, manager, viewport, miscDef, damage, title, wieldEntry,
                                            otherItems.size());
        }
    }
}