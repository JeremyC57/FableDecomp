// LHNetworkLib::LSocket::CheckForActivity(list<LSocket*>& sockets, LSOCKET_SELECT mode, unsigned long timeoutMs) -- select() over every socket of the list for reading / writing / errors (50 us when 0, ms*1000 us otherwise, blocking when -1) and flag the sockets that have activity; false only on an empty list or a select error
// Leaf: ?CheckForActivity@LSocket@LHNetworkLib@@SI_NAAV?$list@PAVLSocket@LHNetworkLib@@V?$allocator@PAVLSocket@LHNetworkLib@@@std@@@std@@W4LSOCKET_SELECT@2@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8540, 352 bytes (+ int3 padding), static __fastcall (sockets in ecx, mode in
//   edx, the timeout on the stack), `ret 4`.
// Identity: Ego_r.pdb `?CheckForActivity@LSocket@LHNetworkLib@@SI_NAAV?$list@PAVLSocket@LHNetworkLib@@..@std@@
//   W4LSOCKET_SELECT@2@K@Z` (the manifest's __fastcall bool, 3 parameters). Called by LSocket::Think / the network event
//   pump (0x0090B160).
// Callees: select (WSOCK32 ordinal 18, IAT 0x0144055C), __WSAFDIsSet 0x00C003EC (the static wsock32.lib helper).
// Globals: the "activity seen" flag byte 0x013BAFFD (descriptive name; cleared on entry, set when select reports any).
// Layout: engine/LSocket.h (Ego_r LSocket, 0x9C: Socket +0x38, ActivityPending +0x84; CheckForActivity declared by the new
//   opt-in FABLE_RETAIL_LSOCKET_CHECKFORACTIVITY, hdr/README.md), engine/LHNetworkTypes.h LSOCKET_SELECT (LSS_READ 1, LSS_WRITE 2,
//   LSS_EXCEPTION 4); STLport 4.6 list<LSocket*> from stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE size()).
// Shape: `size()` is STLport's counting distance(begin(), end()); the winsock 1.1 FD_SET macro with its duplicate scan
//   (`fd_array[i] == fd`, then the FD_SETSIZE 64 test, the fd re-read for the store); the LSOCKET_SELECT switch picks
//   which of the read / write / except set pointers gets the set (1, 2, 4); the -1 timeout passes NULL. The three set pointers
//   are zeroed by one chained assignment `exceptSet = readSet = writeSet = 0;`: exceptSet takes writeSet's zero register
//   (ebp, 0x008F85A4) and the switch value copy follows `xor ebp,ebp` as retail; separate initialisers (four orders tried in
//   October) store it from readSet's ebx.
struct timeval {                                   // winsock timeval
    long tv_sec;
    long tv_usec;
};

struct fd_set {                                    // winsock 1.1 fd_set, FD_SETSIZE 64
    unsigned int fd_count;
    unsigned int fd_array[64];
};

extern "C" __declspec(dllimport) int __stdcall select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
                                                      const timeval* timeout);   // WSOCK32 #18
extern "C" int __stdcall __WSAFDIsSet(unsigned int fd, fd_set* set);              // 0x00C003EC (wsock32.lib stub)

#define FD_SETSIZE 64
#define FD_ZERO(set) (((fd_set*)(set))->fd_count = 0)
#define FD_SET(fd, set) do { \
    unsigned int __i; \
    for (__i = 0; __i < ((fd_set*)(set))->fd_count; __i++) { \
        if (((fd_set*)(set))->fd_array[__i] == (fd)) { \
            break; \
        } \
    } \
    if (__i == ((fd_set*)(set))->fd_count) { \
        if (((fd_set*)(set))->fd_count < FD_SETSIZE) { \
            ((fd_set*)(set))->fd_array[__i] = (fd); \
            ((fd_set*)(set))->fd_count++; \
        } \
    } \
} while (0)
#define FD_ISSET(fd, set) __WSAFDIsSet((unsigned int)(fd), (fd_set*)(set))

#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE
#include "stlport/fable_containers.h"

extern bool g_LSocketActivityDetected;             // 0x013BAFFD

#define FABLE_RETAIL_LSOCKET_CHECKFORACTIVITY
#include "engine/LSocket.h"

namespace LHNetworkLib {

bool __fastcall LSocket::CheckForActivity(std::list<LSocket*>& sockets, LSOCKET_SELECT mode, unsigned long timeoutMs)
{
    g_LSocketActivityDetected = false;
    if (sockets.size() == 0)
        return false;
    timeval timeout;
    if (timeoutMs == 0)
        timeout.tv_usec = 50;
    else
        timeout.tv_usec = timeoutMs * 1000;
    timeout.tv_sec = 0;
    fd_set set;
    FD_ZERO(&set);
    fd_set* readSet;
    fd_set* writeSet;
    fd_set* exceptSet;
    exceptSet = readSet = writeSet = 0;
    switch (mode) {
    case LSS_READ:
        readSet = &set;
        break;
    case LSS_WRITE:
        writeSet = &set;
        break;
    case LSS_EXCEPTION:
        exceptSet = &set;
        break;
    }
    for (std::list<LSocket*>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
        LSocket* socket = *it;
        FD_SET(socket->Socket, &set);
        socket->ActivityPending = false;
    }
    int result;
    if (timeoutMs == (unsigned long)-1)
        result = select(0, readSet, writeSet, exceptSet, 0);
    else
        result = select(0, readSet, writeSet, exceptSet, &timeout);
    if (result == -1)
        return false;
    if (result > 0) {
        for (std::list<LSocket*>::iterator it = sockets.begin(); it != sockets.end(); ++it) {
            if (FD_ISSET((*it)->Socket, &set))
                (*it)->ActivityPending = true;
        }
        g_LSocketActivityDetected = true;
    }
    return true;
}

} // namespace LHNetworkLib