#include <cstdio>
// Behaviour test for 0x008ED010 CTCContainer::ContainsItem(long) const: links against the source object (which carries
// the __find_if instantiation); true exactly when some item's definition index equals the key.
class CCharString { public: void* PStringData; };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() {} };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { };
}
class CTCContainer {
public:
    unsigned char _base[0xc];
    std::vector<std::pair<long, CCharString> > ContainerItems;
    bool ContainsItem(long defIndex) const;
};
int main()
{
    std::pair<long, CCharString> items[7];
    for (int i = 0; i < 7; ++i) { items[i].first = 100 + 3 * i; items[i].second.PStringData = 0; }
    CTCContainer c;
    c.ContainerItems._M_start = items; c.ContainerItems._M_finish = items; c.ContainerItems._M_end_of_storage._M_data = items + 7;
    if (c.ContainsItem(100)) { std::printf("008ed010_TEST FAIL empty\n"); return 1; }
    c.ContainerItems._M_finish = items + 7;     // 7 items: one unrolled round of four plus the switch tail of three
    for (int i = 0; i < 7; ++i)
        if (!c.ContainsItem(100 + 3 * i)) { std::printf("008ed010_TEST FAIL item %d\n", i); return 1; }
    if (c.ContainsItem(101) || c.ContainsItem(99) || c.ContainsItem(119)) { std::printf("008ed010_TEST FAIL absent\n"); return 1; }
    c.ContainerItems._M_finish = items + 2;
    if (c.ContainsItem(106) || !c.ContainsItem(103)) { std::printf("008ed010_TEST FAIL short\n"); return 1; }
    std::printf("008ed010_TEST PASS\n");
    return 0;
}
