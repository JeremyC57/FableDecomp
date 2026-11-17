// LHNetworkLib::LSocket::Receive(vector<char>& data, LAddress& from, const LAddress& local) -- reads waiting data: for a named UDP local address (re)creates the bound datagram socket unless NoListeningBinding, selects for 50 us and recvfrom()s into the buffer, records the sender and closes the socket again; otherwise selects the connected socket and recv()s into the buffer (sized by FIONREAD when empty), treating WSAEWOULDBLOCK / WSAEINPROGRESS as no data; updates the receive counters; false on an error or no data
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): a named `char* buffer = data._M_start;` before recv() gives retail's
//   push-buffer-then-load-socket order (eax reuse); the parked form passed the member inline and hoisted Socket into ecx.
// Leaf: ?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@AAULAddress@2@ABU52@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F9320, 674 bytes (padded to 688), `ret 0xc`, /GS (cookie 0x0139C8A8,
//   __security_check_cookie 0x00BFE9F9). Ego_r.pdb
//   ?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@AAULAddress@2@ABU52@@Z.
// Callees: LSocket::CreateSocket @ 0x008F87A0 (static __fastcall, landed), LSocket::SetNonBlocking @ 0x008F8870 (landed),
//   select (WSOCK32 #18), __WSAFDIsSet @ 0x00C003EC, recvfrom (WSOCK32 #17), recv (WSOCK32 #16), basic_string<char>(const
//   char*, const allocator&) @ 0x00568440 (the dropped "Winsock Error: " report), WSAGetLastError (WSOCK32 #111), _itoa
//   0x00BFED34, free 0x00BFEA14, vector<char>::resize(size_type, const char&) @ 0x004122D0, vector<char>::resize(size_type)
//   @ 0x00412880 (manifest "_Umove<EWeaponClass*>" is propagated: the body is `resize(n, char())`), LSocket::
//   ConvertSockaddr2LAddress @ 0x008F8BB0 (static __fastcall, landed), closesocket (WSOCK32 #3), LSocket::CheckReadSize
//   @ 0x008F8370 (landed).
// Layout: retail LSocket (0x9C bytes) as the landed LSocket rows: Socket +0x38, ConnectionMode +0x60, NoListeningBinding
//   +0x7c, NrTotalDataReceived +0x8c, NrLastDataReceived +0x94.
// Shape: the select + result test (the PDB private SelectStatus, inline) is the LSOCKET_SELECT_READ expansion below; STLport's one-argument resize(n) is
//   inlined (`resize(n, char())`) at the two result sites and called out of line for the empty-buffer sizing (VC7.1's
//   inline budget; modelled with the two-argument form written out); the WSAEWOULDBLOCK / WSAEINPROGRESS range test is
//   an `||`, so its report block is laid out cold after the epilogue.
// Name fix (agentNF54): ported onto the shared engine/LSocket.h (FABLE_RETAIL_LSOCKET_RECEIVE / _LADDRESS: LHNetworkLib::LSocket,
//   LAddress over the STLport std::string, std::vector<char>) with the body the landed one-argument Receive 0x008F99A0 already compiles
//   for this function in the same TU. The row's own symbol is now the PDB spelling
//   ?Receive@LSocket@LHNetworkLib@@QAE_NAAV?$vector@DV?$allocator@D@std@@@std@@AAULAddress@2@ABU52@@Z (Ego_r + FableWin), which the
//   landed callers 0x008F99A0 / 0x0090B160 already bind (was ?Receive@LSocket@@QAE_NAAUCCharVector@@AAULAddress@@ABU2@@Z), and the
//   empty-buffer sizing calls ?resize@?$vector@DV?$allocator@D@std@@@std@@QAEXI@Z 0x00412880 (Ego_r 0x0040E0E0 + FableWin 0x0188AE50,
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL; was the local CCharVector::resize), the two-argument resize 0x004122D0 as
//   ?resize@?$vector@DV?$allocator@D@std@@@std@@QAEXIABD@Z (was CCharVector) and the error text the STLport basic_string(const char*,
//   const allocator&) 0x00568440 (was the local CNetErrorString).
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_LSOCKET_LADDRESS
#define FABLE_RETAIL_LSOCKET_RECEIVE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "engine/LSocket.h"
#include "stlport/fable_string_bodies.h"
extern "C" char* FABLE_CDECL _itoa(int value, char* buffer, int radix);                // 0x00BFED34
struct sockaddr { unsigned short sa_family; char sa_data[14]; };
struct hostent { char* h_name; char** h_aliases; short h_addrtype; short h_length; char** h_addr_list; };
struct timeval { long tv_sec; long tv_usec; };
struct fd_set { unsigned int fd_count; unsigned int fd_array[64]; };
#ifdef _MSC_VER
#define FABLE_WSAAPI __declspec(dllimport) __stdcall
#else
#define FABLE_WSAAPI
#endif
extern "C" int FABLE_WSAAPI WSAGetLastError();                                                         // WSOCK32 #111
extern "C" unsigned long FABLE_WSAAPI inet_addr(const char* cp);                                       // WSOCK32 #10
extern "C" char* FABLE_WSAAPI inet_ntoa(in_addr in);                                                   // WSOCK32 #11
extern "C" hostent* FABLE_WSAAPI gethostbyname(const char* name);                                      // WSOCK32 #52
extern "C" unsigned short FABLE_WSAAPI htons(unsigned short value);                                    // WSOCK32 #9
extern "C" unsigned short FABLE_WSAAPI ntohs(unsigned short value);                                    // WSOCK32 #15
extern "C" unsigned int FABLE_WSAAPI socket(int af, int type, int protocol);                           // WSOCK32 #23
extern "C" int FABLE_WSAAPI bind(unsigned int s, const sockaddr* name, int length);                    // WSOCK32 #2
extern "C" int FABLE_WSAAPI select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const timeval* timeout);   // WSOCK32 #18
extern "C" int FABLE_WSAAPI recvfrom(unsigned int s, char* buffer, int length, int flags, sockaddr_in* from, int* fromLength); // WSOCK32 #17
extern "C" int FABLE_WSAAPI recv(unsigned int s, char* buffer, int length, int flags);                // WSOCK32 #16
extern "C" int FABLE_WSAAPI closesocket(unsigned int s);                                               // WSOCK32 #3
extern "C" int FABLE_STDCALL __WSAFDIsSet(unsigned int fd, fd_set* set);                              // 0x00C003EC (wsock32.lib stub)

namespace LHNetworkLib {

// The select + result test (the PDB's private SelectStatus, inline in retail) as a macro over the member Socket, expanded at
// both sites.
#define LSOCKET_SELECT_READ(result_, microseconds_) \
    { \
        timeval timeout; \
        timeout.tv_sec = 0; \
        timeout.tv_usec = (microseconds_); \
        fd_set readSet; \
        readSet.fd_count = 0; \
        if (readSet.fd_count < 64) \
            readSet.fd_array[readSet.fd_count++] = Socket; \
        int selected = select(0, &readSet, 0, 0, &timeout); \
        if (selected == -1) \
            result_ = false; \
        else if (selected == 1 && __WSAFDIsSet(Socket, &readSet)) \
            result_ = true; \
        else \
            result_ = false; \
    }

bool LSocket::Receive(std::vector<char>& data, LAddress& from, const LAddress& local)
{
    int received = 0;
    NrLastDataReceived = 0;
    if (ConnectionMode == LSP_TCP_PACKET)
        return false;
    if (local.ServerName.c_str()[0] != 0 && local.TransportProtocol == LSP_UDP) {   // a named UDP address
        sockaddr_in fromAddress;
        int fromLength = sizeof(fromAddress);
        if (!NoListeningBinding) {
            Socket = CreateSocket(local, LSP_UDP);
            SetNonBlocking(true);
        }
        bool readable;
        LSOCKET_SELECT_READ(readable, 50)
        if (readable) {
            received = recvfrom(Socket, data.begin(), data.size(), 0, &fromAddress, &fromLength);
            if (received == -1) {
                std::string error("Winsock Error: ");
                char number[50];
                _itoa(WSAGetLastError(), number, 10);
                return false;
            }
        }
        data.resize(received, char());
        ConvertSockaddr2LAddress(fromAddress, from);
        if (!NoListeningBinding)
            closesocket(Socket);
    }
    else {
        if (Socket == (unsigned int)-1)
            return false;
        bool readable;
        LSOCKET_SELECT_READ(readable, 50)
        if (!readable)
            return false;
        unsigned long size = data.size();
        if (size == 0)
            size = CheckReadSize();
        if (data.size() == 0)
            data.resize(size);
        char* buffer = data.begin();
        received = recv(Socket, buffer, size, 0);
        if (received == -1) {
            switch (WSAGetLastError()) {
            case 10053:                            // WSAECONNABORTED
            case 10054:                            // WSAECONNRESET
                received = 0;
                break;
            default: {
                std::string report("Winsock Error: ");
                char number[50];
                _itoa(WSAGetLastError(), number, 10);
                return false;
            }
            }
        }
        data.resize(received, char());
        if (received == 0)
            return false;
    }
    NrTotalDataReceived += received;
    NrLastDataReceived = received;
    return true;
}

}
