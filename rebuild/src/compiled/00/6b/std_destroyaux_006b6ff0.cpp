// std::__destroy_aux(std::vector<SThemeBlock>* first, std::vector<SThemeBlock>* last, const __false_type&) -- destroy a range of vectors of 500-byte theme blocks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6FF0, length 59 bytes.
// Leaf: ?__destroy_aux@std@@YIXPAU?$vector
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. Its caller 0x006B7040
// (`push ecx ; lea eax,[esp+3] ; push eax ; call`) is the inline `_Destroy(first, last)` passing an empty tag, so this is
// STLport `__destroy_aux(first, last, const __false_type&)` over 12-byte vectors. Each element's ~vector calls the
// out-of-line `__destroy_aux` of its own 0x1F4-byte elements @ 0x006B67A0 (that body walks three {vector, vector} pairs
// at the end of each block) and then frees its block. The block type sits in the environment-theme code (callers
// 0x00888AB8 CEnvironmentThemeSet "BuildDay" area); it is not identified, so it is modelled as SThemeBlock.
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: std::__destroy_aux<SThemeBlock*> @ 0x006B67A0, free @ 0x00BFEA14.
// Shape: the inner `_Destroy(start, finish)` passes a NAMED empty tag local, homed in this function's `push ecx` slot
// (`lea eax,[esp+0Bh]`); the vector is the STLport _Vector_base + _STLP_alloc_proxy structure.
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
struct SThemeBlock {                        // 0x1F4-byte environment-theme block (opaque)
    unsigned char _data[0x1f4];
};
namespace std {
struct __false_type { };
void __fastcall __destroy_aux(SThemeBlock* first, SThemeBlock* last, const __false_type&);   // 0x006B67A0
inline void _Destroy(SThemeBlock* first, SThemeBlock* last)
{
    __false_type tag;
    __destroy_aux(first, last, tag);
}
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

void __fastcall __destroy_aux(vector<SThemeBlock>* first, vector<SThemeBlock>* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}