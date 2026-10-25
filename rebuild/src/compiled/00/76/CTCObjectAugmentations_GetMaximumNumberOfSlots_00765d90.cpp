// CTCObjectAugmentations::GetMaximumNumberOfSlots() const -- the def's MaximumNumberOfSlots
// Leaf: ?GetMaximumNumberOfSlots@CTCObjectAugmentations@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765D90, 7 bytes: `mov eax,[ecx+0Ch] ; mov eax,[eax+2Ch] ; ret`.
// Identity (FINDINGS chunk 393: "landed with a wrong name: 0x765D90 GetMaximumNumberOfSlots (as CountReferences)"; the
//   earlier source was an anonymous `C_00765d90::M()` model): Ego_r.pdb
//   `?GetMaximumNumberOfSlots@CTCObjectAugmentations@@QBEJXZ` (long, const); struct_layouts_egor:
//   CTCObjectAugmentations::PDef = CDefPointer<CObjectAugmentationsDef const> at +0x0C and
//   CObjectAugmentationsDef::MaximumNumberOfSlots at +0x2C (NumberOfSlots +0x28); its one caller is
//   CTCInventoryBase::CreateInventoryObject 0x005BE8F3 (0x005BE9F0), beside GetNumberOfSlots 0x00765D80.
// Model: the classes are declared locally (no engine headers); CDefPointer's `->` is the inline pointee cast.
class CDefPointeeBase {
public:
    virtual void v00();
};

class CObjectAugmentationsDef : public CDefPointeeBase {
public:
    unsigned char _pad04[0x24];                 // +0x04 CDefClassBase and earlier members
    long NumberOfSlots;                         // +0x28
    long MaximumNumberOfSlots;                  // +0x2c
};

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
    const T* operator->() const { return static_cast<const T*>(Object); }
};

class CTCObjectAugmentations {
public:
    virtual void v00();
    void*                                      ParentThing;            // +0x04 (CTCBase)
    bool                                       ToBeRemoved;            // +0x08
    bool                                       IsInGlobalUpdateTCs;    // +0x09
    CDefPointer<const CObjectAugmentationsDef> PDef;                   // +0x0c
    long GetMaximumNumberOfSlots() const;
};

long CTCObjectAugmentations::GetMaximumNumberOfSlots() const
{
    return PDef->MaximumNumberOfSlots;
}
