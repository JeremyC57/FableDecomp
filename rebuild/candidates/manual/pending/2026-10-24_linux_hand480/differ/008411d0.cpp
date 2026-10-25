// Grade: DIFFER(510v510) -- frame slot only: the materialised Open()==LH_OK && PrepareRequest()==8 bool lands at [esp+0xa] (ours) vs [esp+0xb] (retail, sharing the allocator temporary's byte); every other byte matches. Tried: if-condition, named bool (local and function-top), ternary, inline Redirect helper, explicit string(""), PrepareRequest default args, allocator with/without dtor, early-return layout (1 grade)
// LHNetworkLib::LHHttp2::HelperGetDocument(bool redirected) -- blocking GET driver: pump SendRequestAsync until the request is out, pump ReceiveHeaderAsync until the header is in, read the status; 200 pumps ReceiveDocumentAsync to completion, a 3xx with a Location reconnects to the new host/URI and recurses once per hop
// Leaf: ?HelperGetDocument@LHHttp2@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008411D0, 510 bytes + 2 int3, thiscall `ret 4`, LH_HTTP_STATUS in eax.
// Identity: Ego_r.pdb ?HelperGetDocument@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@_N@Z (debug build 0x0084FD20, after
//   GetHeader(LHHttpHeaderStatus&)); its only caller is itself (0x008413B4, `push 1`). The catalog's
//   "LHHttp2_HandleResponse" is descriptive.
// Callees: LHHttpHeaderStatus ctor 0x00569C50 / dtor 0x00569D30 / Reset 0x0083FB60 / ParseLocationData 0x0083F420 /
//   GetLocationHost 0x0083F5C0 / GetLocationURI 0x0083F600 (GetLocationPort 0x0083F640 inline), LHHttp2 SendRequestAsync
//   0x008405F0 / ReceiveHeaderAsync 0x00840740 / GetHeader(LHHttpHeaderStatus&) 0x00840B60 / ReceiveDocumentAsync
//   0x0083EFE0 / Close 0x00840070 / Reset 0x0083FF80 / Open(const string&, unsigned short) 0x0083FB20 (landed as
//   SetConnectionDetails: stores host + port, then Open()) / PrepareRequest 0x008400B0, STLport operator!=(string,
//   const char*) 0x00566EA0 (cdecl), string(const char*, const allocator&) 0x00568440, free 0x00BFEA14.
// Strings: "" 0x0122D70E.
// Layout: LHHttp2 CurrForwarding +0x6C; LHHttpHeaderStatus (retail STLport strings, 0x48 bytes) hsServerCode +0,
//   hsLocation +0x1C, hsPort +0x44 (the landed ctor 0x00569C50).
// Shape: three `do ... while (ret != a && ret != b && ret != c)` pumps (VC7.1 clones the epilogue onto each loop's
//   fall-through exit); the redirect range test is unsigned (`jb`/`jae`: an enum compares signed under VC7.1, so the
//   code is read through `unsigned long`); the Open && PrepareRequest condition owns four conditional temporaries
//   (host string 1, allocator 2 -- STLport's user-declared `~allocator() {}` -- the "" string 4, URI string 8) whose
//   flags live in ebx and whose STLport destructors (`if (p) free(p)`) run inline before the materialised bool is tested.
extern "C" void __cdecl free(void* block);                                             // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    ~allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                                                     // +0x00
    _Tp* _M_finish;                                                                    // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;                            // +0x08
    void _M_deallocate_block() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    ~_String_base() { _M_deallocate_block(); }
};

template <class _CharT> class char_traits { };

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : protected _String_base<_CharT, _Alloc> {
public:
    typedef _Alloc allocator_type;
    basic_string(const _CharT* __s, const allocator_type& __a = allocator_type());   // 0x00568440
    ~basic_string() { }
};

template <class _CharT, class _Traits, class _Alloc>
bool __cdecl operator!=(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __c);   // 0x00566EA0

}

typedef std::basic_string<char> string;

namespace LHNetworkLib {

enum LH_RETURN { LH_OK = 0 };
enum HTTP_REQUEST_TYPE { HTTP_GET = 0 };
enum LH_HTTP_SERVER_CODES { LH_HTTP_SERVER_CODE_NONE = -1, LH_HTTP_SERVER_CODE_OK = 200 };
enum LH_HTTP_STATUS {
    LH_HTTP_NO_REQUEST = -104,
    LH_HTTP_TIMED_OUT = -102,
    LH_HTTP_RECEIVE_FAILED = -101,
    LH_HTTP_SEND_FAILED = -100,
    LH_HTTP_REQUEST_SENT = 2,
    LH_HTTP_HEADER_RECEIVED = 5,
    LH_HTTP_REQUEST_PREPARED = 8,
    LH_HTTP_DOCUMENT_RECEIVED = 7
};

struct LHHttpHeaders {                                   // retail 24 bytes
    void*  HdrDocumentHeader[3];                         // +0x00 vector<string>
    string HeaderString;                                 // +0x0c
};

struct LHHttpHeaderStatus {                              // retail 0x48 bytes (STLport strings)
    LH_HTTP_SERVER_CODES hsServerCode;                   // +0x00
    LHHttpHeaders        hsDocumentHeader;               // +0x04
    string               hsLocation;                     // +0x1c
    long                 hsDocumentSize;                 // +0x28
    string               hsHost;                         // +0x2c
    string               hsURI;                          // +0x38
    unsigned short       hsPort;                         // +0x44
    LHHttpHeaderStatus();                                // 0x00569C50
    ~LHHttpHeaderStatus();                               // 0x00569D30
    void Reset();                                        // 0x0083FB60
    void ParseLocationData();                            // 0x0083F420
    string GetLocationHost();                            // 0x0083F5C0
    string GetLocationURI();                             // 0x0083F600
    unsigned short GetLocationPort()                     // 0x0083F640 (inline here)
    {
        ParseLocationData();
        return hsPort;
    }
};

struct LHHttp2 {
    unsigned char       _pad000[0x6c];
    unsigned long       CurrForwarding;                  // +0x06c
    LH_RETURN Close();                                   // 0x00840070
    void Reset();                                        // 0x0083FF80
    LH_RETURN Open(const string& host, unsigned short port);   // 0x0083FB20
    LH_HTTP_STATUS PrepareRequest(HTTP_REQUEST_TYPE type, const string& uri, LHHttpHeaders* headers,
                                  const string& data, unsigned long dataLength);   // 0x008400B0
    LH_HTTP_STATUS SendRequestAsync();                   // 0x008405F0
    LH_HTTP_STATUS ReceiveHeaderAsync();                 // 0x00840740
    LH_HTTP_STATUS GetHeader(LHHttpHeaderStatus& status);   // 0x00840B60
    LH_HTTP_STATUS ReceiveDocumentAsync();               // 0x0083EFE0
    LH_HTTP_STATUS HelperGetDocument(bool redirected);
};

LH_HTTP_STATUS LHHttp2::HelperGetDocument(bool redirected)
{
    LH_HTTP_STATUS ret;
    LHHttpHeaderStatus status;

    if (!redirected)
        CurrForwarding = 0;

    do {
        ret = SendRequestAsync();
    } while (ret != LH_HTTP_NO_REQUEST && ret != LH_HTTP_REQUEST_SENT && ret != LH_HTTP_SEND_FAILED);

    if (ret == LH_HTTP_REQUEST_SENT) {
        do {
            ret = ReceiveHeaderAsync();
        } while (ret != LH_HTTP_HEADER_RECEIVED && ret != LH_HTTP_RECEIVE_FAILED && ret != LH_HTTP_TIMED_OUT);
        if (ret == LH_HTTP_HEADER_RECEIVED) {
            GetHeader(status);
            if (status.hsServerCode == LH_HTTP_SERVER_CODE_OK) {
                do {
                    ret = ReceiveDocumentAsync();
                } while (ret != LH_HTTP_DOCUMENT_RECEIVED && ret != LH_HTTP_RECEIVE_FAILED && ret != LH_HTTP_TIMED_OUT);
            }
            else if ((unsigned long)status.hsServerCode >= 300 && (unsigned long)status.hsServerCode < 400 && status.hsLocation != "") {
                Close();
                Reset();
                if (Open(status.GetLocationHost(), status.GetLocationPort()) == LH_OK
                    && PrepareRequest(HTTP_GET, status.GetLocationURI(), 0, "", 0) == LH_HTTP_REQUEST_PREPARED) {
                    status.Reset();
                    ret = HelperGetDocument(true);
                }
            }
        }
    }
    return ret;
}

}
