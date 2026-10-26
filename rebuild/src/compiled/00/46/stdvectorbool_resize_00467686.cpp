// std::vector<bool>::resize(unsigned int new_size, const bool& x) -- resize a one-byte-per-element bool vector: erase the surplus tail, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00467686, 53 bytes, thiscall `ret 8`, in a size-optimised TU (unaligned entry, the shared
//   `jmp` epilogue, ebp-framed neighbours).
// Identity: vector<bool> (sweep3; the landed rows said vector<unsigned char>): this size-optimised TU group (resize 0x00467686, erase 0x004676BB,
//   _M_fill_insert 0x004676EF, _M_insert_overflow 0x00467831, push_back 0x0053CB4F) serves vector<bool> members only: its callers
//   hold Ego_r vector<bool> members (CTCSpecialAbilities::AbilityAvailability 0x0072C6E0, CVertexGroupBlockOptimiser::RemainingBlocks
//   0x00ADA240, CTCWeaponTrail::ActiveTrail, CTCTavernTable::SeatsCurrentlyUsed, NUISystem::CList::ScrollingChildrenVisibility via
//   push_back) and GFSerialiseVectorBinaryIn<bool> 0x004675F6 calls resize (already bound as ?resize@?$vector@_N@std@@...). The
//   fill is STLport's generic byte loop (x re-read each pass), while the char-type overloads fill/fill_n use memset (the real
//   vector<unsigned char> overflow 0x00485B6C calls fill 0x0047CE40); retail STLport stores one byte per bool.
// Callers: 13 sites, among them 0x00467661 (the byte-array reader just before this body: `resize(n, 0)` over a zeroed byte
//   temporary), 0x007B9E60, 0x00AB07FB, 0x00CDD0B2.
// Callees: vector<bool>::erase(first, last) 0x004676BB (the memmove copy-down, right after this body),
//   vector<bool>::_M_fill_insert(pos, n, x) 0x004676EF.
// Shape: STLport 4.6 `resize(n, x) { if (n < size()) erase(begin() + n, end()); else insert(end(), n - size(), x); }` with
//   insert forwarding to the out-of-line _M_fill_insert; under /Os erase stays an out-of-line call and the two arms share
//   one epilogue. `#pragma optimize("s", on)` stands for the TU's size optimisation.
#pragma optimize("s", on)

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x004676BB
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);     // 0x004676EF
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};

template void vector<bool>::resize(unsigned int, const bool&);
}