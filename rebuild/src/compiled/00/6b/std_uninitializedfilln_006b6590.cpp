// std::uninitialized_fill_n(CThemeChannel* first, unsigned int n, const CThemeChannel& x) -- STLport three-argument front end: copy-construct n theme channels into raw storage, return first + n
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6590, 45 bytes (+ int3 padding), `ret 4`.
// Callers: one site, 0x006B6BF7 (the vector<CThemeChannel> fill/resize helper).
// Callees: CThemeChannel::CThemeChannel(const CThemeChannel&) @ 0x006B6010 (out of line; catalog
//   "CFeatDef_CopySpecializedData" is propagated -- the landed 0x006B6500 uninitialized_copy calls it as the 0x1F0-byte
//   copy constructor).
// Shape: `ret 4` = the top-level `uninitialized_fill_n(first, n, x)` with `__uninitialized_fill_n(.., __false_type())`
//   inlined (`__forceinline`, GOTCHAS 00440180/007688b0): `__fastcall` (first ecx -> esi as `cur`, n edx -> edi counted
//   down, &x on the stack -> ebx loaded once), placement-new null test, the advanced pointer returned.
// Label correction: the catalog `_Fill_n<unsigned long*,unsigned int,unsigned long>` is propagated; the stride (0x1F0)
//   and the copy constructor name the element (NEnvironmentInternal CThemeChannel, as 0x006B6560 fill).
inline void* operator new(unsigned int, void* p) { return p; }

struct CThemeChannel {                                  // 496 bytes (0x1F0)
    unsigned char _body[0x1f0];
    CThemeChannel(const CThemeChannel& other);          // 0x006B6010
};

struct __false_type { __false_type() { } };

namespace std {
template <class ForwardIter, class Size, class T>
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template CThemeChannel* __fastcall uninitialized_fill_n<CThemeChannel*, unsigned int, CThemeChannel>(CThemeChannel*, unsigned int, const CThemeChannel&);
}