// std::vector<CRegion>::resize(unsigned int __new_size) -- resize, filling with default records
// Leaf: ?resize@?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051D5C0, 44 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb/FableWin.pdb ?resize@?$vector@VCRegion@@V?$allocator@VCRegion@@@std@@@std@@QAEXI@Z; its callees are
//   CRegion's default constructor 0x006BC410 and destructor 0x0051A110 (bound) and the two-argument resize 0x0051D350.
// Callees: CRegion::CRegion() 0x006BC410, vector<CRegion>::resize(unsigned int, const CRegion&) 0x0051D350, CRegion::~CRegion() 0x0051A110.
// Shape (family M116, representative 0x0051D5C0): STLport 4.6 `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`
//   -- the temporary (0x58 bytes) in the frame, its address from the constructor's return.
class CRegion {                                    // 0x58 bytes in retail
public:
    CRegion();                                             // 0x006BC410
    ~CRegion();                                            // 0x0051A110
    unsigned char _data[0x58];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    void resize(unsigned int __new_size, const _Tp& __x);  // 0x0051D350
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template void vector<CRegion>::resize(unsigned int);
}