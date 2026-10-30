// NInventory::CCategory::GetHighlightedItemMaxInCategory() -- maximum number of items the highlighted slot can hold (0 when the slot is empty)
// Grade: DIFFER(30v31) -- retail `xor eax,eax ; cmp [ecx+esi+4],eax` with the slot start in esi and the offset in ecx; VC7.1 folds base+offset into one lea / addressing mode. /O2 is right (lea/shl). 5 grades.
// Leaf: ?GetHighlightedItemMaxInCategory@CCategory@NInventory@@QBEJXZ
#define CItem CItemPdb
#include "engine/CItem.h"
#undef CItem

namespace std
{
    template <class _Tp> class allocator { public: };
    template <class _Value, class _Tp, class _MaybeReboundAlloc>
    class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
    template <class _Tp, class _Alloc>
    class _Vector_base
    {
    public:
        _Tp* _M_start;
        _Tp* _M_finish;
        _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    };
    template <class _Tp, class _Alloc = allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
    public:
        typedef _Tp value_type;
        typedef value_type* iterator;
        typedef const value_type* const_iterator;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef unsigned int size_type;
        iterator begin() { return this->_M_start; }
        const_iterator begin() const { return this->_M_start; }
        reference operator[](size_type __n) { return *(begin() + __n); }
        const_reference operator[](size_type __n) const { return *(begin() + __n); }
    };
}

namespace NInventory
{
    class CItem : public CItemPdb
    {
    public:
        long GetMaximumNumberOfItems() const;                // 0x005BD931
    };

    // Layout: retail CCategory with STLport's 12-byte vector (engine/CCategory.h carries Ego_r's 16-byte Dinkumware vector).
    class CCategory
    {
    public:
        unsigned char PAssociatedThing[0x8];                 // +0x00 CIntelligentPointer<CThing>
        std::vector<CItem> ItemsInCategory;                  // +0x08
        void* PDef_Object;                                   // +0x14 CDefPointer<const CInventoryCategoryDef>
        void* PInventory;                                    // +0x18 CTCInventoryBase*
        long GlobalDefIndex;                                 // +0x1C
        long SelectedItemIndex;                              // +0x20
        long HighlightedItemIndex;                           // +0x24
        long GetHighlightedItemMaxInCategory() const;
    };

    long CCategory::GetHighlightedItemMaxInCategory() const
    {
        if (ItemsInCategory[HighlightedItemIndex].NumberOfItems == 0)
            return 0;
        return ItemsInCategory[HighlightedItemIndex].GetMaximumNumberOfItems();
    }
}
