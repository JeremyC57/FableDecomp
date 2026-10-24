// LHNetworkLib::LSocket::Think() -- when output was queued, try to flush every open socket's send queue, then clear the flag; always true
// Leaf: ?Think@LSocket@LHNetworkLib@@
// Retail 0x008F84A0, 0x39 bytes. PDB name set: `?Think@LSocket@LHNetworkLib@@SI_NXZ` (static __fastcall bool, no arguments);
// the manifest "Think" (_global, __fastcall bool) agrees. It sits in the LSocket TU between TryToSendDataFromQueue
// (0x008F8440, its callee) and CheckForActivity (0x008F8540).
// Globals: the static send-pending flag at 0x013BAFFC (descriptive name; no PDB symbol) and the global
// std::list<LSocket*> of live sockets at 0x013BB000 (the landed ~LSocket 0x008F9780 erases `this` from it). Retail
// STLport list = one header-node pointer; node next +0, prev +4, value +8.
// Callee: LSocket::TryToSendDataFromQueue @ 0x008F8440 (thiscall, bool result dropped).
// Shape: `end()` is re-read from the global only after the call (VC keeps it in eax across the non-calling arm); the
// socket test is `Socket != INVALID_SOCKET` (+0x38 in the 0x9C-byte retail LSocket).
namespace LHNetworkLib {

class LSocket;

struct SSocketListNode {
    SSocketListNode* _M_next;                   // +0x00
    SSocketListNode* _M_prev;                   // +0x04
    LSocket*         _M_data;                   // +0x08
};

struct CSocketList {                            // STLport list<LSocket*>, 4 bytes
    SSocketListNode* _M_node;
    SSocketListNode* begin() const { return _M_node->_M_next; }
    SSocketListNode* end() const { return _M_node; }
};

extern bool        g_bSocketDataQueued;         // 0x013BAFFC
extern CSocketList g_AllSockets;                // 0x013BB000

class LSocket {
public:
    unsigned char  RemoteAddress[0x1C];         // +0x00 LAddress
    unsigned char  LocalAddress[0x1C];          // +0x1C LAddress
    unsigned int   Socket;                      // +0x38
    unsigned char  _rest[0x60];                 // +0x3C .. 0x9C
    bool TryToSendDataFromQueue();              // 0x008F8440
    static bool __fastcall Think();
};

bool __fastcall LSocket::Think()
{
    if (g_bSocketDataQueued) {
        for (SSocketListNode* it = g_AllSockets.begin(); it != g_AllSockets.end(); it = it->_M_next) {
            if (it->_M_data->Socket != (unsigned int)-1)
                it->_M_data->TryToSendDataFromQueue();
        }
        g_bSocketDataQueued = false;
    }
    return true;
}

}