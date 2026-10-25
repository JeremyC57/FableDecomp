// std::vector::push_back(const NGameText::CDataBankTextEntry& x) -- vector<NGameText::CDataBankTextEntry>::push_back: copy-construct x at the end (add-ref the text-bank entry), or grow through _M_insert_overflow when full
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CB740, 65 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: called once by the NGameText::CDataBank virtual at 0x009C8C00 on `lea ecx,[ebx+...]` = CDataBank::TextEntryBuffer (retail +0x1EC),
//   Ego_r.pdb CArray<NGameText::CDataBankTextEntry> (retail CArray = the 12-byte STLport vector). The catalog label is descriptive.
// Callee: _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x009CB4E0 (the __false_type tag lives in x's dead argument
//   slot: `lea edx,[esp+0Ch]` after the two constant pushes).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);` -- placement new keeps its null test; the PText copy add-refs at +4 of the pointee, then NarratorIndex
//   (CIVCountedPointer's user copy constructor `P(x.P) { if (P) ++P->RefCount; }`).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

namespace NGameText {
struct CTextEntry {                                  // intrusively counted pointee (CIVCountedPointeeBase)
    void* __vftable;                                 // +0x00
    long  RefCount;                                  // +0x04
};
}

template <class T>
struct CIVCountedPointer {                           // 4 bytes
    T* P;                                            // +0x00
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
    ~CIVCountedPointer();
};

namespace NGameText {
struct CDataBankTextEntry {                          // 8 bytes (Ego_r.pdb)
    CIVCountedPointer<CTextEntry> PText;             // +0x00
    long                          NarratorIndex;     // +0x04
};
}

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef __false_type _IsPODType;
    void push_back(const _Tp& __x);
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);   // 0x009CB4E0
private:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};

template <class _Tp, class _Alloc> void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
}

template void vector<NGameText::CDataBankTextEntry>::push_back(const NGameText::CDataBankTextEntry&);
}