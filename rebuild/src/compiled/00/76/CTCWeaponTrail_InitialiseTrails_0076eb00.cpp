// CTCWeaponTrail::InitialiseTrails() -- once the owner's graphics are loaded, collect the weapon-trail helper pairs: weapon_pos_a/b and weapon_pos_c/d when the mesh has them, then every further "weapon_pos_*" helper that occurs exactly twice; a projectile trail when the weapon def's two projectile helpers exist; then size the per-trail state vectors
// Inline-budget: /d2inlT65 (band T65..T67; default and T68+ expand the loop's vector<CCharString>::push_back, T60..T64 call more sites):
//   retail expands the first four push_backs (0x0076EB64..0x0076ECAE) and calls 0x0044BFF0 in the helper-name loop (0x0076EDB9).
// Leaf: ?InitialiseTrails@CTCWeaponTrail@@AAEXXZ
// Flags: /d2inlT65
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076EB00, 1080 bytes, thiscall, plain `ret` (ebp frame aligned to 8 for the locals).
// Identity: Ego_r.pdb ?InitialiseTrails@CTCWeaponTrail@@AAEXXZ (private); called by the landed OnCreate 0x0076EF40 and
//   UpdateWeaponTrails 0x0076EFA0 (`NoTrails == -1`).
// Callees: CTCGraphicAppearance::IsResourcesLoaded 0x004BE3E0 / HasHelperPoint 0x004BE170 / GetAllHelperPointNames 0x004BDA30 /
//   GetNoHelperPoints 0x004BDE90, vector<CCharString>::_M_insert_overflow 0x00433530 / push_back 0x0044BFF0,
//   vector<pair<CCharString,CCharString> >::push_back 0x007706A0, __find<CCharString*,CCharString> 0x004B8E40, strncmp 0x00BFEAF8,
//   CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6, CTCBase::GetDefinitionManager 0x006869D0, vector<bool>::_M_fill_insert
//   0x004676EF, vector<long>::_M_fill_insert 0x00440F00, vector<list<CWeaponTrailEntry> >::resize 0x00770D60, memmove 0x00BFEAE6.
// Data: START_WEAPON_POINT_NAME "weapon_pos_a", END_WEAPON_POINT_NAME "weapon_pos_b", START_WEAPON_POINT_NAME_2 "weapon_pos_c",
//   END_WEAPON_POINT_NAME_2 "weapon_pos_d"
//   (the landed static initialisers 0x0121E640..0x0121E6A0); the prefix literal "weapon_pos_" 0x0126A5A4.
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/NGraphicAppearance_CTCGraphicAppearance.h opt-in FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES.
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBAL
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#include "engine/CThing.h"
#include "engine/CTCWeaponTrail.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "stlport/fable_algorithm.h"

extern const CCharString START_WEAPON_POINT_NAME;   // "weapon_pos_a"
extern const CCharString END_WEAPON_POINT_NAME;   // "weapon_pos_b"
extern const CCharString START_WEAPON_POINT_NAME_2;   // "weapon_pos_c"
extern const CCharString END_WEAPON_POINT_NAME_2;   // "weapon_pos_d"

void CTCWeaponTrail::InitialiseTrails()
{
    NGraphicAppearance::CTCGraphicAppearance* appearance = GetParentThing().GetTCGraphicAppearance();
    if (!appearance->IsResourcesLoaded(true))
        return;
    NoTrails = 0;
    NoProjectileTrails = 0;
    std::vector<CCharString> usedHelpers;
    if (appearance->HasHelperPoint(START_WEAPON_POINT_NAME) || appearance->HasHelperPoint(END_WEAPON_POINT_NAME)) {
        usedHelpers.push_back(START_WEAPON_POINT_NAME);
        usedHelpers.push_back(END_WEAPON_POINT_NAME);
        HelperNames.push_back(std::pair<CCharString, CCharString>(START_WEAPON_POINT_NAME, END_WEAPON_POINT_NAME));
        ++NoTrails;
    }
    if (appearance->HasHelperPoint(START_WEAPON_POINT_NAME_2) || appearance->HasHelperPoint(END_WEAPON_POINT_NAME_2)) {
        usedHelpers.push_back(START_WEAPON_POINT_NAME_2);
        usedHelpers.push_back(END_WEAPON_POINT_NAME_2);
        HelperNames.push_back(std::pair<CCharString, CCharString>(START_WEAPON_POINT_NAME_2, END_WEAPON_POINT_NAME_2));
        ++NoTrails;
    }
    // any other weapon_pos_* helper that comes as a pair on the mesh is a trail of its own
    std::vector<CCharString> helperNames;
    appearance->GetAllHelperPointNames(helperNames);
    for (std::vector<CCharString>::iterator it = helperNames.begin(); it != helperNames.end(); ++it) {
        if (it->StartsWith("weapon_pos_")) {
            if (std::find(usedHelpers.begin(), usedHelpers.end(), *it) == usedHelpers.end() &&
                appearance->GetNoHelperPoints(*it, 1.0f) == 2) {
                HelperNames.push_back(std::pair<CCharString, CCharString>(*it, *it));
                ++NoTrails;
                usedHelpers.push_back(*it);
            }
        }
    }
    CDefPointer<const CWeaponDef> weaponDef;
    if (GetParentThing().PeekBaseDef().GetSubDef(&weaponDef) &&
        appearance->HasHelperPoint(weaponDef->ProjectileWeaponHelperNameBase) &&
        appearance->HasHelperPoint(weaponDef->ProjectileWeaponHelperNameTip))
        NoProjectileTrails = 1;
    ActiveTrail.resize(NoTrails, false);
    InactiveTicks.resize(NoTrails, GetDefinitionManager().PeekGlobalDef().WeaponTrialNumFramesToFadeOut);
    Trail.resize(NoTrails);
}