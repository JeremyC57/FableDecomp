// CTriangleStripifier2::BuildVertexAndPolyMaps(const CArray<unsigned short>& triangles, CArray<CArray<long> >& vertexPolys, CArray<CArray<unsigned short> >& vertexNeighbours) -- build the per-vertex polygon lists (vertexPolys[v] = every triangle using v) and the per-vertex unique neighbour lists (vertexNeighbours[v] = the other corners of those triangles, first-seen order) from an indexed triangle list
// Grade: DIFFER(928v920) -- section 1 (vertexPolys) byte-exact; section 2 register allocation: retail keeps v*12 in esi (spilled [esp+0x10]), vertexPolys in ebp and the neighbour index as a 32-bit value in edx (spilled [esp+0x18]); VC7.1 here puts v*12 in ebx and narrows the ushort with an extra movzx / `mov bp,word` (8 bytes); a 32-bit neighbour variable mis-allocates both sections
// Leaf: ?BuildVertexAndPolyMaps@CTriangleStripifier2@@AAEXABV?$CArray@G@@AAV?$CArray@V?$CArray@J@@@@AAV?$CArray@V?$CArray@G@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3E6B0, 920 bytes (+ int3 padding), thiscall, `ret 0xc`.
// Identity: Ego_r.pdb ?BuildVertexAndPolyMaps@CTriangleStripifier2@@AAEXABV?$CArray@G@@AAV?$CArray@V?$CArray@J@@@@AAV?$CArray@V?$CArray@G@@@@@Z
//   (private); catalog label "CTriangleStripifier2_ProcessTriangles" is wrong (see NOTES.md).
// Model: STLport 4.6 vector (CArray<T> : public std::vector<T>); vector<long>::_M_insert_overflow is the out-of-line
//   0x00440990 (explicit specialization under auto_inline(off)); erase / _M_fill_insert are same-TU noinline stand-ins.
// Tried (4 grades + probes): ushort / long / unsigned long / int / unsigned int for vert, n and k; a Contains helper;
//   goto form; found flag vs early-continue. Residue is register allocation in section 2 only.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
namespace std {
struct __true_type { __true_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
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
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const _Tp* begin() const { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    const _Tp& operator[](size_type __n) const { return *(begin() + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    void clear() { erase(begin(), end()); }
protected:
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line callees retail's TU also compiled (COMDATs; the linker kept other copies):
// with their bodies visible VC7.1 knows they never write through the const value_type& / tag references.
template <> __declspec(noinline) void vector<long, allocator<long> >::_M_insert_overflow(long* __position,
    const long& __x, const __true_type&, unsigned int __fill_len, bool __atend)          // 0x00440990
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
template <class _Tp, class _Alloc>
__declspec(noinline) typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::erase(iterator __first, iterator __last)
{
    this->_M_finish = __first + (this->_M_finish - __last);
    return __first;
}
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_fill_insert(iterator __pos, size_type __n, const _Tp& __x)
{
    if (__n != 0 && __x.size() == 0)
        this->_M_finish = __pos + __n;
}
#pragma auto_inline(on)
}   // namespace std
template <class T> class CArray : public std::vector<T> { };     // retail CArray<T>: the 12-byte STLport vector

class CTriangleStripifier2 {
public:
    void* __vftable;
private:
    void BuildVertexAndPolyMaps(const CArray<unsigned short>& triangles, CArray<CArray<long> >& vertexPolys,
                                CArray<CArray<unsigned short> >& vertexNeighbours);
};

void CTriangleStripifier2::BuildVertexAndPolyMaps(const CArray<unsigned short>& triangles,
                                                  CArray<CArray<long> >& vertexPolys,
                                                  CArray<CArray<unsigned short> >& vertexNeighbours)
{
    vertexPolys.clear();
    for (long poly = 0; poly < triangles.size() / 3; ++poly) {
        for (long k = 0; k < 3; ++k) {
            long vert = triangles[poly * 3 + k];
            if (vertexPolys.size() <= vert)
                vertexPolys.resize(vert + 1);
            vertexPolys[vert].push_back(poly);
        }
    }
    long numVerts = vertexPolys.size();
    vertexNeighbours.clear();
    vertexNeighbours.resize(numVerts);
    for (long v = 0; v < numVerts; ++v) {
        for (unsigned long p = 0; p < vertexPolys[v].size(); ++p) {
            long poly = vertexPolys[v][p];
            for (long k = 0; k < 3; ++k) {
                unsigned short n = triangles[poly * 3 + k];
                if (n != v) {
                    CArray<unsigned short>& nb = vertexNeighbours[v];
                    bool found = false;
                    for (unsigned long i = 0; i < nb.size(); ++i)
                        if (nb[i] == n) {
                            found = true;
                            break;
                        }
                    if (!found)
                        nb.push_back(n);
                }
            }
        }
    }
}
