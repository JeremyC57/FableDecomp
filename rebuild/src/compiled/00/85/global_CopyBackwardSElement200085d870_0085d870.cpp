// _global::CopyBackward_SElement20_0085d870(SElement20*, SElement20*, SElement20*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 20-byte POD elements, returns the moved-back result
// Leaf: CopyBackward_SElement20_0085d870
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085D870, length 89 bytes. No callees.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall:
// first in ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed element count
// (last - first) / 20 is computed once with the reciprocal multiply, both pointers step back one element per
// iteration and the 20-byte trivially copyable element is moved as an inline dword block (ebx as the shuttle
// register, edx/edi as the per-element source/destination). The manifest label
// (`_Move_backward<pair<unsigned __int64, CPersistInfo>*>`) is propagated across byte-identical bodies of
// different element sizes, so the element is modelled as an opaque 20-byte record (same shape as the landed
// 0x00ADAB20).
struct SElement20 {
    unsigned char _body[0x14];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement20* __fastcall CopyBackward_SElement20_0085d870(SElement20* first, SElement20* last, SElement20* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}