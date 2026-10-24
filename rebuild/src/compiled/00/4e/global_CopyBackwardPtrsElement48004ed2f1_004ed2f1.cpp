// _global::CopyBackwardPtrs_Element48_004ed2f1(SElement48*, SElement48*, SElement48*, const __false_type&) -- STLport __copy_backward_ptrs over 0x48-byte records (the SGI __copy_backward inlined)
// Leaf: CopyBackwardPtrs_Element48_004ed2f1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ED2F1, length 98 bytes (size-optimised TU with frame pointers: `push
// 48h ; pop ecx ; idiv`, `push 7 ; pop ecx`, `leave`; no alignment padding).
// Shape: `ret 8` -- result and one reference argument (the caller 0x004ED210, a vector insert of the same 0x48-byte
// records, pushes a zero-filled byte local's address and the result). STLport `__copy_backward_ptrs(first, last,
// result, const __false_type&)` with the SGI `__copy_backward(..., random_access_iterator_tag(), (ptrdiff_t*)0)`
// inlined; result lives in its argument slot (`sub [ebp+8],48h`) and the destination tail is addressed as
// (result - last) + source (frame word [ebp-4]). The /Os TU inlines neither helper on its own: the inner loop and the
// record's memberwise operator= are __forceinline (an implicit operator= is emitted out of line and called).
// Element (0x48 bytes): a CDefPointeeBase-derived 0x28-byte head assigned through its out-of-line operator= @ 0x00431F10
// (the caller's scratch record resets vtable 0x01230BA0 = CDefPointeeBase on destruction), a long at +0x28 and a
// 7-dword POD block at +0x2C (`rep movsd`). The manifest's `pair<unsigned long,C3DDummyObject>` label is propagated
// (that pair is 0x38 bytes); the concrete def type is not named by this body, so the model stays descriptive.
// Callees: the head's operator= @ 0x00431F10 (catalogued as CThingBuildingDef::operator=; propagated, unverified).
#pragma optimize("s", on)
#pragma optimize("y", off)

class CRecordHead {                    // 0x28 bytes, polymorphic (vtable 0x01230BA0 in the caller's temporary)
public:
    virtual ~CRecordHead();
    unsigned char _body[0x24];         // +0x04
    CRecordHead& operator=(const CRecordHead& other);   // 0x00431F10
};

struct SRecordTail {                   // 0x1c-byte POD block
    unsigned long d[7];
};

struct SElement48 {
    CRecordHead   Head;                // +0x00
    long          Value;               // +0x28
    SRecordTail   Tail;                // +0x2C
    __forceinline SElement48& operator=(const SElement48& other)
    {
        Head = other.Head;
        Value = other.Value;
        Tail = other.Tail;
        return *this;
    }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

__forceinline SElement48* __copy_backward(SElement48* first, SElement48* last, SElement48* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

SElement48* __fastcall CopyBackwardPtrs_Element48_004ed2f1(SElement48* first, SElement48* last, SElement48* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}