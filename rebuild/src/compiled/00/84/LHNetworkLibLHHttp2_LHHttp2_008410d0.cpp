// LHNetworkLib::LHHttp2::~LHHttp2() -- close and delete the host socket, reset the connection state (twice: Close's own Reset, then the destructor's), then destroy the members: document-part vector, proxy host, both header sets, prepared request, host name
// Leaf: ??1LHHttp2@LHNetworkLib
// Retail: Fable.exe (TLC, VC7.1) @ 0x008410D0, 250 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb ??1LHHttp2@LHNetworkLib@@QAE@XZ (the debug build's 0x0084EE40, right after the constructor; TLC's
//   constructor is the landed 0x00840F50 just before this body). 15 call sites in the online-scoreboard code
//   (0x0055F746 ..). The catalog's "CNetworkConnection_Cleanup" is descriptive.
// Callees: LSocket::Close 0x008F8C70, LSocket::~LSocket 0x008F9780, operator delete 0x00BFE9BC, LHHttp2::Reset
//   0x0083FF80 (twice), free 0x00BFEA14, basic_string<char>::assign(const char*, const char*) 0x005693B0,
//   vector<basic_string<char> >::erase(iterator, iterator) 0x00841FC0, vector<basic_string<char> >::~vector 0x00569940.
// Layout: retail LHHttp2 with 12-byte STLport containers (the landed Close 0x00840070 model): Host +0, Hostname +4,
//   PreparedRequest +0x14, MyHeaders +0x28, ReceivedHeaders +0x40, ProxyHost +0x874, DocumentParts +0x884;
//   LHHttpHeaders = HdrDocumentHeader vector +0, HeaderString +0xc (the landed LHHttpHeaders::Reset 0x0083FBC0).
// Shape: `{ Close(); Reset(); }` with Close inline (the socket block, then its own Reset call); the members' implicit
//   destructors in reverse order: ~vector<LHHttpDocumentParts*> (`if (start) free(start)`), the strings' block frees,
//   and ~LHHttpHeaders() { Reset(); } with that Reset inline (HeaderString = "" through assign(first, last), the
//   vector's clear through erase), then its string free and the out-of-line ~vector<string>.
#include <string.h>
#pragma intrinsic(strlen)

extern "C" void __cdecl free(void* block);                       // 0x00BFEA14

struct LSocket {
    bool Close();                                                // 0x008F8C70
    ~LSocket();                                                  // 0x008F9780
};

struct CRetailString {                                           // STLport basic_string<char>, 12 bytes in retail
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    ~CRetailString() { if (_M_start != 0) free(_M_start); }
    CRetailString& Assign(const char* first, const char* last);  // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

struct CRetailStringVector {                                     // STLport vector<basic_string<char> >, 12 bytes
    CRetailString* _M_start;
    CRetailString* _M_finish;
    CRetailString* _M_end_of_storage;
    ~CRetailStringVector();                                      // 0x00569940
    CRetailString* begin() { return _M_start; }
    CRetailString* end() { return _M_finish; }
    CRetailString* erase(CRetailString* first, CRetailString* last);   // 0x00841FC0
    void clear() { erase(begin(), end()); }
};

namespace LHNetworkLib {

struct LHHttpDocumentParts;

struct LDocumentPartVector {                                     // STLport vector<LHHttpDocumentParts*>, 12 bytes
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
    ~LDocumentPartVector() { if (_M_start != 0) free(_M_start); }
};

struct LHHttpHeaders {                                           // 24 bytes in retail
    CRetailStringVector HdrDocumentHeader;                       // +0x00
    CRetailString       HeaderString;                            // +0x0c
    ~LHHttpHeaders() { Reset(); }
    void Reset()                                                 // 0x0083FBC0 (landed)
    {
        HeaderString = "";
        HdrDocumentHeader.clear();
    }
};

struct LHHttp2 {
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
    LDocumentPartVector DocumentParts;                           // +0x884
    unsigned long       DocumentEncoding;                        // +0x890
    unsigned long       DocumentEstimatedLength;                 // +0x894
    unsigned long       DocumentLength;                          // +0x898
    ~LHHttp2();
    void Reset();                                                // 0x0083FF80
    long Close()                                                 // 0x00840070 (landed)
    {
        if (Host) {
            Host->Close();
            delete Host;
            Host = 0;
        }
        Reset();
        return 0;
    }
};

LHHttp2::~LHHttp2()
{
    Close();
    Reset();
}

}