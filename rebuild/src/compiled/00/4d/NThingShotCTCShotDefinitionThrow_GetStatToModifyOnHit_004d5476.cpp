// NThingShot::CTCShotDefinitionThrow::GetStatToModifyOnHit(EHeroExperienceStatCategory& category) -- a thrown shot trains stat category 1; returns true
// Leaf: ?GetStatToModifyOnHit@CTCShotDefinitionThrow@NThingShot@@UAE_NAAW4EHeroExperienceStatCategory@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D5476, length 12 bytes, `ret 4`. CTCShotDefinitionThrow vtable 0x0123CDC4
// slot 30 (also inherited by the derived shot vtables). Slot 30 is CTCShotDefinitionBase's virtual
// ?GetStatToModifyOnHit@CTCShotDefinitionBase@NThingShot@@UAE_NAAW4EHeroExperienceStatCategory@@@Z (Ego_r.pdb): the
// base override in CTCShotDefinitionDrainLife's slot 30 (0x004DB19B) is `xor al,al ; ret 4` and the fireball's
// (0x004DCB11) stores 2 and returns `mov al,1` -- an out-parameter category plus a bool, as the PDB signature says.
// Shape: size-optimised TU (packed address): the constant 1 is built once as `xor eax,eax ; inc eax` and serves both
// as the stored category and as the `true` in al.
#pragma optimize("s", on)
class CThing;
enum EHeroExperienceStatCategory { EHERO_EXPERIENCE_STAT_CATEGORY_0 = 0, EHERO_EXPERIENCE_STAT_CATEGORY_1 = 1 };

namespace NThingShot {

class CTCShotDefinitionThrow {
public:
    virtual ~CTCShotDefinitionThrow();          // slot 0
    virtual void _v01(); virtual void _v02(); virtual void _v03(); virtual void _v04(); virtual void _v05();
    virtual void _v06(); virtual void _v07(); virtual void _v08(); virtual void _v09(); virtual void _v10();
    virtual void _v11(); virtual void _v12(); virtual void _v13(); virtual void _v14(); virtual void _v15();
    virtual void _v16(); virtual void _v17(); virtual void _v18(); virtual void _v19(); virtual void _v20();
    virtual void _v21(); virtual void _v22(); virtual void _v23(); virtual void _v24(); virtual void _v25();
    virtual void _v26(); virtual void _v27(); virtual void _v28(); virtual void _v29();
    virtual bool GetStatToModifyOnHit(EHeroExperienceStatCategory& category);   // slot 30 (+0x78)
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

} // namespace NThingShot

bool NThingShot::CTCShotDefinitionThrow::GetStatToModifyOnHit(EHeroExperienceStatCategory& category)
{
    category = EHERO_EXPERIENCE_STAT_CATEGORY_1;
    return true;
}