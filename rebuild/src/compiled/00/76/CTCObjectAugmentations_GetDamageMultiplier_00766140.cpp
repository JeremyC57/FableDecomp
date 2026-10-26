// CTCObjectAugmentations::GetDamageMultiplier() const -- product of the DamageMultiplier of every augmentation def attached to the object (1.0 with none)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766140, length 113 bytes.
// Callees: CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248 (catalog <CAbilityDef> is a byte twin), the def's slot-1 release.
// Layout: PDB CTCObjectAugmentations: Augmentations +0x14 (vector<CDefPointer<CThingBaseDef const>>, one word each); CAugmentationDef
// Type +0x2C, DamageMultiplier +0x34, ExperienceMultiplier +0x38.
// Shape: `for (unsigned i = 0; i < size(); ++i)`, skip null defs, a zeroed CDefPointer<CAugmentationDef> per pass filled by
// GetSubDef and released by its inline dtor without a null test; the product a float local kept in the frame.
class CAugmentationDef;
class CDefClassBase { public:
    virtual void Destroy();
    virtual void Release();                     // slot 1
    long RefCount;                              // +0x04
    void ReleaseReference()
    {
        if (--RefCount == 0)
            Release();
    }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        Object->ReleaseReference();
    }
    T* operator->() const { return Object; }
};
class CThingBaseDef : public CDefClassBase { public:
    bool GetSubDef(CDefPointer<CAugmentationDef>* out) const;   // 0x0042B248 (GetSubDef<CAugmentationDef>)
};
void* FindDef_CThingBaseDef_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<CAugmentationDef>* out) const
{
    void* def = FindDef_CThingBaseDef_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CAugmentationDef : public CDefClassBase { public:
    unsigned char _pad08[0x28 - 0x08];
    unsigned long Name;                         // +0x28
    long  Type;                                 // +0x2c EObjectAugmentationType (bit mask)
    unsigned long InventoryGraphic;             // +0x30
    float DamageMultiplier;                     // +0x34
    float ExperienceMultiplier;                 // +0x38
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
class CTCObjectAugmentations { public:
    virtual void v00();
    void* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    void* PDef;                                 // +0x0c CDefPointer<CObjectAugmentationsDef const>
    bool LoadedFromSaveGame;                    // +0x10
    CRetailVector<CThingBaseDefPointer> Augmentations;   // +0x14
    float GetDamageMultiplier() const;
};

float CTCObjectAugmentations::GetDamageMultiplier() const
{
    float multiplier = 1.0f;
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        const CThingBaseDef* def = Augmentations[i].Object;
        if (def) {
            CDefPointer<CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            multiplier *= augmentation->DamageMultiplier;
        }
    }
    return multiplier;
}