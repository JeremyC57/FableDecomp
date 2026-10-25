// std::vector<CGraphicInfo>::_M_insert_overflow(CGraphicInfo* pos, const CGraphicInfo& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport grow path of the graphic bank's info vector: allocate size + max(size, fill_len), copy the prefix, construct fill_len copies of x, copy the suffix unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A01120, 337 bytes, thiscall `ret 14h` (pos, &x, the tag reference, fill_len,
//   atend). Callers: the vector's _M_fill_insert 0x00A01290 (0x00A013AA), the landed push_back() 0x00A01650 (it names
//   this callee) and 0x00A016DE. The catalog label "std_vector_expand" is descriptive.
// Callees: malloc @ 0x00BFEA0E, free @ 0x00BFEA14 (everything else is inline).
// Layout: Ego_r.pdb CGraphicInfo (0x2C bytes = retail: `0x2E8BA2E9 ; sar 3`, `imul 2Ch`): Header CGraphicHeader +0x00,
//   TextureType +0x1C, Bank +0x20, Index +0x24, Frames +0x28; implicit copy = `rep movsd` of 11 dwords; the destructor
//   clears Bank and Frames (the landed vector<CGraphicInfo>::erase 0x00A00E60 model).
// Shape: STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)` verbatim (the 32-byte vertex
//   sibling 0x009E14B0) with the inline __uninitialized_copy / __uninitialized_fill_n loops and the inline _Destroy
//   loop; max() takes the address of the fill_len argument. __uninitialized_copy needs `__forceinline` (plain `inline`
//   leaves the suffix copy out of line).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

class CGraphicDataBank;
class CGraphicInfoFrame;

class CGraphicHeader {                              // 0x1C bytes (Ego_r.pdb)
public:
    unsigned short Width;                           // +0x00
    unsigned short Height;                          // +0x02
    unsigned short Depth;                           // +0x04
    unsigned short FrameWidth;                      // +0x06
    unsigned short FrameHeight;                     // +0x08
    unsigned short FrameCount;                      // +0x0A
    long           PixelFormat;                     // +0x0C CPixelFormat (FormatIndex)
    unsigned char  TransparencyType;                // +0x10
    unsigned char  MipmapLevels;                    // +0x11
    unsigned char  IsFlatAnimatedGraphic : 1;       // +0x12
    unsigned long  FrameDataSize;                   // +0x14
    unsigned long  CompressedMipmapSizes[1];        // +0x18
};

enum ETextureType { TEXTURE_TYPE_NONE };

class CGraphicInfo {                                // 0x2C bytes (Ego_r.pdb)
public:
    CGraphicHeader     Header;                      // +0x00
    ETextureType       TextureType;                 // +0x1C
    CGraphicDataBank*  Bank;                        // +0x20
    long               Index;                       // +0x24
    CGraphicInfoFrame* Frames;                      // +0x28
    ~CGraphicInfo() { Bank = 0; Frames = 0; }
};

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CGraphicInfo>::_M_insert_overflow(CGraphicInfo*, const CGraphicInfo&, const __false_type&,
                                                       unsigned int, bool);
}