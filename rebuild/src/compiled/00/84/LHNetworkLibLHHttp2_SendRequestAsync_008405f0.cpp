// LHNetworkLib::LHHttp2::SendRequestAsync() -- push the next block (at most 1 KiB) of the prepared request down the socket: -104 with no request, 2 (and a reset for the response) once everything is sent, -100 when the send fails, -102 when nothing went out for longer than the time-out, else 1
// Leaf: ?SendRequestAsync@LHHttp2@LHNetworkLib@@QAE?AW4LH_HTTP_STATUS@2@XZ
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
// NF123 (2026-11-20, q4193): the local std::vector / basic_string / CRetailString / CRetailStringVector / LAddress / LSocket /
//   LHHttpHeaders / LHHttp2 models replaced by the shared engine/LHHttp2.h (retail branch) and engine/LSocket.h, with the NF123
//   opt-in LHHTTP2_SENDREQUEST; the same-TU SendRawData / ResetRequest bodies are kept as the landed row had them. Respelled
//   bindings: erase 0x00841FC0, assign 0x005693B0 (were the CRetailString* stand-ins).
#include <string.h>
#pragma intrinsic(memcmp, memset, strlen)
extern "C" long __cdecl time(long* timer);                       // 0x00BFED22
#pragma intrinsic(memcpy)
#include "rebuild_abi.h"
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_LHHTTP2_SENDREQUEST
#define FABLE_RETAIL_LSOCKET_SEND_PASS23
#define FABLE_RETAIL_LSOCKET_LADDRESS_CTOR_OUTOFLINE
#include "engine/LHHttp2.h"
#include "engine/LSocket.h"

namespace LHNetworkLib {

#pragma auto_inline(off)
LH_RETURN LHHttp2::SendRawData(char* data, unsigned long size, unsigned long* bytesSent, bool wait)   // 0x0083F8B0 (landed)
{
    if (!Host)
        return LH_FAIL;
    std::vector<char> buffer;
    buffer.resize(size);
    memcpy(&buffer[0], data, size);
    Host->Send(buffer, LAddress(), LAddress());
    *bytesSent = Host->NrLastDataSent;
    return LH_OK;
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
        return HTTP_STATUS_NO_REQUEST_PREPARED;
    if (RequestBytesSent >= RequestLength) {
        ResetRequest();
        ReceivedHeaders.Reset();
        ActionStarted = false;
        CurrentAction = 1;
        return HTTP_STATUS_REQUEST_SENT;
    }
    unsigned long size = RequestLength - RequestBytesSent;
    if (size >= 1024)
        size = 1024;
    unsigned long bytesSent = 0;
    if (SendRawData(&PreparedRequest[RequestBytesSent], size, &bytesSent, true) == LH_ERROR) {
        ResetRequest();
        memset(CurrentHeaderPart, 0, sizeof(CurrentHeaderPart));
        CurrentHeaderPos = 0;
        return HTTP_STATUS_SENDING_ERROR;
    }
    if (bytesSent == 0 && ActionStarted) {
        if ((unsigned long)time(0) > ActionStartTime + TimeOutValue) {
            ResetRequest();
            return HTTP_STATUS_TIMEOUT_ERROR;
        }
    } else {
        time((long*)&ActionStartTime);
        ActionStarted = true;
    }
    RequestBytesSent += bytesSent;
    return HTTP_STATUS_SENDING_REQUEST;
}

}