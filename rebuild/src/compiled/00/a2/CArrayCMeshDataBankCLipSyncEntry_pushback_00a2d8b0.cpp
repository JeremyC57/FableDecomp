// CArray<CMeshDataBank::CLipSyncEntry>::push_back(const CMeshDataBank::CLipSyncEntry& x) -- STLport push_back: copy-construct x at finish (placement new's null test, CCharString's out-of-line copy, then the 4-byte second member) when there is room, else grow through _M_insert_overflow(finish, x, __false_type, 1, true)
// Leaf: ?push_back@?$CArray@VCLipSyncEntry@CMeshDataBank@@@@QAEXABVCLipSyncEntry@CMeshDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a2d8b0, 72 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?push_back@?$CArray@VCLipSyncEntry@CMeshDataBank@@@@QAEXABVCLipSyncEntry@CMeshDataBank@@@Z; CMeshDataBank::InitialiseEntry 0x00A276D0 builds a {CCharString name, index} entry in its frame and pushes it onto the CArray returned by the lookup 0x00A2E290 (this+0x3C8); its growth callee 0x00A2D3B0 copies through the vector<CMeshDataBank::CLipSyncEntry> copy constructor 0x00A2B970 (bound).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30, vector::_M_insert_overflow 0x00A2D3B0.
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
                            bool __atend);                 // 0x00A2D3B0
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
class CMeshDataBank {
public:
    class CLipSyncEntry {                                  // Ego_r.pdb, 8 bytes
    public:
        CCharString PhonemeName;                          // +0x00
        unsigned long AnimationIndex;                     // +0x04
    };
};
template void CArray<CMeshDataBank::CLipSyncEntry>::push_back(const CMeshDataBank::CLipSyncEntry&);