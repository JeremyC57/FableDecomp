// NInventory::CPerItemData::`scalar deleting destructor'(unsigned int flags) -- destroy the record (free its vector block) and free it when bit 0 is set
// Leaf: ??_GCPerItemData@NInventory@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C1537, 36 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its caller is CCountedPointer<NInventory::CPerItemData>::DeleteData 0x005C18E3 (bound; `delete (CPerItemData*)data` = this with flags 1); Ego_r.pdb NInventory::CPerItemData's only non-trivial member is vector<long> Augmentations at +0.
// Callees: free 0x00BFEA14 (the vector's block), operator delete 0x00BFE9BC.
// Shape (family M100, representative 0x004E1CFC): the compiler-generated non-virtual `??_G` -- the implicit destructor inline
//   (STLport ~_Vector_base: `if (_M_start) deallocate`), then the flag test; the delete-expression in DeleteOne forces it into
//   the object (not part of the retail body).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};
}
namespace NInventory {
class CPerItemData {                                      // Ego_r.pdb, 0x24 bytes in retail
public:
    std::vector<long> Augmentations;                      // +0x0
    long NumberOfKills;                                   // +0xc
    long TimeWielded;                                     // +0x10
    long BestTrophyWitnessesAheadToDate;                  // +0x14
    bool QuestPersistent;                                 // +0x18
    long QuestPersistentRefCount;                         // +0x1c
    float BestFishWeight;                                 // +0x20
};
}

void DeleteOne(NInventory::CPerItemData* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}