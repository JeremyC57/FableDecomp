// std::vector::push_back(const CTVertexLandscapeBackground& x) -- (vector<CTVertexLandscapeBackground>) append a 24-byte landscape vertex or grow through _M_insert_overflow
// Leaf: ?push_back@?$vector@VCTVertexLandscapeBackground@@V?$allocator@VCTVertexLandscapeBackground@@@std@@@std@@QAEXABVCTVertexLandscapeBackground@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE4380, length 82 bytes, thiscall `ret 4`.
// Identity (naming wave 20, chunk 688): the call `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` into 0x00B6D0B0 -- the
//   vector<CTVertexLandscapeBackground>::_M_insert_overflow the chunk 688 _M_fill_insert 0x00B6CE00 proves (through the PDB resize 0x00B6CD10) --
//   is STLport 4.6 _vector.h push_back; a member of that vector is the only caller shape that passes (finish, x, 1, true). The element is Ego_r
//   CTVertexLandscapeBackground (24 bytes). Was the descriptive CRetailVector_SRecord24::PushBack_SRecord24_00be4380 (the manifest label
//   "CScriptModuleFullTransition::OnInitLeaf" is propagated). FableWin.pdb does not carry this push_back (the debug build never grew the vector
//   through it), so the spelling rests on the STLport text and the callee.
// Shape: `this` stays in ecx (no frame); the placement copy is six direct dword moves through esi (the trivially copyable element). The full
//   branch passes a const reference to the empty __false_type tag whose 1-byte temporary VC homes in this function's dead argument slot
//   (`lea edx,[esp+0Ch]` after the two `push 1`).
#include "fable_compiler.h"
inline void* operator new(unsigned int, void* __p) { return __p; }

class CTVertexLandscapeBackground {                // Ego_r.pdb, 24 bytes (CTVertexLandscapeBackgroundBase layout; the chunk 688 _M_fill_insert 0x00B6CE00 model)
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float Z;                                       // +0x04
    float NormalX;                                 // +0x08
    float NormalY;                                 // +0x0C
    float NormalZ;                                 // +0x10
    unsigned char UVPad1;                          // +0x14
    unsigned char V1;                              // +0x15
    unsigned char U1;                              // +0x16
    unsigned char UVPad2;                          // +0x17
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2>
FABLE_FORCEINLINE void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                  // 0x00B6D0B0
    public:
    void push_back(const _Tp& __x) {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template void vector<CTVertexLandscapeBackground>::push_back(const CTVertexLandscapeBackground&);
}
