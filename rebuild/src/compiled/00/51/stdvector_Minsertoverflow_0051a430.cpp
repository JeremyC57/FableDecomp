// std::vector::_M_insert_overflow(CLevelFiles* pos, const CLevelFiles& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth: move into a size + max(size, fill_len) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051A430, 304 bytes, thiscall `ret 14h`. Callers: vector<CLevelFiles>::push_back
//   0x0051AE60 (landed: `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`, call at 0x0051AEC2) and 0x00500B3A.
//   The catalog label "VectorReallocateAndMove" is descriptive.
// Callees: malloc @ 0x00BFEA0E, __uninitialized_copy @ 0x00513500 (prefix + suffix; landed), __uninitialized_fill_n
//   @ 0x00513570, CLevelFiles::~CLevelFiles @ 0x0050F980 (the inline _Destroy loop), free @ 0x00BFEA14.
// Layout: CLevelLoader::CObject::CLevelFiles (Ego_r.pdb, 28 bytes = `imul 1Ch`, `0x92492493`): LevelID +0,
//   CCountedPointer<CAFile> PTopologyFile +4 / PNavigationFile +0xC / PThingFile +0x14 ({Data, CCPPointerInfo* Info},
//   the copy bumps Info->RefCount) -- the landed push_back 0x0051AE60 model.
// Shape: STLport 4.6 `_M_insert_overflow(.., const __false_type&, ..)` verbatim; the tag-taking callees retail calls
//   carry same-TU noinline bodies (tags on the live `atend` slot, GOTCHAS 0x4ADB50). `_Copy_Construct` must be
//   `__forceinline`: with the 28-byte implicit copy (three counted-pointer bumps) VC7.1 otherwise calls it out of line.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CAFile;

class CCPPointerInfo { public:                               // 12 bytes
    unsigned long RefCount;                           // +0x00
    void (__cdecl *DeleteFunc)(void*);                // +0x04
    void* Data;                                       // +0x08
};

template <class T> class CCountedPointer {            // 8 bytes
public:
    T* Data;                                          // +0x00
    CCPPointerInfo* Info;                             // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};

class CLevelLoader {
public:
    class CObject {
    public:
        class CLevelFiles {                           // 28 bytes (Ego_r.pdb)
        public:
            long LevelID;                             // +0x00
            CCountedPointer<CAFile> PTopologyFile;    // +0x04
            CCountedPointer<CAFile> PNavigationFile;  // +0x0C
            CCountedPointer<CAFile> PThingFile;       // +0x14
            ~CLevelFiles();                           // 0x0050F980
        };
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> __forceinline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x00513500
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                 // 0x00513570
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
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
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<CLevelLoader::CObject::CLevelFiles>;
}