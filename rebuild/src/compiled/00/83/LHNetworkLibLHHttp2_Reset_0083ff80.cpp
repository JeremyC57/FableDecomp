// LHNetworkLib::LHHttp2::Reset() -- returns the HTTP client to its initial state: destroys the socket, empties the request / header / host / proxy strings, drops the document parts and zeroes the transfer state
// Leaf: ?Reset@LHHttp2@LHNetworkLib@@QAEXXZ
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
// NF123 (2026-11-20, q4193): the local LSocket / CRetailString / CRetailStringVector / LHHttpHeaders / LHHttp2 models replaced by the
//   shared engine/LHHttp2.h (retail branch, STLport std::string / std::vector) and engine/LSocket.h, with the NF123 opt-ins
//   LHHTTP2_RESET (Reset / ResetDocumentParts) and LSOCKET_DTOR. Respelled bindings: erase 0x00841FC0 (was ?erase@CRetailStringVector@@...),
//   assign 0x005693B0 (was ?Assign@CRetailString@@...), ~LSocket 0x008F9780 (was the global ??1LSocket@@QAE@XZ; both PDBs: LHNetworkLib).
#include <string.h>
#pragma intrinsic(memset)
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_RESET
#define FABLE_RETAIL_LSOCKET_DTOR
#include "engine/LHHttp2.h"
#include "engine/LSocket.h"

namespace LHNetworkLib {

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