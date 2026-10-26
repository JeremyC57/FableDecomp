// LHNetworkLib::LHHttpHeaders::Reset() -- empties the prepared header string and erases every stored document header line
// Leaf: ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083FBC0, 37 bytes, __fastcall, plain `ret`. Ego_r.pdb ?Reset@LHHttpHeaders@LHNetworkLib@@QAEXXZ.
// Callees: STLport basic_string<char>::assign(const char* first, const char* last) 0x005693B0 (the empty literal 0x0122D70E
//   as both bounds, as the landed LHHttp2::ResetRequest 0x0083F980); vector<basic_string<char> >::erase(iterator first,
//   iterator last) 0x00841FC0 (manifest "std::vector_move_and_cleanup": moves the tail down, frees the dead strings).
// Callers: 0x0055FB45, 0x00569C33, 0x00569D6D, 0x008409DD.
// Layout: retail LHHttpHeaders (Ego_r.pdb names, 12-byte STLport containers): HdrDocumentHeader vector +0, HeaderString +0xc.
// Shape: HeaderString = "" first, then the inline clear() as erase(begin(), end()). The manifest label
//   "SetStaticCast<NUISystem::CComponent>" is propagated and wrong.
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

class LHHttpHeaders { public:
    CRetailStringVector HdrDocumentHeader;         // +0x00
    CRetailString       HeaderString;              // +0x0c
    void Reset();
};

void LHHttpHeaders::Reset()
{
    HeaderString = "";
    HdrDocumentHeader.clear();
}

}