// behaviour test for 005c0965: STLport random-access __find_if over NInventory::CItem (0x14 bytes) with a const_mem_fun_ref_t predicate (IsEmpty-style member)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
}
template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall FindIfModel(RandomAccessIter first, RandomAccessIter last, Predicate pred, const std::random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
struct CItem {
    int ItemType;
    int NumberOfItems;
    int PPerItemData[2];
    int AverageBoughtPrice;
    bool IsEmpty() const { return NumberOfItems == 0; }
    bool IsSameItemTypeAsDefIndex(long defIndex) const { if (NumberOfItems == 0) return false; return defIndex == ItemType; }
    bool IsSlotFull() const { return NumberOfItems >= 3; }
};
template <class R, class T> class const_mem_fun_ref_t {
    typedef R (T::*fun_type)() const;
    fun_type f;
public:
    explicit const_mem_fun_ref_t(fun_type pf) : f(pf) {}
    R operator()(const T& x) const { return (x.*f)(); }
};
template <class R, class T, class A> class const_mem_fun1_ref_t {
    typedef R (T::*fun_type)(A) const;
    fun_type f;
public:
    explicit const_mem_fun1_ref_t(fun_type pf) : f(pf) {}
    R operator()(const T& x, A a) const { return (x.*f)(a); }
};
template <class Op, class A> class binder2nd {
    Op op;
    A value;
public:
    binder2nd(const Op& x, const A& y) : op(x), value(y) {}
    bool operator()(const CItem& x) const { return op(x, value); }
};
class CIsSameItemType { public:
    long ItemType;
    bool OnlyFindNonFullSlots;
    bool operator()(const CItem& item) const { return item.IsSameItemTypeAsDefIndex(ItemType) && (!OnlyFindNonFullSlots || !item.IsSlotFull()); }
};
int main()
{
    if (sizeof(CItem) != 0x14) { std::printf("005c0965_TEST FAIL\n"); return 1; }
    CItem items[11];
    for (int i = 0; i < 11; ++i) { items[i].ItemType = 10 + i; items[i].NumberOfItems = 1 + (i % 4); items[i].PPerItemData[0] = items[i].PPerItemData[1] = 0; items[i].AverageBoughtPrice = 0; }
    std::random_access_iterator_tag tag;
    int bad = 0;
    /* mem_fun_ref(&CItem::IsEmpty): the first empty slot, every remainder length */
    const_mem_fun_ref_t<bool, CItem> pred(&CItem::IsEmpty);
    for (int n = 0; n <= 11; ++n) {
        for (int k = 0; k <= 11; ++k) {
            for (int i = 0; i < 11; ++i) items[i].NumberOfItems = 1;
            if (k < 11) items[k].NumberOfItems = 0;
            const CItem* r = FindIfModel((const CItem*)items, (const CItem*)items + n, pred, tag);
            const CItem* want = (k < n) ? items + k : items + n;
            if (r != want) ++bad;
        }
    }
    if (bad) { std::printf("005c0965_TEST FAIL\n"); return 1; }
    std::printf("005c0965_TEST PASS\n");
    return 0;
}