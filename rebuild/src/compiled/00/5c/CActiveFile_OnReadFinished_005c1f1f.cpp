// CRetailVector20::_M_clear() -- STLport vector<T>::_M_clear for the 20-byte file-installer record: destroy every element, then give the buffer back
// Leaf: _M_clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C1F1F, length 35 bytes (size TU). Callees: the element destroy loop
// _Destroy_range(first, last, allocator&) @ 0x005C18C6 (`__fastcall`, 0x14-byte stride; manifest label names the
// NCreatureHitNotification::CFlash instantiation), free @ 0x00BFEA14 (the allocator's deallocate, inlined).
// Called by vector::reserve @ 0x005C2013 (landed as CRetailVector20::reserve) after the elements were moved to a new
// buffer. The catalog name "CFileInstaller::CActiveFile::OnReadFinished" is a propagated label: `this` is the vector
// itself ({start, finish, end of storage} at +0/+4/+8) and nothing here touches a file.
//
// Every element is destroyed first, whether or not there is a buffer (the loop simply runs zero times when start and
// finish are both null), and only the release of the storage is guarded against a null buffer: that is the
// allocator's deallocate() contract, which must accept the null pointer an empty vector holds.
//
// Matching: the stateless element allocator is a one-byte stack temporary whose address is pushed
// (`lea eax,[esp+7]`), the deallocate is the inline `if (p) free(p)`, and `optimize("s")` gives the `pop ecx`
// argument clean-up and the single `push ecx` frame slot.
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14

struct SRecord20 {
    unsigned char bytes[20];
};

struct CRetailAllocator20 {                                      // stateless element allocator
};

void __fastcall DestroyRange20(SRecord20* first, SRecord20* last, CRetailAllocator20* allocator);   // 0x005C18C6

struct CAllocProxy20 {                                           // _STLP_alloc_proxy<SRecord20*, ...>: the allocator + its pointer
    SRecord20* _M_data;
    void deallocate(SRecord20* block, unsigned long count)
    {
        if (block != 0)
            free(block);
    }
};

struct CRetailVector20 {
    SRecord20*    _M_start;                                      // +0x00
    SRecord20*    _M_finish;                                     // +0x04
    CAllocProxy20 _M_end_of_storage;                             // +0x08
    void _M_clear();
};

void CRetailVector20::_M_clear()
{
    CRetailAllocator20 allocator;
    DestroyRange20(_M_start, _M_finish, &allocator);
    _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
}
