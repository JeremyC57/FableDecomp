// std::vector<std::pair<CCharString, long>>::push_back(const std::pair<CCharString, long>& x) -- STLport push_back: copy-construct x at finish (placement new's null test, CCharString's out-of-line copy, then the 4-byte second member) when there is room, else grow through _M_insert_overflow(finish, x, __false_type, 1, true)
// Leaf: ?push_back@?$vector@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@QAEXABU?$pair@VCCharString@@J@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006d4970, 72 bytes, thiscall, `ret 4`.
// Identity (probable): its growth callee 0x006D4750 (also used by CThingNPC::TransferLoveState 0x006D30E0) copies through the __uninitialized_copy 0x006D3770 bound to pair<CCharString, long>, beside that pair's __copy 0x006D3B50 (family L031); Ego_r.pdb has a vector<pair<CCharString, long> >::push_back; no caller (unreferenced).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, vector::_M_insert_overflow 0x006D4750.
// Shape (family L043): STLport 4.6 vector::push_back verbatim (family L007's body with an 8-byte record); the empty
//   tag of the overflow call is homed on x's argument slot.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {     // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    vector(const vector<_Tp, _Alloc>& __x);                // out of line per element type
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);                 // 0x006D4750
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
template <class T> class CArray : public std::vector<T> {   // retail CArray<T> = the 12-byte STLport vector
public:
    void push_back(const T& x) { std::vector<T>::push_back(x); }
};
class CCharString {
public:
    CCharString(const CCharString& other);                // 0x0099EC30
private:
    void* PStringData;                                    // +0x00
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
template void std::vector<std::pair<CCharString, long> >::push_back(const std::pair<CCharString, long>&);