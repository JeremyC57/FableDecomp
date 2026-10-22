// CRetailString::_M_append_dispatch(const char*, const char*, const __false_type&) -- STLport string append dispatch for a non-integer iterator pair: append(first, last, iterator category)
// Leaf: _M_append_dispatch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567E80, 13 bytes. Callees: append(first, last, const forward_iterator_tag&)
// @ 0x00567400 (label "CTCElectrocutedEffect::InsertStringRange" is propagated; it is the STLport range
// append: grows with operator new 0x00BFEA0E, two memmoves, frees with `free`, `ret 0Ch`).
// Unreferenced in the image (recovered from a mis-cut catalog row).
// Shape: a sibling tail jump -- the arguments stay in place, the empty iterator-category temporary is homed
// in the dead `last` slot and its address overwrites the dead tag slot (`lea eax,[esp+8] ; mov [esp+0Ch],eax ; jmp`).
struct __false_type { __false_type() {} };
struct forward_iterator_tag { forward_iterator_tag() {} };

struct CRetailString {
    char* _M_start;                   // +0
    char* _M_finish;                  // +4
    char* _M_end_of_storage;          // +8
    template <class Iter, class Tag> CRetailString& append(Iter first, Iter last, const Tag&);   // 0x00567400
    CRetailString& _M_append_dispatch(const char* first, const char* last, const __false_type&);
};

void* __cdecl GrowStringStorage(CRetailString* s, unsigned int extra);   // stand-in for the growth path
// Same-TU body of the STLport range append 0x00567400 (category tag unused).
template <class Iter, class Tag> __declspec(noinline) CRetailString& CRetailString::append(Iter first, Iter last, const Tag&)
{
    if (first != last) {
        unsigned int n = (unsigned int)(last - first);
        if ((unsigned int)(_M_end_of_storage - _M_finish) <= n)
            GrowStringStorage(this, n);
        for (; first != last; ++first)
            *_M_finish++ = *first;
        *_M_finish = 0;
    }
    return *this;
}

CRetailString& CRetailString::_M_append_dispatch(const char* first, const char* last, const __false_type&)
{
    return append(first, last, forward_iterator_tag());
}