// CArray<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc>::push_back(const CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc& x) -- STLport push_back: copy-construct x at finish (placement new's null test, each member's out-of-line copy constructor) when there is room, else grow through _M_insert_overflow(finish, x, __false_type, 1, true)
// Leaf: ?push_back@?$CArray@UCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@@@QAEXABUCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051c800, 78 bytes, thiscall, `ret 4`.
// Identity (probable): its overflow callee is the landed vector<CSeaBodyDesc>::_M_insert_overflow 0x0051C110 and the element copy is CCharString + vector<long> (Ego_r.pdb CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc {CCharString Identifier; CArray<long> Maps}); Ego_r.pdb defines ?push_back@?$CArray@UCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@@@QAEX...@Z out of line; no caller (unreferenced).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, std::vector<long>::vector(const vector&) 0x00514160 (CArray<long>'s implicit copy), vector<CSeaBodyDesc>::_M_insert_overflow 0x0051C110 (landed).
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
                            bool __atend);                 // 0x0051C110
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
    void* PData;                                          // +0x00
    CCharString(const CCharString& other);                // 0x0099EC30
};
class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {                                 // Ego_r.pdb (struct); retail 16 bytes
        CCharString Identifier;                           // +0x00
        CArray<long> Maps;                                // +0x04
    };
};
template void CArray<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc>::push_back(const CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc&);