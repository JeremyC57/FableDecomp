// CTCObjectAugmentations::GetNumberOfSlots() const -- the number of augmentation slots: Augmentations.size()
// Leaf: ?GetNumberOfSlots@CTCObjectAugmentations@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00765D80, 10 bytes: `mov eax,[ecx+18h] ; sub eax,[ecx+14h] ; sar eax,2 ; ret`.
// Identity (FINDINGS chunk 393: "landed with a wrong name: 0x765D80 GetNumberOfSlots (as CShaderPreParser::
//   GetConstantCount)"; the earlier name came from a tiny-body shape generator): Ego_r.pdb
//   `?GetNumberOfSlots@CTCObjectAugmentations@@QBEJXZ` (long, const); struct_layouts_egor CTCObjectAugmentations::
//   Augmentations = vector<CDefPointer<CThingBaseDef const> > at +0x14 (its STLport start / finish at +0x14 / +0x18 in
//   retail, 4-byte elements); its one caller CTCInventoryBase::CreateInventoryObject 0x005BE8F3 (0x005BEA21) also calls
//   the sibling GetMaximumNumberOfSlots 0x00765D90.
// Model: the class is declared locally (no engine header); the vector is STLport's three-pointer layout.
struct CThingBaseDef;
class CDefPointeeBase;
class CObjectAugmentationsDef;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;
};

namespace std {
template <class _Tp> class vector {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _Tp* _M_end_of_storage;                     // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
};
}

class CTCObjectAugmentations {
public:
    virtual void v00();
    void*                                          ParentThing;        // +0x04 (CTCBase)
    bool                                           ToBeRemoved;        // +0x08
    bool                                           IsInGlobalUpdateTCs;// +0x09
    CDefPointer<const CObjectAugmentationsDef>     PDef;               // +0x0c
    bool                                           LoadedFromSaveGame; // +0x10
    std::vector<CDefPointer<const CThingBaseDef> > Augmentations;      // +0x14
    long GetNumberOfSlots() const;
};

long CTCObjectAugmentations::GetNumberOfSlots() const
{
    return Augmentations.size();
}
