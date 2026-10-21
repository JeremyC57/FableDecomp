// std::find_if<const NInventory::CItem*, std::binder2nd<const_mem_fun1_ref_t<bool, NInventory::CItem, long> >> -- forwards to the random-access __find_if with an iterator-category tag
// Leaf: find_if
// Retail: Fable.exe @ 0x005C13CD, 22 bytes, `ret 8` (fastcall: first in ecx, last in edx, the 8-byte predicate on the
// stack). Catalog label std::vector<pair<long,CCountedPointer<CContainmentVolume>>>::_Ucopy is wrong. Ego_r.pdb names this
// instantiation (??$find_if@...@std@@YI..., YI = __fastcall), and its callee ??$_Find_if@... (0x005C1153); STLport passes the
// random_access_iterator_tag by const reference (`lea eax,[ebp+0Fh] ; push eax`). Only caller: NInventory::CCategory::GetNumberOfItemsOfTypeInCategory (const) 0x005BDEAB, predicate std::bind2nd(std::mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex), defIndex) (pushes 0x005BC450).
// NInventory::CItem is 0x14 bytes (Ego_r.pdb, matching the callee's `push 14h ; pop ebx` stride).
// Shape: size TU (ebp frame and `push dword ptr [ebp+..]` are the /O1 encodings).
#pragma optimize("s", on)
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
                                      const random_access_iterator_tag&);          // out of line
template <class InputIter, class Predicate>
inline InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    return __find_if(first, last, pred, __iterator_category(first));
}
template const NInventory::CItem* __fastcall find_if(const NInventory::CItem*, const NInventory::CItem*,
    binder2nd<const_mem_fun1_ref_t<bool, NInventory::CItem, long> >);
}