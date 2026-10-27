// std::vector<CRandomAppearanceMorph::CMorphSet::CMorphSetSkeletalMorph>::push_back(const CRandomAppearanceMorph::CMorphSet::CMorphSetSkeletalMorph& x) -- STLport push_back: copy-construct x at finish (placement new's null test, CCharString's out-of-line copy, then the 4-byte second member) when there is room, else grow through _M_insert_overflow(finish, x, __false_type, 1, true)
// Leaf: ?push_back@?$vector@VCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@V?$allocator@VCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@@std@@@std@@QAEXABVCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086d7d0, 72 bytes, thiscall, `ret 4`.
// Identity (probable): its growth callee 0x0086D370 is bound ?_M_insert_overflow@?$vector@VCMorphSetSkeletalMorph@CMorphSet@CRandomAppearanceMorph@@@std@@... (its other caller CreateRandomMorphEntry 0x0086B9B2 builds CMorphSet entries); 8-byte element {CCharString Name; float Strength} = Ego_r.pdb CMorphSetSkeletalMorph; no caller (unreferenced).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, vector::_M_insert_overflow 0x0086D370.
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
                            bool __atend);                 // 0x0086D370
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
class CRandomAppearanceMorph {
public:
    class CMorphSet {
    public:
        class CMorphSetSkeletalMorph {                     // Ego_r.pdb, 8 bytes
        public:
            CCharString Name;                             // +0x00
            float Strength;                               // +0x04
        };
    };
};
template void std::vector<CRandomAppearanceMorph::CMorphSet::CMorphSetSkeletalMorph >::push_back(const CRandomAppearanceMorph::CMorphSet::CMorphSetSkeletalMorph&);