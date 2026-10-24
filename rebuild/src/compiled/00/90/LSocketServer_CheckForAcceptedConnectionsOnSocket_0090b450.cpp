// LSocketServer::CheckForAcceptedConnectionsOnSocket() -- with a listener socket, runs one network Think(false) pass and reports whether accepted connections are waiting
// Leaf: CheckForAcceptedConnectionsOnSocket
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090B450, 35 bytes. Ego_r.pdb ?CheckForAcceptedConnectionsOnSocket@LSocketServer@LHNetworkLib@@QAE_NXZ.
// Callee: LSocketServer::Think(bool) @ 0x0090B160 (static `__fastcall`, the bool in cl -- PDB ?Think@LSocketServer@LHNetworkLib@@SIX_N@Z;
// manifest "LSocket_ProcessNetworkEvents").
// Layout: retail LSocketServer: ListenerSocket +0, ListOfAcceptedConnections +0x0C (4-byte STLport list: the sentinel node
// pointer; empty when sentinel->next == sentinel).
// Shape: `!list.empty()` returned as a full-register setne (`xor ecx,ecx ; setne cl ; mov al,cl`).
struct LSocket;
struct LAddress {                                  // retail 0x1C bytes (12-byte STLport string)
    unsigned char _body[0x1C];
};

struct CSocketListNode {                           // STLport _List_node<LSocket*>
    CSocketListNode* _M_next;
    CSocketListNode* _M_prev;
    LSocket*         _M_data;
};

struct CSocketList {                               // STLport list<LSocket*>, 4 bytes in retail (the sentinel pointer)
    CSocketListNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
};

class LSocketServer {                              // retail 0x34 bytes (Ego_r.pdb 92 with 12-byte lists and a 44-byte LAddress)
public:
    LSocket*      ListenerSocket;                  // +0x00
    unsigned long MaxConnections;                  // +0x04
    bool          Paused;                          // +0x08
    CSocketList   ListOfAcceptedConnections;       // +0x0C
    CSocketList   ListOfConnectedSockets;          // +0x10
    CSocketList   ListOfUDPDatagramms;             // +0x14
    LAddress      UDPListenerAddress;              // +0x18
    static void __fastcall Think(bool force);      // 0x0090B160
    bool CheckForAcceptedConnectionsOnSocket();
};

bool LSocketServer::CheckForAcceptedConnectionsOnSocket()
{
    if (ListenerSocket == 0)
        return false;
    Think(false);
    return !ListOfAcceptedConnections.empty();
}