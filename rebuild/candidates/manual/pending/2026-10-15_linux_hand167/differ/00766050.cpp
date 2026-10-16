// CTCObjectAugmentations::HasCombinationOfAugmentations(long typeMask) const -- whether the OR of the Type bits of every attached augmentation def covers all the requested bits
// Leaf: HasCombinationOfAugmentations
// Grade: DIFFER(116v110) -- sub-def release gap of 0x00766140 (ours `mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`, retail `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne`), which also swaps this/index into edi/ebx (retail esi/edi); tried dtor, scoped holder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00766050, length 110 bytes (`ret 4`).
// Callees: CDefClassBase::GetSubDef<CAugmentationDef> @ 0x0042B248 (catalog <CAbilityDef> is a byte twin), the def's slot-1 release.
// Layout: PDB CTCObjectAugmentations: Augmentations +0x14 (vector<CDefPointer<CThingBaseDef const>>, one word each); CAugmentationDef
// Type +0x2C.
// Shape: `for (unsigned i = 0; i < size(); ++i)`, skip null defs, a zeroed CDefPointer<CAugmentationDef> per pass filled by GetSubDef
// and released by its inline dtor without a null test.
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
    ~CDefPointer()
    {
        Object->ReleaseReference();
    }
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
    bool HasCombinationOfAugmentations(long typeMask) const;
};

bool CTCObjectAugmentations::HasCombinationOfAugmentations(long typeMask) const
{
    long combined = 0;
    for (unsigned long i = 0; i < Augmentations.size(); ++i) {
        const CThingBaseDef* def = Augmentations[i].Object;
        if (def) {
            CDefPointer<CAugmentationDef> augmentation;
            def->GetSubDef(&augmentation);
            combined |= augmentation->Type;
        }
    }
    return (combined & typeMask) == typeMask;
}
