// LHNetworkLib::LHHttpHeaderStatus::Reset() -- forgets a parsed response header: server code -1, no header lines, empty location / host / URI strings, port 0
// Leaf: ?Reset@LHHttpHeaderStatus@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FB60, 88 bytes, __fastcall, plain `ret`.
// Identity: Ego_r.pdb ?Reset@LHHttpHeaderStatus@LHNetworkLib@@QAEXXZ; TLC keeps it next to LHHttpHeaders::Reset (landed
//   0x0083FBC0). The manifest label "CPatchTesselationEdgeStrip_Reset" is propagated and wrong.
// Callees: vector<basic_string<char> >::erase(iterator, iterator) 0x00841FC0 and basic_string<char>::assign(const char*,
//   const char*) 0x005693B0 with the empty literal 0x0122D70E as both bounds (the landed 0x0083FBC0 models).
// Layout: retail LHHttpHeaderStatus (Ego_r.pdb names, 12-byte STLport containers): hsServerCode +0, hsDocumentHeader +4
//   (LHHttpHeaders: HdrDocumentHeader vector +4, HeaderString +0x10), hsLocation +0x1c, hsDocumentSize +0x28, hsHost +0x2c,
//   hsURI +0x38, hsPort +0x44.
// Shape: only the header-line vector is cleared (the header string is left alone), hsDocumentSize is not touched.
#include <string.h>

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
enum LH_HTTP_SERVER_CODES { };

class LHHttpHeaders { public:
    CRetailStringVector HdrDocumentHeader;         // +0x00
    CRetailString       HeaderString;              // +0x0c
};

struct LHHttpHeaderStatus {
    LH_HTTP_SERVER_CODES hsServerCode;             // +0x00
    LHHttpHeaders        hsDocumentHeader;         // +0x04
    CRetailString        hsLocation;               // +0x1c
    long                 hsDocumentSize;           // +0x28
    CRetailString        hsHost;                   // +0x2c
    CRetailString        hsURI;                    // +0x38
    unsigned short       hsPort;                   // +0x44
    void Reset();
};

void LHHttpHeaderStatus::Reset()
{
    hsServerCode = (LH_HTTP_SERVER_CODES)-1;
    hsDocumentHeader.HdrDocumentHeader.clear();
    hsLocation = "";
    hsHost = "";
    hsURI = "";
    hsPort = 0;
}
}