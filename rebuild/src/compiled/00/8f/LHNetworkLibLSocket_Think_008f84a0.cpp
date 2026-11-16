// LHNetworkLib::LSocket::Think() -- when output was queued, try to flush every open socket's send queue, then clear the flag; always true
// Leaf: ?Think@LSocket@LHNetworkLib@@SI_NXZ
// Retail 0x008F84A0, 0x39 bytes. PDB name set: `?Think@LSocket@LHNetworkLib@@SI_NXZ` (static __fastcall bool, no arguments). It sits in
// the LSocket TU between TryToSendDataFromQueue (0x008F8440, its callee) and CheckForActivity (0x008F8540).
// Globals: the private static LSocket::NeedToFlush at 0x013BAFFC (both PDBs; engine/LSocket.h FABLE_RETAIL_LSOCKET_SEND_PASS23) and the
// public static std::list<LSocket*> LSocket::GlobalList at 0x013BB000 (the landed initializer 0x0121F850 emits that symbol).
// Callee: LSocket::TryToSendDataFromQueue @ 0x008F8440 (private thiscall, bool result dropped).
// Shape: `end()` is re-read from the global only after the call; the socket test is IsConnected() (`Socket != INVALID_SOCKET`, +0x38).
// NF42 (round 5): the local LSocket / socket-list models and the descriptive g_bSocketDataQueued / g_AllSockets globals replaced by the
// shared engine/LSocket.h (overlay opt-in FABLE_RETAIL_LSOCKET_THINK: Think, TryToSendDataFromQueue, GlobalList).
#define FABLE_RETAIL_LSOCKET_SEND_PASS23
#define FABLE_RETAIL_LSOCKET_THINK
#include "rebuild_abi.h"
#include "engine/LSocket.h"
namespace LHNetworkLib {
bool FABLE_FASTCALL LSocket::Think()
{
    if (NeedToFlush) {
        for (std::list<LSocket*>::iterator it = GlobalList.begin(); it != GlobalList.end(); ++it) {
            if ((*it)->IsConnected())
                (*it)->TryToSendDataFromQueue();
        }
        NeedToFlush = false;
    }
    return true;
}
}
