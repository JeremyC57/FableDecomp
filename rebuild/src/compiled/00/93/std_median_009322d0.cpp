// std::__median(const T&, const T&, const T&, bool (__fastcall*)(SSortKey_009322d0, SSortKey_009322d0)) -- the SGI median of three with a function-pointer comparator: b when a<b<c, else c when a<c, else a; symmetrically when !(a<b)
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) @ 0x009322D0, 96 bytes; `__fastcall` (a in ecx -> ebx, b in edx -> edi, c and the comparator on
//   the stack -> ebp / esi), `ret 8`. No direct callees: the comparator is called through esi (`call esi`).
// Identity: the propagated label `CPredicate_And<...CIsThingAlive...>::operator()` is wrong. The five compares in the SGI
//   order and the returned element address are std::__median (cf. the landed functor twin 0x009B2930); here the comparator
//   is a plain `__fastcall` function pointer, and the comparator takes the 4-byte elements BY VALUE, so every call loads both elements (`mov edx,[edi] ; mov ecx,[ebx]`) first.
//   The element type is not recoverable (no call/jmp callers: it is reached from an introsort instantiation).
struct CSortItem_009322d0;                        // the pointee type is not recoverable from the body
typedef CSortItem_009322d0* SSortKey_009322d0;    // a pointer element, passed to the comparator by value in ecx/edx
typedef bool (__fastcall* TCompare_009322d0)(SSortKey_009322d0 a, SSortKey_009322d0 b);

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

template const SSortKey_009322d0* __fastcall __median<SSortKey_009322d0, TCompare_009322d0>(const SSortKey_009322d0& a, const SSortKey_009322d0& b, const SSortKey_009322d0& c, TCompare_009322d0 comp);