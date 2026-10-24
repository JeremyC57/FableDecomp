// _STL::__uninitialized_fill_n(CActiveSoundInfo* first, unsigned int n, const CActiveSoundInfo& x, const __false_type&) -- copy-construct n copies of x into raw storage at first and return the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACB50, length 54 bytes, __fastcall (first in ecx, n in edx, the
//   value and the empty tag reference on the stack, `ret 8`), returns the end in eax.
// Identity: called from vector<CActiveSoundInfo>::_M_insert_overflow 0x00AACDA0 (0x00AACE3A, the fill_len != 1 arm);
//   element vtable 0x0129E674 = CActiveSoundInfo. The manifest's "_Fill_n<unsigned_long*,...>" is propagated and wrong.
// Shape: STLport 4.x `for (; n > 0; --n, ++cur) _Construct(&*cur, x)`: unsigned count (`jbe`), `dec edx ; jne`,
//   null-tested placement copy (vtable store + three words).
inline void* operator new(unsigned int, void* p) { return p; }

class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};

class CActiveSoundInfo : public CBaseClass {       // vtable 0x0129E674, 16 bytes
public:
    void* m_pSound;                                // +0x04
    void* m_pOwner;                                // +0x08
    void* m_pInfo;                                 // +0x0c
    virtual ~CActiveSoundInfo();
};

namespace _STL {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CActiveSoundInfo* __fastcall __uninitialized_fill_n(CActiveSoundInfo*, unsigned int, const CActiveSoundInfo&,
                                                             const __false_type&);
}