// std::__find_if(const NInventory::CItem* first, const NInventory::CItem* last, std::binder2nd<std::const_mem_fun1_ref_t<bool, NInventory::CItem, long> > pred, const random_access_iterator_tag&) -- STLport random-access find_if over a category's items: the first item for which item.*f(value) is true, else last
// Leaf: ??$__find_if@
// Retail: Fable.exe @ 0x005C1153, 167 bytes, `ret 0Ch`: __fastcall (/Gr template TU) -- first in ecx, last in edx (spilled to
// the frame word [ebp-4]), the 8-byte predicate {member pointer, bound long} BY VALUE and the unused tag on the stack. Only
// caller: the const find_if forwarder @ 0x005C13CD (landed; Ego_r.pdb `find_if<const CItem*, binder2nd<const_mem_fun1_ref_t<
// bool,CItem,long>>>`), used by NInventory::CCategory::GetNumberOfItemsOfTypeInCategory (const) 0x005BDEAB with
// bind2nd(mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex @ 0x005BC450), defIndex).
// Body: STLport 4.x _algo.c __find_if (four inlined predicate calls per trip, the 3/2/1 remainder switch). Each call is
// binder2nd::operator() -> const_mem_fun1_ref_t::operator() -> `(x.*f)(value)`: `push [ebp+0Ch] ; mov ecx,esi ; call [ebp+8]`
// (the bound value and the member pointer are re-read from the by-value predicate's own argument slots).
// Shape: size TU (`push 14h ; pop ebx` stride, `idiv`); the ebp frame is the /O1 choice for the slot-relative calls.
#pragma optimize("s", on)
#include "engine/CItem.h"
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// STLport 4.x _algo.c: the random-access __find_if, four predicate calls per trip, then the 3/2/1 remainder switch.
template <class RandomAccessIter, class Predicate>
RandomAccessIter __fastcall __find_if(RandomAccessIter first, RandomAccessIter last, Predicate pred, const random_access_iterator_tag&)
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
}
namespace std {
template <class Arg1, class Arg2, class Result> struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
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
}
template const CItem* __fastcall std::__find_if(const CItem*, const CItem*, std::binder2nd<std::const_mem_fun1_ref_t<bool, CItem, long> >, const std::random_access_iterator_tag&);