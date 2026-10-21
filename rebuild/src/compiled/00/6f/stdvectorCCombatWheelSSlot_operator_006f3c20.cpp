// std::vector<CCombatWheel::SSlot>::operator=(const vector&) -- STLport vector assignment for a 28-byte element holding an intelligent creature pointer: reallocate and copy-construct when the source is larger than the capacity, else assign over the live elements and destroy the surplus, or assign the overlap and copy-construct the rest
// Leaf: ??4
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F3C20, 325 bytes. `ret 4`.
// Label correction: catalogued as "CCombatWheel::CopyCombatWheel"; the body is the STLport vector<T>::operator=
//   instantiation for a 0x1c-byte element (byte, packed C3DVector at +1, long +0x10, CIntelligentPointer<
//   CThingCreatureBase> +0x14 -- vtable 0x0123E7D8 in the out-of-line copy helpers), the element type of a
//   CCombatWheel vector (descriptive name SSlot).
// Callees: malloc 0x00BFEA0E (allocator<T>::allocate); __uninitialized_copy(first, last, result, const __false_type&)
//          0x006F14A0 and __copy(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) 0x006F0640
//          (both `__fastcall`, labelled _Copy_impl); _Destroy(first, last) 0x006F2B00 (`__fastcall`, labelled
//          _Destroy_range); _M_clear 0x006F3450 (destroy + free; labelled CCombatWheel::ResetRings).
//
// Shape: the STLport 4.x operator= text: `if (&x != this)`, size()/capacity() as signed /28 differences cast to
// size_type (the 0x92492493 magic), `_M_allocate_and_copy` inline (`n != 0 ? malloc(n * 28) : 0`), the tag
// objects live in the dead argument slot and __copy's null distance pointer is pushed once for both arms. Same-TU
// stand-ins for __uninitialized_copy / __copy / _Destroy (auto_inline(off), above the caller) are what let every tag
// temporary share the dead argument slot (4-byte frame) and duplicate the _M_finish store into each arm, as retail does.
struct CThing;
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
};
struct SSlot {                              // 0x1c bytes
    unsigned char           _head[0x14];    // flag byte, packed C3DVector at +1, long +0x10
    CBaseIntelligentPointer Creature;       // +0x14 CIntelligentPointer<CThingCreatureBase>
};

void SlotCopyConstruct(SSlot* to, const SSlot& from);    // stand-in callees of the stand-in bodies below
void SlotAssign(SSlot* to, const SSlot& from);
void SlotDestroy(SSlot* p);
#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line STLport helpers (their template instantiations live in retail's TU): VC7.1
// then knows the tag references do not escape.
SSlot* __fastcall __uninitialized_copy(const SSlot* first, const SSlot* last, SSlot* result, const __false_type&)   // 0x006F14A0
{
    for (; first != last; ++first, ++result)
        SlotCopyConstruct(result, *first);
    return result;
}
SSlot* __fastcall __copy(const SSlot* first, const SSlot* last, SSlot* result, const random_access_iterator_tag&, int*)   // 0x006F0640
{
    for (int n = int(last - first); n > 0; --n, ++first, ++result)
        SlotAssign(result, *first);
    return result;
}
void __fastcall _Destroy(SSlot* first, SSlot* last)       // 0x006F2B00
{
    for (; first != last; ++first)
        SlotDestroy(first);
}
#pragma auto_inline(on)
inline SSlot* __copy_ptrs(const SSlot* first, const SSlot* last, SSlot* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

struct CSlotVector {                        // STLport vector<SSlot> (12 bytes)
    typedef unsigned int size_type;
    SSlot* _M_start;
    SSlot* _M_finish;
    SSlot* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    SSlot* allocate(size_type n) { return n != 0 ? (SSlot*)malloc(n * sizeof(SSlot)) : 0; }
    SSlot* _M_allocate_and_copy(size_type n, const SSlot* first, const SSlot* last)
    {
        SSlot* result = allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    void _M_clear();                        // 0x006F3450
    CSlotVector& operator=(const CSlotVector& x);
};

CSlotVector& CSlotVector::operator=(const CSlotVector& x)
{
    if (&x != this) {
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            SSlot* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            SSlot* i = __copy_ptrs(x._M_start, x._M_finish, _M_start, __false_type());
            _Destroy(i, _M_finish);
        } else {
            __copy_ptrs(x._M_start, x._M_start + size(), _M_start, __false_type());
            __uninitialized_copy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}