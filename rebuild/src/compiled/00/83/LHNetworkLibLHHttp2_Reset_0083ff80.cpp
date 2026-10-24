// LHNetworkLib::LHHttp2::Reset() -- returns the HTTP client to its initial state: destroys the socket, empties the request / header / host / proxy strings, drops the document parts and zeroes the transfer state
// Leaf: ?Reset@LHHttp2@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FF80, 235 bytes, __fastcall, plain `ret`.
// Identity: Ego_r.pdb ?Reset@LHHttp2@LHNetworkLib@@QAEXXZ, between Open(const string&, unsigned short) and Close() as in TLC
//   (Close landed at 0x00840070). The manifest label "LHHttp2_Clear" is descriptive.
// Callees: LSocket::~LSocket 0x008F9780 + operator delete 0x00BFE9BC; basic_string<char>::assign(const char*, const char*)
//   0x005693B0 with the empty literal 0x0122D70E as both bounds; vector<basic_string<char> >::erase 0x00841FC0 (the inline
//   LHHttpHeaders::Reset, landed out of line at 0x0083FBC0); LHHttp2::ResetDocumentParts 0x0083EB00 (landed).
// Layout: retail LHHttp2 (landed ResetRequest 0x0083F980 / ResetDocumentRecv 0x0083EB90; Ego_r.pdb names): Host +0,
//   Hostname +4, Port +0x10, PreparedRequest +0x14, RequestBytesSent +0x20, RequestLength +0x24, MyHeaders +0x28,
//   ReceivedHeaders +0x40, TimeOutValue +0x58, ActionStartTime +0x5c, ActionStarted +0x60, CurrentAction +0x62,
//   CurrDownSize +0x64, Forwardings +0x68, CurrForwarding +0x6c, CurrentHeaderPart +0x70, CurrentHeaderPos +0x870,
//   ProxyHost +0x874, ProxyPort +0x880, DocumentParts +0x884, DocumentEncoding +0x890, DocumentEstimatedLength +0x894,
//   DocumentLength +0x898, CurrentDocumentPart +0x89c, CurrentDocumentPos +0x109c, InChunkMode +0x10a0.
// Shape: `if (Host) { delete Host; Host = 0; }` (the null store inside the test); RequestBytesSent is zeroed before the
//   Hostname assignment (its store sits before that call); one zero register (ebx) for every member store, the
//   CurrentDocumentPart memset as `rep stosd`.
#include <string.h>
#pragma intrinsic(memset)

struct LSocket {                                   // global, as the landed LSocket rows bind it
    ~LSocket();                                    // 0x008F9780
};

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

struct CRetailStringVector {                       // STLport vector<basic_string<char> >, 12 bytes in retail
    CRetailString* _M_start;
    CRetailString* _M_finish;
    CRetailString* _M_end_of_storage;
    CRetailString* begin() { return _M_start; }
    CRetailString* end() { return _M_finish; }
    CRetailString* erase(CRetailString* first, CRetailString* last);   // 0x00841FC0
    void clear() { erase(begin(), end()); }
};

namespace LHNetworkLib {

struct LHHttpHeaders {
    CRetailStringVector HdrDocumentHeader;         // +0x00
    CRetailString       HeaderString;              // +0x0c
    void Reset()                                   // landed out of line at 0x0083FBC0
    {
        HeaderString = "";
        HdrDocumentHeader.clear();
    }
};

struct LHHttpDocumentParts;
struct LDocumentPartVector {                       // STLport vector<LHHttpDocumentParts*>, 12 bytes in retail
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
};

struct LHHttp2 {                                   // retail offsets (Ego_r.pdb names)
    LSocket*            Host;                      // +0x000
    CRetailString       Hostname;                  // +0x004
    unsigned short      Port;                      // +0x010
    CRetailString       PreparedRequest;           // +0x014
    unsigned long       RequestBytesSent;          // +0x020
    unsigned long       RequestLength;             // +0x024
    LHHttpHeaders       MyHeaders;                 // +0x028
    LHHttpHeaders       ReceivedHeaders;           // +0x040
    unsigned long       TimeOutValue;              // +0x058
    unsigned long       ActionStartTime;           // +0x05c
    bool                ActionStarted;             // +0x060
    unsigned short      CurrentAction;             // +0x062
    unsigned long       CurrDownSize;              // +0x064
    unsigned long       Forwardings;               // +0x068
    unsigned long       CurrForwarding;            // +0x06c
    char                CurrentHeaderPart[2048];   // +0x070
    unsigned long       CurrentHeaderPos;          // +0x870
    CRetailString       ProxyHost;                 // +0x874
    unsigned long       ProxyPort;                 // +0x880
    LDocumentPartVector DocumentParts;             // +0x884
    unsigned long       DocumentEncoding;          // +0x890
    unsigned long       DocumentEstimatedLength;   // +0x894
    unsigned long       DocumentLength;            // +0x898
    char                CurrentDocumentPart[2048]; // +0x89c
    unsigned long       CurrentDocumentPos;        // +0x109c
    bool                InChunkMode;               // +0x10a0
    LHHttpDocumentParts* CurrentDocumentChunk;     // +0x10a4
    void ResetDocumentParts();                     // 0x0083EB00
    void Reset();
};

void LHHttp2::Reset()
{
    if (Host) {
        delete Host;
        Host = 0;
    }
    PreparedRequest = "";
    MyHeaders.Reset();
    ReceivedHeaders.Reset();
    ResetDocumentParts();
    RequestBytesSent = 0;
    Hostname = "";
    ActionStartTime = 0;
    CurrDownSize = 0;
    CurrentAction = 0;
    ActionStarted = false;
    CurrentHeaderPos = 0;
    DocumentLength = 0;
    DocumentEstimatedLength = 0;
    memset(CurrentDocumentPart, 0, sizeof(CurrentDocumentPart));
    CurrentDocumentPos = 0;
    InChunkMode = false;
    ProxyHost = "";
    ProxyPort = 0;
}

}