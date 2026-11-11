// _global::CopyBackward_SElement12_0075b5b0(SElement12*, SElement12*, SElement12*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 12-byte POD elements, returns the moved-back result
// Leaf: CopyBackward_SElement12_0075b5b0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075B5B0, length 76 bytes. No callees.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall:
// first in ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed element count
// (last - first) / 12 is computed once with the reciprocal multiply, both pointers step back one element per
// iteration and the 12-byte trivially copyable element is moved as an inline dword block (ebx as the shuttle
// register, edx/edi as the per-element source/destination). The manifest label
// (`_Move_backward<pair<unsigned __int64, CPersistInfo>*>`) is propagated across byte-identical bodies of
// different element sizes, so the element is modelled as an opaque 12-byte record (same shape as the landed
// 0x00ADAB20).
struct SElement12 {
    unsigned char _body[0xc];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement12* __fastcall CopyBackward_SElement12_0075b5b0(SElement12* first, SElement12* last, SElement12* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}