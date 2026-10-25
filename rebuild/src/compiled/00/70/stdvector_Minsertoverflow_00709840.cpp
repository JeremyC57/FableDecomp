// std::vector::_M_insert_overflow(CModifierGraphic* position, const CModifierGraphic& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the hero appearance modifier-graphic vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00709840, 239 bytes, `ret 0x14`.
// Label correction: catalog "Vector_Reallocate_CNavigationPosition" -> STLport 4.x
//   vector<CTCHeroAttachableAppearanceModifiers::CModifierGraphic>::_M_insert_overflow (the callers of the landed
//   __uninitialized_copy 0x00708B60 / __uninitialized_fill_n 0x00708BA0 name the element; 12 bytes: the 0x2AAAAAAB /
//   `sar edx,1` divide).
// Element: Ego_r.pdb CModifierGraphic = CDefPointer<const CTextureReplacementDef> +0 (user copy ctor: copy, null test,
//   `inc [obj+4]`), BankIndex +4, RenderSizeX +8.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x00708B60, __uninitialized_fill_n 0x00708BA0,
//   __destroy_aux 0x00709350.
// Shape: STLport `_M_insert_overflow` (see 0x006FC6A0): len = size + max(size, fill_len) via the const-ref `max`,
//   allocate, copy the head, construct one element in place (the counted def pointer's add-ref inline) or fill_n, copy
//   the tail unless atend, _M_clear, _M_set. The empty `__false_type` tags are homed in the high argument slot; the
//   tag-taking callees are same-TU noinline stand-ins (their real bodies are separate rows).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CDefCountedBase {                        // counted def: vtable +0, reference count +4
public:
    virtual void v00();
    virtual void Release();                    // slot 1
    unsigned long RefCount;                    // +0x04
};

template<class T> class CDefPointer {          // 4-byte intrusive pointer
public:
    CDefCountedBase* Object;                   // +0x00
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        if (Object) {
            if (--Object->RefCount == 0)
                Object->Release();
            Object = 0;
        }
    }
};

class CTextureReplacementDef;

namespace CTCHeroAttachableAppearanceModifiers {
class CModifierGraphic {                       // Ego_r.pdb, 12 bytes
public:
    CDefPointer<const CTextureReplacementDef> PTextureReplacement;   // +0x00
    long  BankIndex;                           // +0x04
    float RenderSizeX;                         // +0x08
};
}

namespace std {

struct __false_type { __false_type() {} };

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x00708B60, 0x00708BA0, 0x00709350).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
template<class T> __declspec(noinline) void __fastcall __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~T();
}
#pragma auto_inline(on)

template<class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<CTCHeroAttachableAppearanceModifiers::CModifierGraphic>;

}