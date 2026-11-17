#ifndef FABLE_RETAIL_LSOCKET_HEADER
#define FABLE_RETAIL_LSOCKET_HEADER
#include "stlport/fable_string.h"
#include "LHNetworkTypes.h"
#if defined(FABLE_RETAIL_LSOCKET_RECEIVE) || defined(FABLE_RETAIL_LSOCKET_CHECKFORACTIVITY)
#include "rebuild_abi.h"
#endif
#include <stdio.h>
// SDK Winsock structures: original CodeView fields and native 16-byte copies.
struct in_addr {
    union {
        struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { unsigned short s_w1,s_w2; } S_un_w;
        unsigned long S_addr;
    } S_un;
};
struct sockaddr_in { short sin_family;unsigned short sin_port;in_addr sin_addr;char sin_zero[8]; };
namespace LHNetworkLib {
struct LAddress {
    std::string ServerName;
    unsigned long ServerIP;
    unsigned short ServerPort;
    LSOCKET_PROTOCOL TransportProtocol;
    LSOCKET_FLAGS TransportFlags;
#if defined(FABLE_RETAIL_LSOCKET_LADDRESS_CTOR_OUTOFLINE) && !defined(FABLE_RETAIL_LSOCKET_LADDRESS)
    // NF67 opt-in (round 7, row 0x008F95D0 LSocket::SendBlock): FableWin.pdb ??0LAddress@LHNetworkLib@@QAE@XZ, called out of line in that TU
    // (0x00841E90, `lea ecx,[esp+38h] ; call`, the this pointer pushed as Send's argument). Declaration only, no layout change.
    LAddress();                                                          // 0x00841E90
#endif
#ifdef FABLE_RETAIL_LSOCKET_LADDRESS
    // Retail opt-in (chunk 892, row 0x008F99A0 LSocket::Receive(vector<char>&)): Ego_r.pdb / FableWin.pdb ??0LAddress@LHNetworkLib@@QAE@XZ
    // and ?ClearData@LAddress@LHNetworkLib@@QAEXXZ (public), inline in retail: each default LAddress builds the STLport empty string
    // (malloc(8), terminator) and ClearData assigns "" (assign(s, s + 0): the literal pushed twice, 0x008F99B4 / 0x008F99BF), then
    // zeroes the scalars from one zero register (ServerPort's word store first, 0x008F99D9).
    LAddress() { ClearData(); }
    void ClearData()
    {
        ServerName = "";
        ServerPort = 0;
        ServerIP = 0;
        TransportProtocol = LSP_NONE;
        TransportFlags = LSF_NONE;
    }
#endif
};
#ifdef FABLE_RETAIL_LSOCKET_SEND_PASS23
// Original Ego_r/FableWin complete class: empty, size1, no bases or members.
class LSocketConnError {};
#endif
class LSocketServer;
class LSocket {
#ifdef FABLE_RETAIL_LSOCKET_SEND_PASS23
    // Retail opt-in (Codex pass23): both original PDBs and native008f8f50/970-byte parent.
private:
    static bool NeedToFlush;
    bool IsWriteable(long);
    static bool FABLE_FASTCALL ConvertLAddress2Sockaddr(const LAddress&,sockaddr_in&);
public:
    bool Send(std::vector<char>&,const LAddress&,const LAddress&);
    bool Close();
    LSOCKET_PROTOCOL GetMode(){return ConnectionMode;}
    bool IsConnected(){if(Socket==(unsigned int)-1)return false;return true;}
private:
#endif
    LAddress RemoteAddress;
    LAddress LocalAddress;
    unsigned int Socket;
    sockaddr_in SocketInformation;
    sockaddr_in SocketInterfaceInformation;
    bool ListeningSocket;
    LSOCKET_PROTOCOL ConnectionMode;
    std::vector<char> SendOutputQueue;
    std::vector<char> InternalPacketBuffer;
    bool NoListeningBinding;
    LSocketServer* ServerObject;
public:
    bool ActivityPending;
    unsigned long NrTotalDataSent;
    unsigned long NrTotalDataReceived;
    unsigned long NrLastDataSent;
    unsigned long NrLastDataReceived;
    FILE* _DEBUGFP;
    bool IsReadable(long);
    bool Receive(std::vector<char>&);
#ifdef FABLE_RETAIL_LSOCKET_SENDBLOCK
    // NF67 opt-in (round 7, row 0x008F95D0 itself): Ego_r.pdb / FableWin.pdb ?SendBlock@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@@Z
    // (public). Declaration only, no layout change.
    bool SendBlock(std::vector<char>& data);                            // 0x008F95D0
#endif
#ifdef FABLE_RETAIL_LSOCKET_RECEIVE
    // Retail opt-in (chunk 892, row 0x008F99A0): the LSocket.cpp members the one-argument Receive reaches, with Ego_r.pdb's access
    // (SI public static __fastcall, CI private static __fastcall, AAE private, QAE public). Declarations; the row defines the bodies
    // retail compiles into the same TU (0x008F86A0 .. 0x008F9320), so VC7.1 sees that none of them captures an LAddress.
    static unsigned long FABLE_FASTCALL ResolveHost(const std::string& host);                     // 0x008F86A0
    static bool FABLE_FASTCALL ResolveHost(LAddress& address, unsigned long& ip);                 // 0x008F86F0
    unsigned long CheckReadSize();                                                            // 0x008F8370
    bool Receive(std::vector<char>& data, LAddress& from, const LAddress& local);             // 0x008F9320
private:
    static unsigned int FABLE_FASTCALL CreateSocket(const LAddress& address, LSOCKET_PROTOCOL protocol);   // 0x008F87A0
    static bool FABLE_FASTCALL ConvertSockaddr2LAddress(const sockaddr_in& from, LAddress& address);       // 0x008F8BB0
    bool SetNonBlocking(bool nonBlocking);                                                    // 0x008F8870
public:
#endif
#ifdef FABLE_RETAIL_LSOCKET_CHECKFORACTIVITY
    // Opt-in (chunk 898, row 0x008F8540): Ego_r.pdb ?CheckForActivity@LSocket@LHNetworkLib@@SI_NAAV?$list@PAVLSocket@LHNetworkLib@@V?$allocator@
    // PAVLSocket@LHNetworkLib@@@std@@@std@@W4LSOCKET_SELECT@2@K@Z (public static __fastcall; the includer brings std::list from
    // stlport/fable_containers.h). Declaration only: no layout change.
    static bool FABLE_FASTCALL CheckForActivity(std::list<LSocket*>& sockets, LSOCKET_SELECT mode, unsigned long timeoutMs);   // 0x008F8540
#endif
#ifdef FABLE_RETAIL_LSOCKET_THINK
    // Opt-in (NF42 round 5, row 0x008F84A0 LSocket::Think): Ego_r.pdb ?Think@LSocket@LHNetworkLib@@SI_NXZ (public static __fastcall),
    // ?TryToSendDataFromQueue@LSocket@LHNetworkLib@@AAE_NXZ (private, 0x008F8440) and the public static GlobalList (0x013BB000, the
    // spelling the landed dynamic initializer 0x0121F850 emits). Needs FABLE_RETAIL_LSOCKET_SEND_PASS23 (NeedToFlush, IsConnected) and
    // std::list from stlport/fable_containers.h. Declarations only.
public:
    static std::list<LSocket*> GlobalList;
    static bool FABLE_FASTCALL Think();
private:
    bool TryToSendDataFromQueue();
#endif
};
}
#ifdef _MSC_VER
typedef char LSocketSizeCheck[sizeof(LHNetworkLib::LSocket)==0x9c?1:-1];
#endif
#endif
