// std::__uninitialized_fill_n(ExperienceSpendInfo* first, unsigned int n, const ExperienceSpendInfo& x, const __false_type&) -- STLport placement fill of n 12-byte spend records
// Leaf: ??$__uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) @ 0x00578C8D, 32 bytes, __fastcall (first ecx, n edx) `ret 8`. Called by the
//   12-byte vector's _M_insert_overflow (0x00579A4B) and _M_fill_insert (0x00579B3C, both this chunk) and
//   0x005790FC; the element is CTCInventoryExperience::ExperienceSpendInfo (see 0x00578C50). A class, so STLport's
//   type traits pick the __false_type (construct-each) overload. The catalog "_Fill_n<unsigned_long*,...>" is
//   propagated.
// Shape: STLport 4.x verbatim: `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return __cur;` --
//   unsigned count (`test edx,edx ; jbe`), _Construct as placement new with its null test, the implicit copy as
//   three `movsd`.
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* __p) { return __p; }

class CTCInventoryExperience {
public:
    class ExperienceSpendInfo {
    public:
        long currentSpend;                          // +0x0
        long initialSpend;                          // +0x4
        long levelUpSpend;                          // +0x8
    };
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CTCInventoryExperience::ExperienceSpendInfo* __fastcall __uninitialized_fill_n(
    CTCInventoryExperience::ExperienceSpendInfo*, unsigned int, const CTCInventoryExperience::ExperienceSpendInfo&,
    const __false_type&);

}