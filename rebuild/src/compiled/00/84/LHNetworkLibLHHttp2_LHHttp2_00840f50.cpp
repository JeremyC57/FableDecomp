// LHNetworkLib::LHHttp2::LHHttp2() -- the HTTP client's constructor: the five STLport strings (Hostname, PreparedRequest, both header strings, ProxyHost) built empty in their 8-byte blocks and the three vectors nulled, then the body clears the socket, re-assigns the request / hostname / proxy strings to "", sets the 60 s time-out and 10 forwardings, and zeroes the transfer state and the document buffer
// Leaf: ??0LHHttp2@LHNetworkLib@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00840F50, 374 bytes (padded to 384), `__fastcall`, returns `this`, plain `ret`; no EH frame
//   (the network library TU, like the landed LAddress ctor 0x00841E90, has no unwind state for its member strings).
// Callees: malloc @ 0x00BFEA0E (each string's 8-byte initial block), basic_string<char>::assign(const char*, const char*)
//   @ 0x005693B0 with the empty literal 0x0122D70E as both bounds.
// Layout: the landed LHHttp2::Reset 0x0083FF80 (Ego_r.pdb names, retail offsets): Host +0, Hostname +4, Port +0x10,
//   PreparedRequest +0x14, MyHeaders +0x28 / ReceivedHeaders +0x40 (vector<string> + HeaderString), TimeOutValue +0x58, ...,
//   ProxyHost +0x874, ProxyPort +0x880, DocumentParts +0x884, ..., CurrentDocumentChunk +0x10a4.
// Shape: the landed LAddress string model (_String_base nulls start / finish / the _STLP_alloc_proxy end, then allocates and
//   terminates); vectors as STLport _Vector_base with the proxy end member; the body in the order the stores sit between
//   the assign calls (Host before the PreparedRequest assign, Port before the Hostname assign, ProxyPort after the ProxyHost
//   assign); one zero register (ebx), the CurrentDocumentPart memset as `rep stosd`.
#include <string.h>
#pragma intrinsic(memset)
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T m_data;
    SRetailAllocProxy(T data) : m_data(data) { }
};

struct CRetailStringBase {                      // STLport _String_base
    char* m_start;
    char* m_finish;
    SRetailAllocProxy<char*> m_endOfStorage;
    CRetailStringBase(size_t n) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        m_start = static_cast<char*>(malloc(n));
        m_finish = m_start;
        m_endOfStorage.m_data = m_start + n;
    }
};

struct CRetailString : CRetailStringBase {      // STLport basic_string<char>, 12 bytes in retail
    CRetailString() : CRetailStringBase(8)
    {
        *m_finish = 0;
    }
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

template <class T> struct CRetailVector {       // STLport vector<T>: _Vector_base, 12 bytes in retail
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T*> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct LSocket;

namespace LHNetworkLib {

class LHHttpHeaders { public:
    CRetailVector<CRetailString> HdrDocumentHeader;   // +0x00
    CRetailString                HeaderString;        // +0x0c
};

struct LHHttpDocumentParts;

class LHHttp2 { public:                                   // retail offsets (Ego_r.pdb names)
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
    CRetailVector<LHHttpDocumentParts*> DocumentParts;   // +0x884
    unsigned long       DocumentEncoding;          // +0x890
    unsigned long       DocumentEstimatedLength;   // +0x894
    unsigned long       DocumentLength;            // +0x898
    char                CurrentDocumentPart[2048]; // +0x89c
    unsigned long       CurrentDocumentPos;        // +0x109c
    bool                InChunkMode;               // +0x10a0
    LHHttpDocumentParts* CurrentDocumentChunk;     // +0x10a4
    LHHttp2();
};

LHHttp2::LHHttp2()
{
    Host = 0;
    PreparedRequest = "";
    Port = 0;
    Hostname = "";
    TimeOutValue = 60;
    ActionStartTime = 0;
    CurrentAction = 0;
    CurrentHeaderPos = 0;
    CurrDownSize = 0;
    ActionStarted = false;
    DocumentLength = 0;
    Forwardings = 10;
    CurrForwarding = 0;
    DocumentEstimatedLength = 0;
    memset(CurrentDocumentPart, 0, sizeof(CurrentDocumentPart));
    CurrentDocumentPos = 0;
    InChunkMode = false;
    CurrentDocumentChunk = 0;
    ProxyHost = "";
    ProxyPort = 0;
}

}