// CRetailVector20::reserve(unsigned long) -- STLport vector<T>::reserve for a 20-byte element (file-installer records)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2013, length 115 bytes (`ret 4`; size TU with frame pointer).
// Callees: vector::_M_allocate_and_copy(n, first, last) @ 0x005C10F7, vector::_M_clear() @ 0x005C1F1F (manifest
// label "CActiveFile::OnReadFinished" is propagated), CRetailAllocator_20::allocate(n, hint) @ 0x005C0436 (landed).
// Label: the manifest's "CActiveFile::ReserveSpace" does not fit -- `this` is the vector itself ({start,
// finish, end of storage} at +0/+4/+8, CActiveFile holds file handles there); the only caller (0x005BFB32)
// reserves `size() + n` elements. The element type is named by its size, like the landed allocator.
//
// Shape: STLport `reserve`: `capacity() < n` (signed `idiv` by 20 through `push 14h ; pop edi`, unsigned
// compare), the old size computed before the reallocation and kept in the frame word, then either
// _M_allocate_and_copy + _M_clear (a buffer exists) or the end-of-storage allocator's allocate(n, 0) (on
// `&_M_end_of_storage`, kept in ebx), and the inline _M_set(start, start + size, start + n). The frame pointer
// with `leave` and the push/pop constant are `optimize("y", off)` + `optimize("s", on)`.
// Leaf: ?reserve@CRetailVector20
#pragma optimize("y", off)
#pragma optimize("s", on)

struct SRecord20 {
    unsigned char bytes[20];
};

struct CAllocProxy20 {                          // _STLP_alloc_proxy<SRecord20*, ...>: the allocator + its pointer
    SRecord20* _M_data;
    SRecord20* allocate(unsigned long count, const void* hint = 0);   // 0x005C0436
};

struct CRetailVector20 {
    SRecord20* _M_start;                        // +0x00
    SRecord20* _M_finish;                       // +0x04
    CAllocProxy20 _M_end_of_storage;            // +0x08
    unsigned long size() const { return _M_finish - _M_start; }
    unsigned long capacity() const { return _M_end_of_storage._M_data - _M_start; }
    SRecord20* _M_allocate_and_copy(unsigned long count, SRecord20* first, SRecord20* last);   // 0x005C10F7
    void _M_clear();                            // 0x005C1F1F
    void _M_set(SRecord20* start, SRecord20* finish, SRecord20* endOfStorage)
    {
        _M_start = start;
        _M_finish = finish;
        _M_end_of_storage._M_data = endOfStorage;
    }
    void reserve(unsigned long count);
};

void CRetailVector20::reserve(unsigned long count)
{
    if (capacity() < count) {
        const unsigned long oldSize = size();
        SRecord20* buffer;
        if (_M_start != 0) {
            buffer = _M_allocate_and_copy(count, _M_start, _M_finish);
            _M_clear();
        } else {
            buffer = _M_end_of_storage.allocate(count);
        }
        _M_set(buffer, buffer + oldSize, buffer + count);
    }
}
