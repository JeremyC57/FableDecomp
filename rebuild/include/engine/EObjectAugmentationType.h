#ifndef FABLE_ENGINE_EOBJECTAUGMENTATIONTYPE_H
#define FABLE_ENGINE_EOBJECTAUGMENTATIONTYPE_H
// EObjectAugmentationType (new shared header, chunk 1002b): Ego_r.pdb global enum (mangled W4EObjectAugmentationType@@, e.g. the
// map<EObjectAugmentationType,long> insert_unique 0x004E9442). Retail values are single bits; their meaning comes from the title text
// each one selects in CTCInventoryWeapons::ConstructAugmentationsLeftDescription 0x005C4F42 (TEXT_OBJECT_AUGMENTATION_<X>_TITLE).
// The PDB name set carries no enumerator names, so the enumerators are descriptive.
enum EObjectAugmentationType {
    OBJECT_AUGMENTATION_TYPE_NONE               = 0,
    OBJECT_AUGMENTATION_TYPE_SHARPENING         = 0x01,
    OBJECT_AUGMENTATION_TYPE_SILVER             = 0x02,
    OBJECT_AUGMENTATION_TYPE_FLAME              = 0x04,
    OBJECT_AUGMENTATION_TYPE_LIGHTNING          = 0x08,
    OBJECT_AUGMENTATION_TYPE_PIERCING           = 0x10,
    OBJECT_AUGMENTATION_TYPE_HEALTH_REGENERATION = 0x20,
    OBJECT_AUGMENTATION_TYPE_WILL_REGENERATION  = 0x40,
    OBJECT_AUGMENTATION_TYPE_EXPERIENCE         = 0x80
};
#endif // FABLE_ENGINE_EOBJECTAUGMENTATIONTYPE_H
