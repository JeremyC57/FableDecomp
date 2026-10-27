// std::vector<CMusicEntry>::resize(unsigned int newSize, const CMusicEntry& x) -- STLport resize: erase the tail when shrinking, otherwise insert newSize - size() copies of x at the end
// Leaf: ?resize@?$vector@VCMusicEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046B681, 64 bytes, thiscall, `ret 8`.
// Identity (proven): reloc_bindings ?resize@?$vector@VCMusicEntry@@V?$allocator@VCMusicEntry@@@std@@@std@@QAEXIABVCMusicEntry@@@Z; its caller resize(n) 0x0046B65A passes a {0, TablePos -1, 0.0f, false} element (Ego_r.pdb CMusicEntry) and is reached from CSoundDef::Transfer 0x0046A9CE (MusicEntries).
// Callees: vector::erase(iterator, iterator) 0x0046B6C1, vector::_M_fill_insert(iterator, size_type, const T&) 0x0046B6F6.
// Shape (family K101, representative 0x0046B681): STLport 4.6 `resize(n, x)` with the inline insert(pos, n, x) forwarding to
//   _M_fill_insert; size TU (`optimize("s", on)`), so the tiny accessors are __forceinline; the signed `sar 4` size.
#pragma optimize("s", on)
class CDefString {                                       // Ego_r.pdb 4 bytes
public:
    long TablePos;
};
class CMusicEntry {                                      // Ego_r.pdb 16 bytes
public:
    unsigned long ID;
    CDefString Filename;
    float Gain;
    bool CacheToHD;
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    __forceinline iterator begin() { return _M_start; }
    __forceinline iterator end() { return _M_finish; }
    __forceinline size_type size() const { return size_type(_M_finish - _M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x0046B6C1
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x0046B6F6
    __forceinline void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
template void vector<CMusicEntry >::resize(unsigned int, const CMusicEntry&);
}