// CUnprovenRecord_005bbdcc::`scalar deleting destructor'(unsigned int flags) -- destroy the record (free its vector block) and free it when bit 0 is set
// Leaf: ??_GCUnprovenRecord_005bbdcc@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BBA28, 36 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (descriptive): its callers are the __destroy_aux 0x005BBDCC (family M097, 12-byte stride, flags 0) and the _Destroy(p) wrapper 0x005BBBC6 of the same TU; the element (12 bytes, a freed block first) is not pinned by Ego_r.pdb -- named after M097's row.
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
class CUnprovenRecord_005bbdcc {                          // descriptive: 12 bytes, its first word a malloc'd block
public:
    ~CUnprovenRecord_005bbdcc() { if (Block != 0) free(Block); }
    void* Block;
    unsigned long Members[2];
};

void DeleteOne(CUnprovenRecord_005bbdcc* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}