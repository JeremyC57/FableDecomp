// std::__median(const T&, const T&, const T&, bool (__fastcall*)(const T&, const T&)) -- the SGI median of three with a function-pointer comparator: b when a<b<c, else c when a<c, else a; symmetrically when !(a<b)
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A45A10, 89 bytes (+7 int3 padding); `__fastcall` (a in ecx -> ebx, b in edx -> edi, c and the comparator on
//   the stack -> ebp / esi), `ret 8`. No direct callees: the comparator is called through esi (`call esi`).
// Identity: the propagated label `CPredicate_And<...CIsThingAlive...>::operator()` is wrong. The five compares in the SGI
//   order and the returned element address are std::__median (cf. the landed functor twin 0x009B2930); here the comparator
//   is a plain `__fastcall` function pointer, and the comparator takes the elements BY REFERENCE, so the element addresses go straight into ecx/edx.
//   The element type is not recoverable (no call/jmp callers: it is reached from an introsort instantiation).
struct SSortKey_00a45a10 { void* m_pItem; };   // element type not recoverable from the body (only its address is used)
typedef bool (__fastcall* TCompare_00a45a10)(const SSortKey_00a45a10& a, const SSortKey_00a45a10& b);

template <class T, class Compare>
const T* __fastcall __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b)) {
        if (comp(b, c))
            return &b;
        else if (comp(a, c))
            return &c;
        else
            return &a;
    }
    else if (comp(a, c))
        return &a;
    else if (comp(b, c))
        return &c;
    else
        return &b;
}

template const SSortKey_00a45a10* __fastcall __median<SSortKey_00a45a10, TCompare_00a45a10>(const SSortKey_00a45a10& a, const SSortKey_00a45a10& b, const SSortKey_00a45a10& c, TCompare_00a45a10 comp);