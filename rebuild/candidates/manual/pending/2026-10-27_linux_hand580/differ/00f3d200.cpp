// CTriangleStripifier2::CheckForReshadedVerts(CArray<unsigned short>& indices) -- walk an index list through a 16-entry FIFO vertex-cache model: grow the per-vertex reshaded flags and last-use stamps on demand, and on each cache miss bump the miss counter, stamp the vertex and flag it as reshaded (release build: the result is discarded)
// Grade: DIFFER(285v284) -- register allocation: retail homes the two local arrays' first pointers in ebx/ebp (and the size in ecx) and spills the miss counter and the loop index to [esp+14h]/[esp+18h] (frame 28h); VC7.1 here keeps the counter in ebp and the index in ebx and reloads the array pointers into edx/ecx (frame 20h). Tried: vert as unsigned short/int/long, loop as for/while, counter/array declaration order, ++ forms, plain end-of-storage pointer, explicit dtors, flags /Oa /Ow /G6 /G7 /GX, same-TU noinline _M_fill_insert bodies. Next: try the QFE-4035 toolset, or a CArray-level resize/SetSize helper shape.
// Leaf: ?CheckForReshadedVerts@CTriangleStripifier2@@AAEXAAV?$CArray@G@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3D200, 284 bytes, thiscall `ret 4` (`this` unused).
// Identity: Ego_r.pdb CTriangleStripifier2 name set, in donor order ... CheckForReshadedVerts 0x00BD8D20 (0x180 bytes),
//   GetMeshIslandSubsection 0x00BD8EA0 (0x630), CalcSectionScore 0x00BD94D0; retail ... 0x00F3D200 (0x11C), 0x00F3D320 (0x510),
//   the landed CalcSectionScore 0x00F3D830. Signature `AAEXAAV?$CArray@G@@@Z` (void, one CArray<unsigned short>&) matches the
//   single word-index-array argument and `ret 4`; no xrefs (a debug-only check whose output release discards).
// Callees: vector<bool>::_M_fill_insert 0x004676EF and vector<long>::_M_fill_insert 0x00440F00 (both via the inline STLport
//   resize -> insert), free 0x00BFEA14 (the two local arrays' inline destructors).
// Layout: CArray<T> derives from std::vector<T> (Ego_r.pdb: CArray<unsigned short> base vector, 16 bytes Dinkumware; retail
//   STLport 4.6, 12 bytes). Retail STLport stores vector<bool> one byte per element (see 0x004676EF).
extern "C" void __cdecl free(void* block);                     // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start) free(_M_start); }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() const { return this->_M_start; }
    iterator end() const { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Tp& operator[](unsigned int __n) const { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        unsigned int __n = (unsigned int)((char*)this->_M_finish - (char*)__last);
        if (__n)
            memmove(__first, __last, __n);
        this->_M_finish = (_Tp*)((char*)__first + __n);
        return __first;
    }
    void _M_fill_insert(iterator __pos, unsigned int __n, const _Tp& __x);
    void insert(iterator __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
};

class CTriangleStripifier2 {
public:
    virtual ~CTriangleStripifier2();
private:
    void CheckForReshadedVerts(CArray<unsigned short>& indices);
};

void CTriangleStripifier2::CheckForReshadedVerts(CArray<unsigned short>& indices)
{
    CArray<bool> reshaded;
    CArray<long> lastUse;
    long cacheMisses = 0;
    for (unsigned int i = 0; i < indices.size(); ++i) {
        unsigned int vert = indices[i];
        if (vert >= reshaded.size()) {
            reshaded.resize(vert + 1, false);
            lastUse.resize(vert + 1, (long)0xC0000000);
        }
        if (cacheMisses - lastUse[vert] >= 16) {
            ++cacheMisses;
            lastUse[vert] = cacheMisses;
            reshaded[vert] = true;
        }
    }
}
