// std::less::operator()(const CDisplayModeDesc& x, const CDisplayModeDesc& y) -- set<CDisplayModeDesc> ordering: compare width, height, bits per pixel, then refresh rate (signed, lexicographic)
// Leaf: ??R?$less@VCDisplayModeDesc@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C1860, 50 bytes, thiscall `ret 8` with `this` (the empty functor) unused.
// Identity: the out-of-line comparator of the STLport `set<CDisplayModeDesc>` built by
// CDisplayManager::EnumerateDisplayModes (0x009C02D0: a local tree header malloc'd, each adapter mode inserted through
// 0x009C2800 -> _M_insert 0x009C2350, which calls 0x009C1860 on the tree's comparator `lea ecx,[edi+8]`).
// Ego_r.pdb: CDisplayModeDesc {C2DExtentsI Dimensions; long BitsPerPixel; long RefreshRate} (16 bytes) with
// `bool CDisplayModeDesc::operator<(const CDisplayModeDesc&) const` (??MCDisplayModeDesc@@QBE_NABV0@@Z), and the
// Dinkumware set<CDisplayModeDesc, less<CDisplayModeDesc>> of the same function. Label correction: catalogued as
// `MemoryAllocator_Compare_24byte`.
// Shape: STLport `less<T>::operator()` returning `x < y` with the member operator< inlined: each field pair is
// compared and the first unequal one decides; the four `return a < b` tails share one `setl al` (VC merges the
// identical flag tests into the final compare's block).

class C2DExtentsI {
public:
    long X;                                             // +0x00
    long Y;                                             // +0x04
};

class CDisplayModeDesc {
public:
    C2DExtentsI Dimensions;                             // +0x00
    long        BitsPerPixel;                           // +0x08
    long        RefreshRate;                            // +0x0C

    bool operator<(const CDisplayModeDesc& other) const
    {
        if (Dimensions.X != other.Dimensions.X)
            return Dimensions.X < other.Dimensions.X;
        if (Dimensions.Y != other.Dimensions.Y)
            return Dimensions.Y < other.Dimensions.Y;
        if (BitsPerPixel != other.BitsPerPixel)
            return BitsPerPixel < other.BitsPerPixel;
        return RefreshRate < other.RefreshRate;
    }
};

namespace std {

template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};

template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const;
};

template <class _Tp> bool less<_Tp>::operator()(const _Tp& __x, const _Tp& __y) const
{
    return __x < __y;
}

template struct less<CDisplayModeDesc>;

}