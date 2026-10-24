// _global::CopyBackward_SElement24_00819670(SElement24*, SElement24*, SElement24*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 24-byte POD elements, returns the moved-back result
// Leaf: CopyBackward_SElement24_00819670
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00819670, length 95 bytes. No callees.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall:
// first in ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed element count
// (last - first) / 24 is computed once with the reciprocal multiply, both pointers step back one element per
// iteration and the 24-byte trivially copyable element is moved as an inline dword block (ebx as the shuttle
// register, edx/edi as the per-element source/destination). The manifest label
// (`_Move_backward<pair<unsigned __int64, CPersistInfo>*>`) is propagated across byte-identical bodies of
// different element sizes, so the element is modelled as an opaque 24-byte record (same shape as the landed
// 0x00ADAB20).
struct SElement24 {
    unsigned char _body[0x18];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement24* __fastcall CopyBackward_SElement24_00819670(SElement24* first, SElement24* last, SElement24* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}