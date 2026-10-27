// std::vector::push_back(const CVertexGroupBlockOptimiser::CBlockInfo& __x) -- (vector<CVertexGroupBlockOptimiser::CBlockInfo >) append a copy, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@VCBlockInfo@CVertexGroupBlockOptimiser@@V?$allocator@VCBlockInfo@CVertexGroupBlockOptimiser@@@std@@@std@@QAEXABVCBlockInfo@CVertexGroupBlockOptimiser@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADBFB0, 64 bytes, thiscall, `ret 4`.
// Identity (probable): its only caller CVertexGroupBlockOptimiser::AddBlocks 0x00AD9F10 pushes a 12-byte stack record {block index, count, group count} onto a local vector; Ego_r.pdb CVertexGroupBlockOptimiser::CBlockInfo is the 12-byte {BlockIndex, Cost, Benefit} that the same module's landed __insertion_sort / __final_insertion_sort / sort_heap (0x00ADBE70, 0x00ADC080, 0x00ADBCB0) order; its overflow 0x00ADBD50 divides by 12 and copies through __false_type helpers.
// Callees: vector<CVertexGroupBlockOptimiser::CBlockInfo>::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x00ADBD50.
// Shape (family M185, representative 0x0066E630): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the implicit 12-byte copy
//   (three dword moves through esi) under placement new's null test; the empty tag homed in the dead __x slot. Speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CVertexGroupBlockOptimiser {
public:
    class CBlockInfo {                                    // Ego_r.pdb, 12 bytes
    public:
        unsigned long BlockIndex;
        long Cost;
        long Benefit;
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00ADBD50
};
}
namespace std {
template void vector<CVertexGroupBlockOptimiser::CBlockInfo >::push_back(const CVertexGroupBlockOptimiser::CBlockInfo& __x);
}