// CTCObjectAugmentations::GetExperienceMultiplier() const -- product of the ExperienceMultiplier of every augmentation def attached to the object (1.0 with none)
// Leaf: GetExperienceMultiplier
// Grade: DIFFER(119v113) -- same sub-def release gap as 0x00766140: ours `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`, retail `mov ecx,[esp+0Ch] ... mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne` (tried dtor, pointer-local hand release, member accessor + member release)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766400, length 113 bytes. Catalog label "GetDamageMultiplier" is a
// byte-twin propagation: this copy multiplies ExperienceMultiplier (+0x38); the real GetDamageMultiplier is 0x00766140.
// Callees: CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248 (catalog <CAbilityDef> is a byte twin), the def's slot-1 release.
// Layout: PDB CTCObjectAugmentations: Augmentations +0x14 (vector<CDefPointer<CThingBaseDef const>>, one word each); CAugmentationDef
// Type +0x2C, DamageMultiplier +0x34, ExperienceMultiplier +0x38.
// Shape: `for (unsigned i = 0; i < size(); ++i)`, skip null defs, a zeroed CDefPointer<CAugmentationDef> per pass filled by
// GetSubDef; its object is taken into a pointer local and released by hand without a null test; the product a float local kept in the frame.
struct CAugmentationDef;
struct CDefClassBase {
    virtual void Destroy();
    virtual void Release();                     // slot 1
    long RefCount;                              // +0x04
    void ReleaseReference()
    {
        if (--RefCount == 0)
            Release();
    }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    T* operator->() const { return Object; }
};
struct CThingBaseDef : CDefClassBase {
    bool GetSubDef(CDefPointer<CAugmentationDef>* out) const;   // 0x0042B248 (GetSubDef<CAugmentationDef>)
};
struct CAugmentationDef : CDefClassBase {
    unsigned char _pad08[0x28 - 0x08];
    unsigned long Name;                         // +0x28
    long  Type;                                 // +0x2c EObjectAugmentationType (bit mask)
    unsigned long InventoryGraphic;             // +0x30
    float DamageMultiplier;                     // +0x34
    float ExperienceMultiplier;                 // +0x38
    float GetExperienceMultiplier() const { return ExperienceMultiplier; }
};
struct CThingBaseDefPointer {                   // CDefPointer<CThingBaseDef const>, one word
    const CThingBaseDef* Object;
};
template<class T> struct CRetailVector {        // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CTCObjectAugmentations {
    virtual void v00();
    void* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* PDef;                                 // +0x0c CDefPointer<CObjectAugmentationsDef const>
    bool LoadedFromSaveGame;                    // +0x10
    CRetailVector<CThingBaseDefPointer> Augmentations;   // +0x14
    float GetExperienceMultiplier() const;
};

float CTCObjectAugmentations::GetExperienceMultiplier() const
{
    float multiplier = 1.0f;
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        const CThingBaseDef* def = Augmentations[i].Object;
        if (def) {
            CDefPointer<CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            CAugmentationDef* subDef = augmentation.Object;
            multiplier *= subDef->GetExperienceMultiplier();
            subDef->ReleaseReference();
        }
    }
    return multiplier;
}
