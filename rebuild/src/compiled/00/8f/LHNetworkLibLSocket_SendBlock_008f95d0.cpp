// LHNetworkLib::LSocket::SendBlock(std::vector<char>& data) -- in TCP-packet mode, send a 4-byte length header then the block itself through
// plain-TCP Send calls (mode switched to TCP for the two sends and back after); true when both sends succeed
// Leaf: ?SendBlock@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z
// Flags: /O2 /Oy
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F95D0, 263 bytes, thiscall `ret 4`; the TU is built without /GX (no EH frame: the LAddress
//   temporaries' strings and the header vector are freed in place).
// Shared headers (NF67 port, finished by agentNF71 round 7): engine/LSocket.h (FABLE_RETAIL_LSOCKET_SEND_PASS23: Send(vector<char>&,
//   const LAddress&, const LAddress&) 0x008F8F50, ConnectionMode +0x60; SENDBLOCK declares this member, LADDRESS_CTOR_OUTOFLINE the
//   out-of-line ??0LAddress 0x00841E90) and the STLport vector<char> (insert(end(), 4, 0) -> _M_fill_insert 0x004110D0).
// Locals: Ego_d.pdb (TLC source, VC10 /Od) names them retval (bool), data_length (vector<char>) and templength (unsigned long).
// Release: retail frees data_length with a bare `mov ecx,[esp+10h] ; push ecx ; call free` (0x008F96B8; the same TU's ReceiveBlock
//   0x008F9B20 likewise) while the LAddress strings keep their null tests; FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED (agentNF71
//   overlay opt-in, allowed by the round-7 brief) gives that release, and with it `retval` shares ebx with the zero (`mov bl,1`).
#include <stdlib.h>
#include <string.h>
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_FREE_UNTESTED
#define FABLE_RETAIL_LSOCKET_SEND_PASS23
#define FABLE_RETAIL_LSOCKET_SENDBLOCK
#define FABLE_RETAIL_LSOCKET_LADDRESS_CTOR_OUTOFLINE
#include "engine/LSocket.h"
namespace LHNetworkLib {
bool LSocket::SendBlock(std::vector<char>& data)
{
    if (ConnectionMode != LSP_TCP_PACKET)
        return false;
    ConnectionMode = LSP_TCP;
    bool retval = false;
    std::vector<char> data_length;
    data_length.insert(data_length.end(), 4, char(0));
    unsigned long templength = data.size();
    memcpy(&data_length[0], &templength, sizeof(templength));
    if (Send(data_length, LAddress(), LAddress())) {
        if (Send(data, LAddress(), LAddress()))
            retval = true;
    }
    ConnectionMode = LSP_TCP_PACKET;
    return retval;
}
}
