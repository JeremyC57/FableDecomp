// LHNetworkLib::LSocket::CheckForActivity(list<LSocket*>& sockets, LSOCKET_SELECT mode, unsigned long timeoutMs) -- select() over every socket of the list for reading / writing / errors (50 us when 0, ms*1000 us otherwise, blocking when -1) and flag the sockets that have activity; false only on an empty list or a select error
// Grade: DIFFER(344v344) -- one register and one schedule: exceptSet's zero is stored from ebx (readSet) where retail uses ebp (writeSet), and `mov eax,edx` (the switch value) sits before `xor ebp,ebp`; tried four declaration orders of the three set pointers (the first order also swapped ebx/ebp throughout)
// Leaf: ?CheckForActivity@LSocket@LHNetworkLib@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F8540, 352 bytes (+ int3 padding), static __fastcall (sockets in ecx, mode in
//   edx, the timeout on the stack), `ret 4`.
// Identity: Ego_r.pdb `?CheckForActivity@LSocket@LHNetworkLib@@SI_NAAV?$list@PAVLSocket@LHNetworkLib@@..@std@@
//   W4LSOCKET_SELECT@2@K@Z` (the manifest's __fastcall bool, 3 parameters). Called by LSocket::Think / the network event
//   pump (0x0090B160).
// Callees: select (WSOCK32 ordinal 18, IAT 0x0144055C), __WSAFDIsSet 0x00C003EC (the static wsock32.lib helper).
// Globals: the "activity seen" flag byte 0x013BAFFD (descriptive name; cleared on entry, set when select reports any).
// Layout: retail LSocket as the landed LSocket::IsReadable 0x008F8210 (Socket +0x38, ActivityPending +0x84); STLport
//   list<LSocket*> (sentinel _M_node, node data at +8).
// Shape: `size()` is STLport's counting distance(begin(), end()); the winsock 1.1 FD_SET macro with its duplicate scan
//   (`fd_array[i] == fd`, then the FD_SETSIZE 64 test, the fd re-read for the store); the LSOCKET_SELECT switch picks
//   which of the read / write / except set pointers gets the set (1, 2, 4); the -1 timeout passes NULL.
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

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                      // +0x00
    _List_node_base* _M_prev;                      // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                   // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator==(const _List_iterator& __x) const { return _M_node == __x._M_node; }
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _InputIterator>
inline unsigned int distance(_InputIterator __first, _InputIterator __last)
{
    unsigned int __n = 0;
    while (__first != __last) {
        ++__first;
        ++__n;
    }
    return __n;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node<_Tp>* _M_node;                      // the sentinel
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    unsigned int size() { return distance(begin(), end()); }
};
} // namespace std

extern bool g_LSocketActivityDetected;             // 0x013BAFFD

namespace LHNetworkLib {

enum LSOCKET_SELECT {
    LSOCKET_SELECT_READ   = 1,
    LSOCKET_SELECT_WRITE  = 2,
    LSOCKET_SELECT_EXCEPT = 4
};

class LSocket {                                    // retail 0x9C bytes
public:
    unsigned char  _pad00[0x38];                   // RemoteAddress / LocalAddress (two 0x1C LAddress)
    unsigned int   Socket;                         // +0x38
    unsigned char  _pad3C[0x84 - 0x3C];            // the sockaddr_in pair, mode, queues, server object
    bool           ActivityPending;                // +0x84
    static bool __fastcall CheckForActivity(std::list<LSocket*>& sockets, LSOCKET_SELECT mode, unsigned long timeoutMs);
};

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
    fd_set* exceptSet = 0;
    fd_set* writeSet = 0;
    fd_set* readSet = 0;
    switch (mode) {
    case LSOCKET_SELECT_READ:
        readSet = &set;
        break;
    case LSOCKET_SELECT_WRITE:
        writeSet = &set;
        break;
    case LSOCKET_SELECT_EXCEPT:
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
