// CRetailString::_M_append_dispatch(const char*, const char*, const __false_type&) -- STLport string append dispatch for a non-integer iterator pair: append(first, last, iterator category)
// Grade: DIFFER(23v13) -- VC7.1 calls append (push x3 ; call ; ret 0Ch) where retail tail-jumps with the tag address written into the dead third slot; the identical 2-argument sibling 0x00568800 shows the call shape
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
    CRetailString& append(const char* first, const char* last, const forward_iterator_tag&);   // 0x00567400
    CRetailString& _M_append_dispatch(const char* first, const char* last, const __false_type&);
};

CRetailString& CRetailString::_M_append_dispatch(const char* first, const char* last, const __false_type&)
{
    return append(first, last, forward_iterator_tag());
}
