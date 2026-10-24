// std::__median<CPriorityDefPair, CContextSensitivePriorityCompare>(const T& a, const T& b, const T& c, CContextSensitivePriorityCompare comp) -- median-of-three pivot for the context-sensitive item sort
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) @ 0x00592FFB, 101 bytes, `ret 8` (`/Gr` `__fastcall`: a in ecx, b in edx; c and the
// by-value comparator on the stack; returns a reference in eax).
// Element: std::pair<long, CDefPointer<const CContextSensitiveItemDef> > (8 bytes; Ego_r.pdb sort/heap instantiations
// over `?$pair@JV?$CDefPointer@$$CBVCContextSensitiveItemDef@@@@`), compared by
// CContextSensitivePriorityCompare::operator()(const pair&, const pair&) const @ 0x005928A8 (landed; `this` unused).
// Only caller: the STLport `__introsort_loop` @ 0x00594E4D (`__median(*first, *(first + (last - first)/2),
// *(last - 1), comp)`).
// Label: the catalog "Min3_CContextSensitiveItem" is descriptive; the body is STLport 4.x `__median` verbatim
// (the Ego_r `_Med3`/`_Median` spellings are Dinkumware).
// Shape: a in edi, b in esi; every comparator call is a thiscall on the comparator's own stack slot
// (`lea ecx,[ebp+0Ch]`, so the size TU keeps an ebp frame), the c argument is pushed before the first branch and
// shared by both arms. Size TU: `#pragma optimize("s", on)`.
#pragma optimize("s", on)

class CContextSensitiveItemDef;

template <class T> class CDefPointer {
public:
    T* PDef;                                         // the reference-counted def (released by ~CDefPointer)
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > CPriorityDefPair;

class CContextSensitivePriorityCompare {
public:
    bool operator()(const CPriorityDefPair& left, const CPriorityDefPair& right) const;   // 0x005928A8
};

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template const CPriorityDefPair& __fastcall __median(const CPriorityDefPair&, const CPriorityDefPair&,
                                                     const CPriorityDefPair&, CContextSensitivePriorityCompare);
}