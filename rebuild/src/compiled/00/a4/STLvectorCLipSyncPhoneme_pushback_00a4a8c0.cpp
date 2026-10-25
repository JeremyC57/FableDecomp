// std::vector<CLipSyncPhoneme>::push_back(const CLipSyncPhoneme& x) -- append a lip-sync phoneme: placement copy at the finish pointer when there is room, else _M_insert_overflow(finish, x, __false_type(), 1, true)
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4A8C0, length 78 bytes (+2 int3 pad), `ret 4`.
// Callees: CCharString copy ctor 0x0099EC30, 0x00A4A710 vector<CLipSyncPhoneme>::_M_insert_overflow (the catalog's
//   "vector_insert"; it calls the phoneme __uninitialized_copy 0x00A48670).
// Identity: catalogued as `Vector_CCharString_12byte_Append`; no call/jmp/absolute references to it survive. The 12-byte element
//   {CCharString, dword, dword} is Ego_r.pdb CLipSyncPhoneme (Symbol +0, ID +4, Weight +8) of the lip-sync TU around it.
//   Byte twin of 0x00A4AB10 (both reach the same _M_insert_overflow; same source).
// Shape: STLport 4.x `push_back` verbatim: the finish pointer is read into esi and compared with the end of storage in
//   memory; the placement copy (null test) runs the implicit copy ctor (string copy ctor, two dword moves) and the
//   finish pointer is bumped in memory (`add [edi+4],0Ch`); the overflow arm loads x before the pushes, so the empty
//   `__false_type()` tag is homed in x's dead argument slot (`lea edx,[esp+14h]`) and a declaration of the callee suffices.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
};

class CLipSyncPhoneme {                              // 12 bytes
public:
    CCharString   Symbol;                            // +0x00
    unsigned long ID;                                // +0x04
    float         Weight;                            // +0x08
};

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A4A710
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template void vector<CLipSyncPhoneme>::push_back(const CLipSyncPhoneme&);
}