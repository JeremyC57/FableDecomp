// CThingObject::IsCrop() const -- true when bit 1 of the object's property word is set
// Leaf: ?IsCrop@CThingObject@@UBE_NXZ
// Retail: Fable.exe @ 0x00702CB0, 14 bytes: `mov eax,[ecx] ; call [eax+0C0h] ; shr eax,1 ; and eax,1 ; ret`
// (CThingObject vtable 0x01264DCC slot 49; slot 48 = the landed CThingObject::GetProperties 0x00702FF0).
// Identity (probable, by elimination): slot 49 is a CThingPhysical bool virtual with no arguments whose CThingPhysical body
// (0x00529750) returns false and that only CThingObject overrides (rtti-vtables.tsv: no other thing class replaces slot 49).
// The PDB name sets give CThingObject exactly five no-argument bool overrides that CThingPhysical also defines out of line
// (IsCrop, IsFragile, CanBlockNavigatingCreature, CanBlockUpperLevelNavigatingCreature, UseHighDetailQuadTree); slots 54
// (def bit IsFragile, +0x98 bit 0), 55 (tail call of CThingPhysical::CanBlockNavigatingCreature 0x006A4D90), 56 and 57
// (UseHighDetailQuadTree) hold the other four, which leaves IsCrop for slot 49. The manifest label "vfunc_49" is a placeholder.
// Shape: the property word is fetched through the virtual (not the def), shifted and masked in the full register.
class CThingObject { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual unsigned long GetProperties() const;        // slot 48 (0x00702FF0)
    virtual bool IsCrop() const;                        // slot 49
};

bool CThingObject::IsCrop() const
{
    return (GetProperties() >> 1) & 1;
}