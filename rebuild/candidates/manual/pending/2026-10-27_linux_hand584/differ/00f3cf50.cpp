// CTriangleStripifier2::CalcSectionPolyMask(const CArray<unsigned short>& triangles, const CArray<CArray<long> >& vertexPolys, const CArray<unsigned short>& section, CArray<bool>& polyMask) -- mark the polygons a strip section covers
// Grade: DIFFER(679v683) -- retail passes the three vector ctors' empty allocator_type() temporary in the dead arg slot [esp+0x4c] (ours uses a fresh frame byte, shifting the `false` value byte [esp+0x17] -> [esp+0x16]); retail strength-reduces the first section loop's i*2 into edx (movzx eax,[edx+eax]) where ours indexes [ecx+eax*2]
// Leaf: ?CalcSectionPolyMask@CTriangleStripifier2@@AAEJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@0AAV?$CArray@_N@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3CF50, 683 bytes, thiscall, `ret 0x10`.
// Identity (proven): Ego_r.pdb names ?CalcSectionPolyMask@CTriangleStripifier2@@AAEJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@0AAV?$CArray@_N@@@Z
//   (private, AAE); the landed caller 0x00F3D830 binds a QAE spelling (NAME_FIXES.tsv). Catalog label is wrong.
// Callees: vector<long>(size_type, const long&, const allocator&) 0x00512630, vector<bool>(size_type, const bool&, const allocator&)
//   0x00451583, vector<bool>::_M_fill_insert 0x004676EF, memmove 0x00BFEAE6 (inline __copy_trivial of erase), free 0x00BFEA14.
// Data: -0x40000000 sentinel, 15 (the max order spread of a strip triangle).
// Tried (4 grades): local allocator byte (679), no allocator ctor (695), real STLport allocator copy ctor/dtor + int index (679),
//   no `vert` local (698). Next: an allocator-temp form that reuses the dead arg slot, or a byte-offset loop; possibly a lane gap.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14

namespace std {
struct __true_type { };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef __true_type _TrivialAss;
    vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type());
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    const _Tp& operator[](size_type __n) const { return *(this->_M_start + __n); }
    void _M_fill_insert(pointer __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        this->_M_finish = __i;
        return __first;
    }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void clear() { erase(begin(), end()); }
};
}

template <class T> class CArray : public std::vector<T> { };

class CTriangleStripifier2 {
public:
    virtual void v00();
private:
    long CalcSectionPolyMask(const CArray<unsigned short>& triangles, const CArray<CArray<long> >& vertexPolys,
                             const CArray<unsigned short>& section, CArray<bool>& polyMask);
};

long CTriangleStripifier2::CalcSectionPolyMask(const CArray<unsigned short>& triangles, const CArray<CArray<long> >& vertexPolys,
                                               const CArray<unsigned short>& section, CArray<bool>& polyMask)
{
    std::vector<bool> polyTested(triangles.size() / 3, false);
    std::vector<bool> vertInSection(vertexPolys.size(), false);
    std::vector<long> vertOrder(vertexPolys.size(), -0x40000000);
    unsigned int i;
    for (i = 0; i < section.size(); ++i) {
        unsigned short vert = section[i];
        vertInSection[vert] = true;
        vertOrder[vert] = i + 1;
    }
    polyMask.clear();
    polyMask.resize(triangles.size() / 3, false);
    long count = 0;
    for (i = 0; i < section.size(); ++i) {
        unsigned short vert = section[i];
        for (unsigned int j = 0; j < vertexPolys[vert].size(); ++j) {
            long poly = vertexPolys[vert][j];
            if (!polyTested[poly]) {
                const unsigned short* tri = &triangles[poly * 3];
                if (vertInSection[tri[0]] && vertInSection[tri[1]] && vertInSection[tri[2]]) {
                    long a = vertOrder[tri[0]];
                    long b = vertOrder[tri[1]];
                    long c = vertOrder[tri[2]];
                    long lo = ((a < b ? a : b) < c) ? (a < b ? a : b) : c;
                    long hi = ((a > b ? a : b) > c) ? (a > b ? a : b) : c;
                    if (hi - lo < 15) {
                        polyMask[poly] = true;
                        ++count;
                    }
                }
                polyTested[poly] = true;
            }
        }
    }
    return count;
}
