// NInventory::CCategory::GetNumberOfItemsOfTypeInCategory(long def_index) const -- the NumberOfItems of the first item in the category whose type matches the definition index, or 0
// Leaf: ?GetNumberOfItemsOfTypeInCategory@CCategory@NInventory@@QBEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDE96, 43 bytes, thiscall `ret 4`; size-optimised TU (unaligned entry).
// Identity (proven): the only caller of the landed find_if<const CItem*, binder2nd<const_mem_fun1_ref_t<bool, CItem, long> > >
//   0x005C13CD, its predicate bind2nd(mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex), def_index) (pushes 0x005BC450);
//   Ego_r.pdb CCategory::ItemsInCategory vector<CItem> and CItem::NumberOfItems (+4) match the reads.
// Callees: std::find_if 0x005C13CD (__fastcall: first ecx, last edx, the 8-byte binder on the stack).
// Layout: retail CCategory: PAssociatedThing CIntelligentPointer<CThing> +0 (8 bytes), ItemsInCategory +8 (start +8, finish +0xC).
#pragma optimize("s", on)
namespace NInventory {
class CItem {
public:
    long ItemType;                                       // +0x00
    long NumberOfItems;                                  // +0x04
    void* PPerItemData[2];                               // +0x08 CCountedPointer<NInventory::CPerItemData>
    long AverageBoughtPrice;                             // +0x10
    bool IsSameItemTypeAsDefIndex(long defIndex) const;  // 0x005BC450
};
}
namespace std {
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
template <class R, class T, class A> inline const_mem_fun1_ref_t<R, T, A> mem_fun_ref(R (T::*f)(A) const)
{
    return const_mem_fun1_ref_t<R, T, A>(f);
}
template <class Op> class binder2nd {
protected:
    Op op;
    typename Op::second_argument_type value;
public:
    binder2nd(const Op& x, const typename Op::second_argument_type& y) : op(x), value(y) {}
    typename Op::result_type operator()(const typename Op::first_argument_type& x) const { return op(x, value); }
};
template <class Op, class T> inline binder2nd<Op> bind2nd(const Op& op, const T& x)
{
    return binder2nd<Op>(op, typename Op::second_argument_type(x));
}
template <class InputIter, class Predicate>
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred);   // 0x005C13CD
template <class _Tp> class allocator { public: allocator() {} };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef const _Tp* const_iterator;
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
};
}
class CThing;
template <class T> class CIntelligentPointer { public:
    T* Object;
    void* Info;
};
namespace NInventory {
class CCategory {
public:
    CIntelligentPointer<CThing> PAssociatedThing;        // +0x00
    std::vector<CItem> ItemsInCategory;                  // +0x08
    long GetNumberOfItemsOfTypeInCategory(long def_index) const;
};
}

long NInventory::CCategory::GetNumberOfItemsOfTypeInCategory(long def_index) const
{
    std::vector<CItem>::const_iterator it = std::find_if(ItemsInCategory.begin(), ItemsInCategory.end(),
        std::bind2nd(std::mem_fun_ref(&CItem::IsSameItemTypeAsDefIndex), def_index));
    if (it != ItemsInCategory.end())
        return it->NumberOfItems;
    return 0;
}