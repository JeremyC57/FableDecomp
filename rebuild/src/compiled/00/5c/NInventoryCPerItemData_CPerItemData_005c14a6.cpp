// NInventory::CPerItemData::CPerItemData() -- default per-item data: empty augmentation vector, zero counters, flag set, ref count -1
// Retail: Fable.exe @ 0x005C14A6, 30 bytes. Catalog label "CNavQuadTree::CNavigationLayer::CNavigationLayer" is wrong (ICF
// propagation): every caller (CItem::AddItem 0x005BECC7, CItem::UpdateItemStats 0x005BEE29/0x005BEEF2, ...) does
// `operator new(0x24)` then this ctor and hands the result to the item's CCountedPointer<CPerItemData> (+0x08) ShareData.
// Layout: 0x24 bytes = Ego_r.pdb CPerItemData (40) minus 4 for the 12-byte STLport vector. Field names follow Ego_r.pdb;
// the ctor stores +0x0c/+0x10/+0x14 = 0, +0x18 (bool) = true, +0x1c = -1 and leaves +0x20 alone.
// Opt: size TU (`or dword ptr [eax+1Ch],0FFFFFFFFh`). Shape: the STLport _Vector_base/_STLP_alloc_proxy ctor chain keeps
// the vector zeros ahead of the -1 store (a flat 3-pointer vector lets the `or` hoist to the top).
#pragma optimize("s", on)
namespace NInventory {
template <class T> class allocator { public: allocator() {} };
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) {}
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(const A& a = A()) : _Vector_base<T, A>(a) {}
};
class CPerItemData {
public:
    vector<long> Augmentations;                          // +0x00
    long         NumberOfKills;                          // +0x0c
    long         TimeWielded;                            // +0x10
    long         BestTrophyWitnessesAheadToDate;         // +0x14
    bool         QuestPersistent;                        // +0x18
    long         QuestPersistentRefCount;                // +0x1c
    float        BestFishWeight;                         // +0x20
    CPerItemData();
};
CPerItemData::CPerItemData()
    : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1)
{
}
}