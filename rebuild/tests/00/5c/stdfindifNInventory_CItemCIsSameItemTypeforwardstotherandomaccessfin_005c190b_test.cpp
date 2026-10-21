// behaviour test for 0x005C190B
#include <stdio.h>
static int g_findIfCalls = 0;
namespace NInventory {
class CItem {
public:
    long ItemType;
    long NumberOfItems;
    void* PPerItemData[2];
    long AverageBoughtPrice;
    bool IsSameItemTypeAsDefIndex(long defIndex) const;  // 0x005BC450
};
}
class CIsSameItemType {
public:
    long ItemType;                                       // +0x00
    bool OnlyFindNonFullSlots;                           // +0x04
    CIsSameItemType(long itemType, bool onlyFindNonFullSlots);
    bool operator()(const NInventory::CItem& item) const;   // 0x005C150B
};
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};
template <class T> inline random_access_iterator_tag __fastcall __iterator_category(const T*) { return random_access_iterator_tag(); }
template <class Arg1, class Arg2, class Result> struct binary_function {
    typedef Arg1 first_argument_type; typedef Arg2 second_argument_type; typedef Result result_type;
};
template <class R, class T, class A> class const_mem_fun1_ref_t : public binary_function<T, A, R> {
    typedef R (T::*fun_type)(A) const;
    fun_type f;
public:
    explicit const_mem_fun1_ref_t(fun_type pf) : f(pf) {}
    R operator()(const T& x, A a) const { return (x.*f)(a); }
};
template <class Op> class binder2nd {
protected:
    Op op;
    typename Op::second_argument_type value;
public:
    binder2nd(const Op& x, const typename Op::second_argument_type& y) : op(x), value(y) {}
    typename Op::result_type operator()(const typename Op::first_argument_type& x) const { return op(x, value); }
};
template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred,
                                      const random_access_iterator_tag&)
{ ++g_findIfCalls; for (; first != last; ++first) if (pred(*first)) return first; return last; }
template <class InputIter, class Predicate>
inline InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, __iterator_category(first));
}
template NInventory::CItem* __fastcall find_if(NInventory::CItem*, NInventory::CItem*,
    ::CIsSameItemType);
}

bool NInventory::CItem::IsSameItemTypeAsDefIndex(long defIndex) const { return ItemType == defIndex; }
CIsSameItemType::CIsSameItemType(long t, bool f) : ItemType(t), OnlyFindNonFullSlots(f) {}
bool CIsSameItemType::operator()(const NInventory::CItem& item) const
{ return item.ItemType == ItemType && (!OnlyFindNonFullSlots || item.NumberOfItems < 10); }

int main()
{
    NInventory::CItem items[4]; int ok = 1;
    long types[4] = {3, 7, 7, 7}; long counts[4] = {1, 10, 10, 2};
    for (int i = 0; i < 4; ++i) { items[i].ItemType = types[i]; items[i].NumberOfItems = counts[i]; }
    NInventory::CItem* r = std::find_if((NInventory::CItem*)items, (NInventory::CItem*)items + 4, ::CIsSameItemType(7, false));
    ok &= r == items + 1;
    r = std::find_if((NInventory::CItem*)items, (NInventory::CItem*)items + 4, ::CIsSameItemType(7, true));
    ok &= r == items + 3;
    r = std::find_if((NInventory::CItem*)items, (NInventory::CItem*)items + 4, ::CIsSameItemType(9, false));
    ok &= r == items + 4;
    ok &= g_findIfCalls >= 2 && sizeof(NInventory::CItem) == 0x14;
    printf(ok ? "005c190b_TEST PASS\n" : "005c190b_TEST FAIL\n");
    return ok ? 0 : 1;
}