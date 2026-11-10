// CTCInventoryWeapons::ConstructAugmentationsLeftDescription(CWideString& text, std::vector<std::pair<long, long> >& augmentations, bool melee) -- appends one "<augmentation title> x<count>" line per augmentation type the hero owns (counts summed per type through a map), then the melee or ranged augment guide text
// Leaf: ?ConstructAugmentationsLeftDescription@CTCInventoryWeapons@@QAEXAAVCWideString@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C4F42, 1514 bytes, thiscall `ret 0Ch`, no EH frame; biased frame (`push ebp ; lea ebp,[esp-6Ch] ;
//   sub esp,0D8h`): optimize("s",on), as the landed screens of this TU (0x005C8459).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructAugmentationsLeftDescription@CTCInventoryWeapons@@QAEXAAVCWideString@@AAV?$vector@...@_N@Z
//   (Ego_r donor 0x00629E9C, between RefreshWeaponDescriptions and ConstructAugEntry, the retail order); callers
//   RefreshAugmentationsLeftDescriptions 0x005C69AA (true for the "_M" components, false for "_R") and ConstructAugEntry(PC)
//   0x005C574E / 0x005C5F11. The bool picks TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_AUGMENT (true) / _RANGED_AUGMENT (false).
// Callees: CTCInventoryStats::GetXString 0x005CC772, map<EObjectAugmentationType,long>() 0x004DF8E9, CTCBase::GetDefinitionManager
//   0x006869D0, GetDef<CDefClassBase> 0x0042AF3C, GetSubDef<CAugmentationDef> 0x0042B248, the tree's _M_find 0x005CC04A /
//   insert_unique 0x004E9442 / destructor 0x004DD3AD, _Rb_global<bool>::_M_increment 0x004292C0, CCharString(const char*, long)
//   0x0099EBF0, GFIntToWideString 0x00405F00, CDataBank::GetTextBySymbol 0x009C95E0, operator+(CWideString) 0x0099BE70,
//   CWideString::operator+= 0x0099B8D0, ~CWideString 0x0099B510, ~CCharString 0x0099EAE0.
// Data: 0x013B8BA8 `anonymous namespace'::NEWLINE (L"\n"); title symbols 0x01254410 .. 0x012542D8, guide symbols 0x012542AC / 0x01254280.
// Shape: the def lookups are the same-TU template bodies (as the landed 0x005C7452 / 0x005C8459), so the def pointers stay unaliased;
//   the absent type is inserted through std::make_pair (the pair<EObjectAugmentationType,long> temporary converted to the map's
//   value_type: VC7.1 then loads the count once before the found/absent branch, retail's `mov ecx,[ecx+ebx*8+4]` at 0x005C4FC7);
//   the guide symbol is a named CCharString (its own frame node: retail's 0xD8 frame, the CCharString at [ebp+50h]); the switch
//   cases are in retail's block order (the per-case bit values: 8,4,2,1,0x10,0x80,0x40,0x20).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CAugmentationDef.h"
#include "stlport/fable_map.h"
#include "engine/EObjectAugmentationType.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

namespace { extern const CWideString NEWLINE; }                  // 0x013B8BA8 (`anonymous namespace'::NEWLINE of this TU, L"\n")

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CDefClassBase> 0x0042AF3C, GetSubDef<CAugmentationDef>
// 0x0042B248): VC7.1 sees that they only write through the out pointer, so the CDefPointer locals stay unaliased.
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

void CTCInventoryWeapons::ConstructAugmentationsLeftDescription(CWideString& text, std::vector<std::pair<long, long> >& augmentations, bool melee)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    CWideString x = CTCInventoryStats::GetXString();
    // count the owned augmentations per type
    std::map<EObjectAugmentationType, long> counts;
    for (unsigned long i = 0; i < augmentations.size(); ++i) {
        CDefPointer<const CDefClassBase> def;
        GetDefinitionManager().GetDef(augmentations[i].first, &def);
        CDefPointer<const CAugmentationDef> augmentationDef;
        if (def->GetSubDef(&augmentationDef)) {
            std::map<EObjectAugmentationType, long>::iterator it = counts.find(augmentationDef->Type);
            if (it != counts.end())
                it->second += augmentations[i].second;
            else
                counts.insert(std::make_pair(augmentationDef->Type, augmentations[i].second));
        }
    }
    // one "<title> x<count>" line per type
    for (std::map<EObjectAugmentationType, long>::iterator it = counts.begin(); it != counts.end(); ++it) {
        if (it->second > 0) {
            switch (it->first) {
            case OBJECT_AUGMENTATION_TYPE_LIGHTNING:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_LIGHTNING_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_FLAME:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_FLAME_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_SILVER:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_SILVER_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_SHARPENING:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_SHARPENING_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_PIERCING:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_PIERCING_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_EXPERIENCE:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_EXPERIENCE_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_WILL_REGENERATION:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_WILL_REGENERATION_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            case OBJECT_AUGMENTATION_TYPE_HEALTH_REGENERATION:
                text += bank.GetTextBySymbol("TEXT_OBJECT_AUGMENTATION_HEALTH_REGENERATION_TITLE") + x + GFIntToWideString(it->second) + NEWLINE;
                break;
            }
        }
    }
    CCharString guide(melee ? "TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_AUGMENT" : "TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_AUGMENT");
    text += bank.GetTextBySymbol(guide);
}