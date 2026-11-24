// 007bfa90 NThingShot::CTCShotDefinitionBase::GetWeaponTrailGraphic
// Leaf: ?GetWeaponTrailGraphic@CTCShotDefinitionBase@NThingShot@@UBEJXZ
// Flags: none
#define FABLE_RETAIL_WEAPON_ARROW_TRAIL_PASS26
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_FORCEINLINE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_MEMBERS
#define FABLE_RETAIL_CTCWEAPON_ONCREATE
#define FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS
#define FABLE_RETAIL_CWEAPONTRAILGRAPHICSET_CLASS
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_NOINLINE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_ARROW_TREE_FIND_SIZE_OWNER_PASS26
#include "engine/EObjectAugmentationType.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGlobalDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCObjectAugmentations.h"
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"

namespace NThingShot {
long CTCShotDefinitionBase::GetWeaponTrailGraphic() const
{
    const CGlobalDef* pglob_def = &GetDefinitionManager().PeekGlobalDef();
    long gfxweapontrail = pglob_def->GraphicWeaponTrailArrow;
    const CTCWeapon* ptc_weapon;
    if (GetParentThing().PeekTC(&ptc_weapon)) {
        const CTCObjectAugmentations* ptc_object_augmentations;
        if (GetParentThing().PeekTC(&ptc_object_augmentations)) {
            long aug_type = 0;
            aug_type |= ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_SHARPENING) ? OBJECT_AUGMENTATION_TYPE_SHARPENING : 0;
            aug_type |= ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_SILVER) ? OBJECT_AUGMENTATION_TYPE_SILVER : 0;
            aug_type |= ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_FLAME) ? OBJECT_AUGMENTATION_TYPE_FLAME : 0;
            aug_type |= ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_LIGHTNING) ? OBJECT_AUGMENTATION_TYPE_LIGHTNING : 0;
            aug_type |= ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_PIERCING) ? OBJECT_AUGMENTATION_TYPE_PIERCING : 0;
            std::map<EObjectAugmentationType,long>::const_iterator findit =
                ptc_weapon->GetWeaponDef()->ArrowTrails.find(static_cast<EObjectAugmentationType>(aug_type));
            if (findit == ptc_weapon->GetWeaponDef()->ArrowTrails.end()) {
                bool found = false;
                if (ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_PIERCING)) {
                    findit = ptc_weapon->GetWeaponDef()->ArrowTrails.find(OBJECT_AUGMENTATION_TYPE_PIERCING);
                    if (findit != ptc_weapon->GetWeaponDef()->ArrowTrails.end()) {
                        gfxweapontrail = findit->second;
                        found = true;
                    }
                }
                if (!found && ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_LIGHTNING)) {
                    findit = ptc_weapon->GetWeaponDef()->ArrowTrails.find(OBJECT_AUGMENTATION_TYPE_LIGHTNING);
                    if (findit != ptc_weapon->GetWeaponDef()->ArrowTrails.end()) {
                        gfxweapontrail = findit->second;
                        found = true;
                    }
                }
                if (!found && ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_FLAME)) {
                    findit = ptc_weapon->GetWeaponDef()->ArrowTrails.find(OBJECT_AUGMENTATION_TYPE_FLAME);
                    if (findit != ptc_weapon->GetWeaponDef()->ArrowTrails.end()) {
                        gfxweapontrail = findit->second;
                        found = true;
                    }
                }
                if (!found && ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_SILVER)) {
                    findit = ptc_weapon->GetWeaponDef()->ArrowTrails.find(OBJECT_AUGMENTATION_TYPE_SILVER);
                    if (findit != ptc_weapon->GetWeaponDef()->ArrowTrails.end()) {
                        gfxweapontrail = findit->second;
                        found = true;
                    }
                }
                if (!found && ptc_object_augmentations->HasAugmentationOfType(OBJECT_AUGMENTATION_TYPE_SHARPENING)) {
                    findit = ptc_weapon->GetWeaponDef()->ArrowTrails.find(OBJECT_AUGMENTATION_TYPE_SHARPENING);
                    if (findit != ptc_weapon->GetWeaponDef()->ArrowTrails.end()) {
                        gfxweapontrail = findit->second;
                        found = true;
                    }
                }
            } else {
                gfxweapontrail = findit->second;
            }
        }
    }
    return gfxweapontrail;
}
}