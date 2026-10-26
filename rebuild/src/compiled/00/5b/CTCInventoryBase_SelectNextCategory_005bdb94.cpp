// CTCInventoryBase::SelectNextCategory() -- with more than one category and no selection in progress, deselects the current category, advances (wrapping) and starts the category slide
// Leaf: SelectNextCategory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDB94, 116 bytes; size TU without a frame pointer here.
// Shape: `size()` is the STLport unsigned `size_type(finish - start)` (signed `idiv` by 0x2C, unsigned compares);
// the current category's Selected flag is cleared only when set; the index wraps with `>= size()` to `0`
// (`and [esi+70h],0`); the slide is Movement 1.0, Moving true, Direction 1.0 (`fld1 ; fstp` for both floats).
// Callees: the category-change sound helper @ 0x005BD33C (manifest "PlaySoundForMeshAnimation"), this class's
//          vtable slot 37 (+0x94, tail-jumped).
// Layout: retail NInventory::CTCInventoryBase 8 bytes below the PDB from +0x58: InventorySelectionState +0x1C,
//         Categories +0x20 (0x2C-byte CCategory, Selected +0x28), SelectedCategoryIndex +0x70, CategoryMoving +0x89,
//         CategoryMovement +0x8C, CategoryMovementDirection +0x90 (PDB +0x20/+0x24/+0x78/+0x91/+0x94/+0x98).
// Layout (supersedes any local-model note above): engine/CCategory.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CCategory CCategoryDonor
#include "engine/CCategory.h"
#undef CCategory
#pragma optimize("s", on)

namespace NInventory {

class CCategory : public CCategoryDonor {
 public:                                   // NInventory::CCategory, retail 0x2C bytes
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

class CTCInventoryBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void OnCategoryChanged();                // slot 37 (+0x94)
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x14];
    long          InventorySelectionState;           // +0x1C
    CRetailVector<CCategory> Categories;             // +0x20
    unsigned char _pad_0x2c[0x44];
    long          SelectedCategoryIndex;             // +0x70
    unsigned char _pad_0x74[0x14];
    bool          Dying;                             // +0x88
    bool          CategoryMoving;                    // +0x89
    unsigned char _pad_0x8a[0x2];
    float         CategoryMovement;                  // +0x8C
    float         CategoryMovementDirection;         // +0x90

    void PlayCategoryChangeSound();                  // 0x005BD33C
    void SelectNextCategory();
    void SelectPreviousCategory();
};

void CTCInventoryBase::SelectNextCategory()
{
    if (Categories.size() > 1 && InventorySelectionState == 0) {
        PlayCategoryChangeSound();
        CCategory& category = Categories[SelectedCategoryIndex];
        if (category.Selected)
            category.Selected = false;
        ++SelectedCategoryIndex;
        if ((unsigned long)SelectedCategoryIndex >= Categories.size())
            SelectedCategoryIndex = 0;
        CategoryMovement = 1.0f;
        CategoryMoving = true;
        CategoryMovementDirection = 1.0f;
        OnCategoryChanged();
    }
}

}