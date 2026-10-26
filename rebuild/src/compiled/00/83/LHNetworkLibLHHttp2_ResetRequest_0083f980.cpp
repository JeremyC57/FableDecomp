// LHNetworkLib::LHHttp2::ResetRequest() -- empties the prepared request string and clears the sent/request byte counts and the document lengths
// Leaf: ResetRequest
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083F980, 43 bytes (+ int3 padding). Ego_r.pdb ?ResetRequest@LHHttp2@LHNetworkLib@@QAEXXZ.
// Callee: STLport basic_string<char>::assign(const char* first, const char* last) @ 0x005693B0 (manifest "Vector_Insert" is
// propagated; the landed CNamedRecord 0x00841E90 -- the LAddress constructor -- calls it the same way), with the empty literal
// @ 0x0122D70E pushed as both bounds (strlen("") folded to 0).
// Layout: retail LHHttp2 offsets (engine/LHHttp2.h is the PDB layout and does not fit retail; see GetDocumentSize 0x0083EA70):
// PreparedRequest +0x14, RequestBytesSent +0x20, RequestLength +0x24, DocumentEstimatedLength +0x894, DocumentLength +0x898.
// Shape: operator=(const char*) inline over the out-of-line range assign, then four zeros from one register.
#include <string.h>
struct LSocket;                                    // global, as the landed LSocket rows bind it

struct CRetailString {                             // STLport basic_string<char>, 12 bytes in retail (global, the landed 0x00841E90 name)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

namespace LHNetworkLib {


struct LHHttpDocumentParts {                       // Ego_r.pdb, 12 bytes (destructor 0x0083EA10)
    char*         Buffer;                          // +0x00
    unsigned long BufferLength;                    // +0x04
    unsigned long CurrBufPos;                      // +0x08
};

struct LDocumentPartVector {                       // STLport vector<LHHttpDocumentParts*>, 12 bytes in retail
    LHHttpDocumentParts** _M_start;
    LHHttpDocumentParts** _M_finish;
    LHHttpDocumentParts** _M_end_of_storage;
    LHHttpDocumentParts** begin() { return _M_start; }
    LHHttpDocumentParts** end() { return _M_finish; }
};

class LHHttp2 { public:                                   // retail offsets (Ego_r.pdb names; its 0x1C-byte strings shift every field)
    LSocket*            Host;                      // +0x000
    CRetailString       Hostname;                  // +0x004
    unsigned short      Port;                      // +0x010
    CRetailString       PreparedRequest;           // +0x014
    unsigned long       RequestBytesSent;          // +0x020
    unsigned long       RequestLength;             // +0x024
    unsigned char       _pad028[0x48];             // +0x028 MyHeaders, ReceivedHeaders, TimeOutValue .. CurrForwarding
    char                CurrentHeaderPart[2048];   // +0x070
    unsigned long       CurrentHeaderPos;          // +0x870
    CRetailString       ProxyHost;                 // +0x874
    unsigned long       ProxyPort;                 // +0x880
    LDocumentPartVector DocumentParts;             // +0x884
    unsigned long       DocumentEncoding;          // +0x890
    unsigned long       DocumentEstimatedLength;   // +0x894
    unsigned long       DocumentLength;            // +0x898
    void ResetRequest();
};

void LHHttp2::ResetRequest()
{
    PreparedRequest = "";
    RequestBytesSent = 0;
    RequestLength = 0;
    DocumentEstimatedLength = 0;
    DocumentLength = 0;
}

}