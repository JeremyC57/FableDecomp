// LHNetworkLib::LHHttp2::~LHHttp2() -- close and delete the host socket, reset the connection state (twice: Close's own Reset, then the destructor's), then destroy the members: document-part vector, proxy host, both header sets, prepared request, host name
// Leaf: ??1LHHttp2@LHNetworkLib@@QAE@XZ
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
// NF123 (2026-11-20, q4193): the local LSocket / CRetailString / CRetailStringVector / LDocumentPartVector / LHHttpHeaders / LHHttp2
//   models replaced by the shared engine/LHHttp2.h (retail branch, STLport std::string / std::vector) and engine/LSocket.h, with the
//   NF123 opt-ins LHHTTP2_DTOR (~LHHttp2, Close, the inline ~LHHttpHeaders), LHHTTP2_RESET and LSOCKET_DTOR. Close's body is
//   written here inline, as the LHHttp TU saw it (its out-of-line copy is the landed 0x00840070). Respelled bindings: erase
//   0x00841FC0, assign 0x005693B0, ~vector<string> 0x00569940 (were the CRetailString* stand-ins), LSocket::Close 0x008F8C70 and
//   ~LSocket 0x008F9780 (were the global LSocket; both PDBs: LHNetworkLib).
#include <string.h>
#pragma intrinsic(strlen)
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_DTOR
#define FABLE_RETAIL_LHHTTP2_RESET
#define FABLE_RETAIL_LSOCKET_DTOR
#define FABLE_RETAIL_LSOCKET_SEND_PASS23
#include "rebuild_abi.h"
#include "engine/LHHttp2.h"
#include "engine/LSocket.h"

namespace LHNetworkLib {

inline LH_RETURN LHHttp2::Close()
{
    if (Host) {
        Host->Close();
        delete Host;
        Host = 0;
    }
    Reset();
    return LH_OK;
}

LHHttp2::~LHHttp2()
{
    Close();
    Reset();
}

}
