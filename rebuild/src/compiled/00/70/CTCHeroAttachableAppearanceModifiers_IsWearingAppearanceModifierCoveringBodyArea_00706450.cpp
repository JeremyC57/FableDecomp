// Flags: 
// 00706450 CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea
// Leaf: ?IsWearingAppearanceModifierCoveringBodyArea@CTCHeroAttachableAppearanceModifiers@@QBE_NJW4EHeroAttachableAppearanceModifierType@@@Z
#define FABLE_RETAIL_HERO_MODIFIER_GRAPHICS_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGOBJECTDEF_SIZE_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
// Inline-budget: both calls expand HasTC and call bitset::test. Ordinary inline kept HasTC out of line at both sites.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CBASE_CLASS_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PERSONALITY
#define FABLE_RETAIL_CHEROSTATSDEF_APPEARANCESCALING
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_LIST
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_COVERS
#define FABLE_RETAIL_CTCHEROATTACHABLEAPPEARANCEMODIFIERS_TCBASE
#define FABLE_RETAIL_CTCHEROMORPH
#define FABLE_RETAIL_HERO_MORPH_SERIALISE_PASS29
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_MEMBERS
#define FABLE_RETAIL_CTATTOODEF_TYPEBALANCED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CDEFPOINTER
#define FABLE_RETAIL_CTHINGOBJECTDEF
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_HERO_APPEARANCE_UPDATE_PASS30

#include "engine/CThing.h"
#include "engine/CHeroStatsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CTCHeroAttachableAppearanceModifiers.h"

#include "engine/CTattooDef.h"
#include "engine/CWorld.h"

bool CTCHeroAttachableAppearanceModifiers::IsWearingAppearanceModifierCoveringBodyArea(long bodyAreaFlags, EHeroAttachableAppearanceModifierType type) const
{
    const std::vector<CModifierEntry>& entries = AppearanceModifiers[type];
    for (const CModifierEntry* it = entries.begin(); it != entries.end(); ++it) {
        CDefPointer<const CThingObjectDef> thingDef;
        CDefPointer<const CAppearanceModifierDef> appearanceDef;
        if (GetDefinitionManager().GetDef(it->DefIndex, &thingDef) && thingDef->GetSubDef(&appearanceDef) && (appearanceDef->CoversBodyAreaFlags & bodyAreaFlags))
            return true;
    }
    return false;
}
// Fidelity: VC7.1 instantiates the visible GetSubDef<CAppearanceModifierDef>
// at the end of this TU. Its 162-byte native owner uses a frame and size mode.
#ifdef _MSC_VER
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
