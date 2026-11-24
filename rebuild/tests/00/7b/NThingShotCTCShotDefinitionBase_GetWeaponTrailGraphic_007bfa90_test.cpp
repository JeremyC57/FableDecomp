#include <stdio.h>
#include <map>
enum EObjectAugmentationType {OBJECT_AUGMENTATION_TYPE_NONE=0,OBJECT_AUGMENTATION_TYPE_SHARPENING=1,
    OBJECT_AUGMENTATION_TYPE_SILVER=2,OBJECT_AUGMENTATION_TYPE_FLAME=4,OBJECT_AUGMENTATION_TYPE_LIGHTNING=8,
    OBJECT_AUGMENTATION_TYPE_PIERCING=16};
struct CGlobalDef {long GraphicWeaponTrailArrow;};
struct CGameDefinitionManager {CGlobalDef d;const CGlobalDef& PeekGlobalDef()const{return d;}};
struct CWeaponDef{std::map<EObjectAugmentationType,long> ArrowTrails;};
struct CTCWeapon{CWeaponDef def;const CWeaponDef* GetWeaponDef()const{return &def;}};
struct CTCObjectAugmentations{unsigned bits;bool HasAugmentationOfType(EObjectAugmentationType t)const{return (bits & t)!=0;}};
struct CThing{CTCWeapon w; CTCObjectAugmentations a;bool weapon,aug;
    bool PeekTC(const CTCWeapon** p)const{*p=weapon?&w:0;return weapon;}
    bool PeekTC(const CTCObjectAugmentations** p)const{*p=aug?&a:0;return aug;}};
namespace NThingShot{
struct CTCShotDefinitionBase{
    CGameDefinitionManager mgr;CThing parent;
    const CGameDefinitionManager& GetDefinitionManager() const{return mgr;}
    const CThing& GetParentThing() const{return parent;}
    long GetWeaponTrailGraphic() const;
};
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
int main(){
    for(unsigned mask=0;mask<32;++mask)for(unsigned entries=0;entries<32;++entries)
    for(unsigned add=0;add<2;++add)for(unsigned present=0;present<4;++present) {
        NThingShot::CTCShotDefinitionBase shot;
        shot.mgr.d.GraphicWeaponTrailArrow=-24;shot.parent.a.bits=mask;
        shot.parent.weapon=(present&1)!=0;shot.parent.aug=(present&2)!=0;
        std::map<EObjectAugmentationType,long>& defs=shot.parent.w.def.ArrowTrails;
        for(unsigned bit=1;bit<=16;bit*=2)if(entries&bit)defs[(EObjectAugmentationType)bit]=51+bit;
        if(add)defs[(EObjectAugmentationType)mask]=107;
        long expected=-24;
        if(present==3) {
            for(unsigned bit=1;bit<=16;bit*=2)if((mask&entries&bit)!=0)expected=51+bit;
            if(add)expected=107;
        }
        if(shot.GetWeaponTrailGraphic()!=expected)return 1;
    }
    puts("007bfa90_TEST PASS");return 0;
}