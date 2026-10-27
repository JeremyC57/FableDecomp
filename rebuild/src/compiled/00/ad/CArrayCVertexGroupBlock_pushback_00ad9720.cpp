// CArray<CVertexGroupBlock>::push_back(const CVertexGroupBlock& x) -- STLport push_back: copy-construct x at finish (placement new's null test, each member's out-of-line copy constructor) when there is room, else grow through _M_insert_overflow(finish, x, __false_type, 1, true)
// Leaf: ?push_back@?$CArray@VCVertexGroupBlock@@@@QAEXABVCVertexGroupBlock@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ad9720, 78 bytes, thiscall, `ret 4`.
// Identity (probable): the element copy is vector<unsigned long> + map<unsigned long, long> = Ego_r.pdb CVertexGroupBlock {CArray<unsigned long> Groups; map<unsigned long, long> GroupToIndexMap}, the overflow callee 0x00ADCE60 is bound to the CVertexGroupBlock growth insert; Ego_r.pdb defines ?push_back@?$CArray@VCVertexGroupBlock@@@@QAEXABVCVertexGroupBlock@@@Z; no caller (unreferenced). Two byte-identical CVertexGroupBlock push_backs exist (0x00AD9720, 0x00ADCFD0): one symbol each, this one the CArray member (it sits in the CVertexGroupBlockList code), the other the vector's own -- the split between the two is by position only.
// Callees: std::vector<unsigned long>::vector(const vector&) 0x00535470 (bound), std::map<unsigned long, long>::map(const map&) 0x00A96E40 (landed), vector<CVertexGroupBlock>::_M_insert_overflow 0x00ADCE60.
// Shape (family L007, representative 0x0051C800): STLport 4.6 vector::push_back verbatim, inline in the CArray member;
//   the record's implicit copy constructor copies its two members out of line (ebx = &x, then &x + member offset); the
//   empty tag of the overflow call is homed on x's argument slot.
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
                            bool __atend);                 // 0x00ADCE60
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
namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                                // STLport 4.6, 12 bytes (one _Rb_tree)
public:
    void* _M_t[3];
    map(const map<_Key, _Tp, _Compare, _Alloc>& __x);       // 0x00A96E40 for <unsigned long, long>
};
}
class CVertexGroupBlock {                                   // Ego_r.pdb; retail 24 bytes
public:
    CArray<unsigned long> Groups;                         // +0x00
    std::map<unsigned long, long> GroupToIndexMap;        // +0x0C
};
template void CArray<CVertexGroupBlock>::push_back(const CVertexGroupBlock&);