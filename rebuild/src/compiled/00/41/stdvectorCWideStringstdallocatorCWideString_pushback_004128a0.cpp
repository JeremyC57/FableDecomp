// std::vector<CWideString>::push_back(const CWideString& x) -- STLport 4.6: copy-construct x at the finish when there is room,
//   else grow through _M_insert_overflow (one copy, at the end)
// Leaf: ?push_back@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXABVCWideString@@@Z
// Naming wave 27 (family pb, DECISIONS_W26 D7): the STLport member, the spelling reloc_bindings binds at 0x004128A0 (0x004917B5 and
//   the vector<CWideString> callers; Ego_r.pdb / FableWin.pdb spell it the same), emitted by an explicit instantiation as the own
//   row 0x00412330 emits its _M_insert_overflow. Was a Leaf-less free __fastcall Vector_PushBack over a VectorPushBackOverlay view.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004128A0, 57 bytes, thiscall `ret 4`: CWideString(const CWideString&) 0x0099B720 under the
//   placement-new null test, `add [esi+4],4`; else _M_insert_overflow 0x00412330 (finish, x, the __false_type temporary homed in
//   x's argument slot, 1, true).
inline void* operator new(unsigned int, void* where) { return where; }

class CWideString {
public:
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
private:
    void* PStringData;                                    // +0x00
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport 4.6 vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void push_back(const _Tp& __x);
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                // 0x00412330
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        _Construct(this->_M_finish, __x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1, true);
}

template void vector<CWideString>::push_back(const CWideString&);
}
