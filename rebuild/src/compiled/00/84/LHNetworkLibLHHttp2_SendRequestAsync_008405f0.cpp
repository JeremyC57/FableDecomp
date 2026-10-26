// LHNetworkLib::LHHttp2::SendRequestAsync() -- push the next block (at most 1 KiB) of the prepared request down the socket: -104 with no request, 2 (and a reset for the response) once everything is sent, -100 when the send fails, -102 when nothing went out for longer than the time-out, else 1
// Leaf: ?SendRequestAsync@LHHttp2@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008405F0, 322 bytes (+ int3 padding), thiscall `ret`.
// Identity: Ego_r.pdb ?SendRequestAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ; callers 0x0055FBA8 (the online
//   scoreboard code) and LHHttp2::HandleResponse-side 0x00841202 (0x008411D0). The catalog's "LHHttp2_SendRequest" is
//   descriptive.
// Callees: basic_string<char>::assign(const char*, const char*) 0x005693B0, vector<basic_string<char> >::erase 0x00841FC0,
//   LHHttp2::SendRawData 0x0083F8B0 (landed), LHHttp2::ResetRequest 0x0083F980 (landed; inline at its first use, called
//   at the other two), time 0x00BFED22.
// Layout: retail LHHttp2 (the landed ResetRequest 0x0083F980 / ~LHHttp2 0x008410D0 models): PreparedRequest +0x14,
//   RequestBytesSent +0x20, RequestLength +0x24, ReceivedHeaders +0x40, TimeOutValue +0x58, ActionStartTime +0x5c,
//   ActionStarted +0x60, CurrentAction +0x62, CurrentHeaderPart +0x70 (2048), CurrentHeaderPos +0x870,
//   DocumentEstimatedLength +0x894, DocumentLength +0x898.
// Shape: `PreparedRequest == ""` is STLport's size test then the zero-length memcmp (`repe cmpsb`); the finished branch
//   expands ResetRequest and ReceivedHeaders.Reset() (HeaderString = "", the vector's clear through erase); the failed
//   send also clears CurrentHeaderPart (memset intrinsic, `rep stosd`); the idle test is `time(0) > start + time-out`
//   and an active send restamps ActionStartTime through time(&ActionStartTime). SendRawData's body is in the TU
//   (noinline): knowing it only writes through bytesSent keeps the count in esi across time() (an opaque callee
//   forces reloads); bytesSent is declared after the 1 KiB clamp (its zero store lands after the pushes), and the
//   block start is &PreparedRequest[sent] (STLport operator[]; `c_str() + sent` swaps the add's registers).
#include <string.h>
#pragma intrinsic(memcmp, memset, strlen)
extern "C" long __cdecl time(long* timer);                       // 0x00BFED22

#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

class basic_string {                               // STLport basic_string<char>, 12 bytes
public:
    char* _M_start;
    char* _M_finish;
    _STLP_alloc_proxy<char*, char, allocator<char> > _M_end_of_storage;
    ~basic_string() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { }                                  // char: _Destroy is trivial
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void erase(iterator __first, iterator __last);
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);                          // 0x004110D0
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

}


struct CRetailString {                                           // STLport basic_string<char>, 12 bytes in retail
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const char* c_str() const { return _M_start; }
    char& operator[](unsigned int n) { return *(_M_start + n); }
    CRetailString& Assign(const char* first, const char* last);  // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
    bool operator==(const char* text) const
    {
        unsigned int length = strlen(text);
        return size() == length && memcmp(_M_start, text, length) == 0;
    }
};

struct CRetailStringVector {                                     // STLport vector<basic_string<char> >, 12 bytes
    CRetailString* _M_start;
    CRetailString* _M_finish;
    CRetailString* _M_end_of_storage;
    CRetailString* begin() { return _M_start; }
    CRetailString* end() { return _M_finish; }
    CRetailString* erase(CRetailString* first, CRetailString* last);   // 0x00841FC0
    void clear() { erase(begin(), end()); }
};

namespace LHNetworkLib {

struct LAddress {                                  // retail 0x1C bytes (landed 0x00841E90 / 0x00841F60)
    std::basic_string ServerName;                  // +0x00
    unsigned long     ServerIP;                    // +0x0c
    unsigned short    ServerPort;                  // +0x10
    unsigned long     TransportProtocol;           // +0x14
    unsigned long     TransportFlags;              // +0x18
    LAddress();                                    // 0x00841E90
};

class LSocket {                                    // retail offsets (Ego_r.pdb names)
public:
    unsigned char _pad_0x00[0x90];
    unsigned long NrLastDataSent;                  // +0x90
    bool Send(std::vector<char>& data, const LAddress& to, const LAddress& from);              // 0x008F8F50
};

enum LH_RETURN { LH_RETURN_ERROR = 2 };
enum LH_HTTP_STATUS {
    LH_HTTP_NO_REQUEST = -104,
    LH_HTTP_TIMED_OUT = -102,
    LH_HTTP_SEND_FAILED = -100,
    LH_HTTP_SENDING = 1,
    LH_HTTP_REQUEST_SENT = 2
};

class LHHttpHeaders { public:                                           // 24 bytes in retail
    CRetailStringVector HdrDocumentHeader;                       // +0x00
    CRetailString       HeaderString;                            // +0x0c
    void Reset()                                                 // 0x0083FBC0 (landed)
    {
        HeaderString = "";
        HdrDocumentHeader.clear();
    }
};

class LHHttp2 { public:
    LSocket*            Host;                                    // +0x000
    CRetailString       Hostname;                                // +0x004
    unsigned short      Port;                                    // +0x010
    CRetailString       PreparedRequest;                         // +0x014
    unsigned long       RequestBytesSent;                        // +0x020
    unsigned long       RequestLength;                           // +0x024
    LHHttpHeaders       MyHeaders;                               // +0x028
    LHHttpHeaders       ReceivedHeaders;                         // +0x040
    unsigned long       TimeOutValue;                            // +0x058
    unsigned long       ActionStartTime;                         // +0x05c
    bool                ActionStarted;                           // +0x060
    unsigned short      CurrentAction;                           // +0x062
    unsigned long       CurrDownSize;                            // +0x064
    unsigned long       Forwardings;                             // +0x068
    unsigned long       CurrForwarding;                          // +0x06c
    char                CurrentHeaderPart[2048];                 // +0x070
    unsigned long       CurrentHeaderPos;                        // +0x870
    CRetailString       ProxyHost;                               // +0x874
    unsigned long       ProxyPort;                               // +0x880
    unsigned char       _pad884[0x894 - 0x884];                  // +0x884 DocumentParts, DocumentEncoding
    unsigned long       DocumentEstimatedLength;                 // +0x894
    unsigned long       DocumentLength;                          // +0x898
    LH_RETURN SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool wait);   // 0x0083F8B0
    void ResetRequest();                                         // 0x0083F980
    LH_HTTP_STATUS SendRequestAsync();
};

#pragma auto_inline(off)
LH_RETURN LHHttp2::SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool wait)   // 0x0083F8B0 (landed)
{
    if (!Host)
        return (LH_RETURN)3;
    std::vector<char> buffer;
    buffer.resize(size);
    memcpy(&buffer[0], data, size);
    Host->Send(buffer, LAddress(), LAddress());
    *bytesSent = Host->NrLastDataSent;
    return (LH_RETURN)0;
}
#pragma auto_inline(on)

void LHHttp2::ResetRequest()
{
    PreparedRequest = "";
    RequestBytesSent = 0;
    RequestLength = 0;
    DocumentEstimatedLength = 0;
    DocumentLength = 0;
}

LH_HTTP_STATUS LHHttp2::SendRequestAsync()
{
    if (PreparedRequest == "")
        return LH_HTTP_NO_REQUEST;
    if (RequestBytesSent >= RequestLength) {
        ResetRequest();
        ReceivedHeaders.Reset();
        ActionStarted = false;
        CurrentAction = 1;
        return LH_HTTP_REQUEST_SENT;
    }
    unsigned long size = RequestLength - RequestBytesSent;
    if (size >= 1024)
        size = 1024;
    unsigned long bytesSent = 0;
    if (SendRawData(&PreparedRequest[RequestBytesSent], size, &bytesSent, true) == LH_RETURN_ERROR) {
        ResetRequest();
        memset(CurrentHeaderPart, 0, sizeof(CurrentHeaderPart));
        CurrentHeaderPos = 0;
        return LH_HTTP_SEND_FAILED;
    }
    if (bytesSent == 0 && ActionStarted) {
        if ((unsigned long)time(0) > ActionStartTime + TimeOutValue) {
            ResetRequest();
            return LH_HTTP_TIMED_OUT;
        }
    } else {
        time((long*)&ActionStartTime);
        ActionStarted = true;
    }
    RequestBytesSent += bytesSent;
    return LH_HTTP_SENDING;
}

}