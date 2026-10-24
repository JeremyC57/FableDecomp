// _STL::vector<CLipSyncPhonemeSample>::push_back(const CLipSyncPhonemeSample& x) -- append a lip-sync phoneme sample: placement copy at the finish pointer when there is room (add-ref the sample frame), else _M_insert_overflow(finish, x, __false_type(), 1, true)
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A4AE00, length 97 bytes, `ret 4`.
// Caller: the `jmp` at 0x00A495E0 (an out-of-line forwarder). Callees: CCharString copy ctor 0x0099EC30, 0x00A4AC60
//   vector<CLipSyncPhonemeSample>::_M_insert_overflow.
// Identity: catalogued as `Vector_CCharStringCountedPtr_Append`. The 16-byte element is Ego_r.pdb CLipSyncPhonemeSample:
//   Symbol CCharString +0, ID unsigned long +4, SampleFrame CCountedPointer<CSpeechFrame> +8 (Data +8, count block +0xC
//   whose first word is the reference count).
// Shape: STLport 4.x `push_back` verbatim; the placement copy (null test) runs the implicit copy ctor: the string's
//   out-of-line copy ctor, the ID word, then the counted pointer's user copy ctor (Data, then the block with `inc [blk]`);
//   the overflow arm loads x before the pushes so the empty tag is homed in x's dead argument slot.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
};

struct CCPPointerInfo {                              // count block: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;                            // +0x00
    CCPPointerInfo* Info;                            // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

class CSpeechFrame;
class CLipSyncPhonemeSample {                        // 16 bytes
public:
    CCharString                   Symbol;            // +0x00
    unsigned long                 ID;                // +0x04
    CCountedPointer<CSpeechFrame> SampleFrame;       // +0x08
};

namespace _STL {
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
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A4AC60
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

template void vector<CLipSyncPhonemeSample>::push_back(const CLipSyncPhonemeSample&);
}