// std::vector<CSubtitledMessage>::resize(unsigned int __new_size) -- resize, filling with default records
// Leaf: ?resize@?$vector@VCSubtitledMessage@@V?$allocator@VCSubtitledMessage@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A630, 44 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb/FableWin.pdb ?resize@?$vector@VCSubtitledMessage@@V?$allocator@VCSubtitledMessage@@@std@@@std@@QAEXI@Z; its callees are
//   CSubtitledMessage's default constructor 0x004CDB70 and destructor 0x004D1DB0 (bound) and the two-argument resize 0x0071A5C0.
// Callees: CSubtitledMessage::CSubtitledMessage() 0x004CDB70, vector<CSubtitledMessage>::resize(unsigned int, const CSubtitledMessage&) 0x0071A5C0, CSubtitledMessage::~CSubtitledMessage() 0x004D1DB0.
// Shape (family M116, representative 0x0051D5C0): STLport 4.6 `void resize(size_type __new_size) { resize(__new_size, _Tp()); }`
//   -- the temporary (0x44 bytes) in the frame, its address from the constructor's return.
class CSubtitledMessage {                                    // 0x44 bytes in retail
public:
    CSubtitledMessage();                                             // 0x004CDB70
    ~CSubtitledMessage();                                            // 0x004D1DB0
    unsigned char _data[0x44];
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    void resize(unsigned int __new_size, const _Tp& __x);  // 0x0071A5C0
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
template void vector<CSubtitledMessage>::resize(unsigned int);
}