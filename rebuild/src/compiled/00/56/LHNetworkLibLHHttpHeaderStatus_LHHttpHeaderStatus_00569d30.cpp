// LHNetworkLib::LHHttpHeaderStatus::~LHHttpHeaderStatus() -- reset the parsed header status, destroy the URI / host / location strings, then the document header (reset, header string, header-line vector)
// Leaf: ??1LHHttpHeaderStatus@LHNetworkLib@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569D30, 90 bytes, thiscall, tail `jmp ~vector<string>` on this+4.
// Identity (proven): Ego_r.pdb (0x005E04D0, after the ctor 0x005E0450) and FableWin.pdb (0x022E4280) spell
//   ??1LHHttpHeaderStatus@LHNetworkLib@@QAE@XZ (non-virtual; LHHttpHeaderStatus is a struct: LHHttp2::GetHeader mangles
//   AAULHHttpHeaderStatus@2@). The first call is ?Reset@LHHttpHeaderStatus@LHNetworkLib@@QAEXXZ 0x0083FB60 (landed, Ego_r donor
//   order Reset -> LHHttpHeaders::Reset = retail 0x0083FB60 -> 0x0083FBC0); the layout below is the one that Reset and the
//   landed ParseLocationData 0x0083F420 use. Callers destroy LHHttpHeaderStatus locals: CTCHeroOnlineScoreboard::UpdateTransaction
//   0x0055FB80 (four exits) and the LHHttp2 response handler 0x008411D0 (three). The catalog label "~CPatchTesselationEdgeStrip"
//   is propagated.
// Callees: LHHttpHeaderStatus::Reset 0x0083FB60, free 0x00BFEA14 (four string blocks), LHHttpHeaders::Reset 0x0083FBC0
//   (?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ, Ego_r / FableWin), vector<basic_string<char> >::~vector 0x00569940 (tail jump).
// Shape: user destructor `{ Reset(); }`; members in reverse order: hsURI, hsHost, hsLocation (STLport basic_string: the char
//   _Destroy is empty, _String_base frees the block when non-null), then hsDocumentHeader's inline user destructor
//   `{ Reset(); }` (Ego_r / FableWin ??1LHHttpHeaders@LHNetworkLib@@QAE@XZ; its own out-of-line copy is 0x00569C30) with
//   HeaderString freed and the HdrDocumentHeader vector's out-of-line destructor called.
// Layout: Ego_r.pdb names, retail 12-byte STLport containers (0x48): hsServerCode +0, hsDocumentHeader +4 (HdrDocumentHeader
//   +4, HeaderString +0x10), hsLocation +0x1C, hsDocumentSize +0x28, hsHost +0x2C, hsURI +0x38, hsPort +0x44. Local model:
//   engine/LHHttpHeaderStatus.h has these offsets but is a flattened record (global namespace, the strings and LHHttpHeaders
//   as opaque byte arrays, #pragma pack(1)), so it cannot destroy its members (header debt).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _CharT> class char_traits;
template <> class char_traits<char> {
public:
    typedef char char_type;
};
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
inline void _Destroy(char*, char*) { }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    ~_String_base() { _M_deallocate_block(); }
};
template <class _CharT, class _Traits, class _Alloc>
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    ~basic_string() { _Destroy(this->_M_start, this->_M_finish + 1); }
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

namespace LHNetworkLib {

enum LH_HTTP_SERVER_CODES { };

class LHHttpHeaders {
public:
    LHHttpHeaders();
    ~LHHttpHeaders() { Reset(); }
    void Reset();                                           // 0x0083FBC0
    std::vector<std::string> HdrDocumentHeader;             // +0x00
    std::string HeaderString;                               // +0x0C
};

struct LHHttpHeaderStatus {
    LHHttpHeaderStatus();
    ~LHHttpHeaderStatus();
    void Reset();                                           // 0x0083FB60
    LH_HTTP_SERVER_CODES hsServerCode;                      // +0x00
    LHHttpHeaders hsDocumentHeader;                         // +0x04
    std::string hsLocation;                                 // +0x1C
    long hsDocumentSize;                                    // +0x28
    std::string hsHost;                                     // +0x2C
    std::string hsURI;                                      // +0x38
    unsigned short hsPort;                                  // +0x44
};

LHHttpHeaderStatus::~LHHttpHeaderStatus()
{
    Reset();
}

} // namespace LHNetworkLib