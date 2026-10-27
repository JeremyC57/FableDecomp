// std::vector<QuadricOptimiserInternals::CUniqueVertex>::resize(unsigned int __new_size) -- resize, filling with default records
// Leaf: ?resize@?$vector@VCUniqueVertex@QuadricOptimiserInternals@@V?$allocator@VCUniqueVertex@QuadricOptimiserInternals@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD8310, 44 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb/FableWin.pdb ?resize@?$vector@VCUniqueVertex@QuadricOptimiserInternals@@V?$allocator@VCUniqueVertex@QuadricOptimiserInternals@@@std@@@std@@QAEXI@Z; its callees are
//   QuadricOptimiserInternals::CUniqueVertex's default constructor 0x00AD4C40 and destructor 0x00AD4A70 (bound) and the two-argument resize 0x00AD8090.
// Callees: QuadricOptimiserInternals::CUniqueVertex::CUniqueVertex() 0x00AD4C40, vector<QuadricOptimiserInternals::CUniqueVertex>::resize(unsigned int, const QuadricOptimiserInternals::CUniqueVertex&) 0x00AD8090, QuadricOptimiserInternals::CUniqueVertex::~CUniqueVertex() 0x00AD4A70.
// Shape (family M116, representative 0x0051D5C0): STLport 4.6 `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`
//   -- the temporary (0x1C bytes) in the frame, its address from the constructor's return.
namespace QuadricOptimiserInternals {
class CUniqueVertex {                                    // 0x1C bytes in retail
public:
    CUniqueVertex();                                             // 0x00AD4C40
    ~CUniqueVertex();                                            // 0x00AD4A70
    unsigned char _data[0x1C];
};
}
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    void resize(unsigned int __new_size, const _Tp& __x);  // 0x00AD8090
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template void vector<QuadricOptimiserInternals::CUniqueVertex>::resize(unsigned int);
}