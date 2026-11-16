// CTCHeroAttachableAppearanceModifiers::GetHiddenMaterialNames(std::set<CCharString>&) const -- copies HiddenMaterialNames (+0x24) into
// the caller's set (the tree's out-of-line operator= 0x006B9E00) and returns whether it holds any name (node count +0x28)
// Leaf: ?GetHiddenMaterialNames@CTCHeroAttachableAppearanceModifiers@@QBE_NAAV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706860.
// NF39 (2026-11-16): moved off its free __fastcall stand-in (CTCActivationReceptorBase_AddTrigger) onto the shared
// engine/CTCHeroAttachableAppearanceModifiers.h (opt-in _HIDDENMATERIALS, which names 0x00706860; overlay opt-in _HIDDENMATERIALS_TYPED gives
// the member its Ego_r.pdb type set<CCharString>): Ego_r.pdb + FableWin.pdb (_fablewin_api.tsv 0x0182FB77); called on TC 0x5E
// (TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS) with a set<CCharString>& at 0x004BD89A.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_HIDDENMATERIALS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_HIDDENMATERIALS_TYPED
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"

bool CTCHeroAttachableAppearanceModifiers::GetHiddenMaterialNames(std::set<CCharString>& names) const
{
    names = HiddenMaterialNames;
    return HiddenMaterialNames.size() != 0;
}
