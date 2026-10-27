// LHNetworkLib::LSocketServer::GetUDPPacket(std::vector<char>& data, LAddress& source) -- hand out the oldest queued UDP datagram: grow the caller's buffer to fit, copy the bytes and the sender address, unlink the queue node; false when nothing is queued
// Leaf: ?GetUDPPacket@LSocketServer@LHNetworkLib@@QAE_N
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090AF80, 181 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb `?GetUDPPacket@LSocketServer@LHNetworkLib@@QAE_NAAV...` (vector<char>&, LAddress&); the body
//   reads LSocketServer::ListOfUDPDatagramms (list<DatagrammPacket*>, retail +0x14) and copies DatagrammPacket::Data and
//   ::Source (LAddress: ServerName string +0, ServerIP +0xc, ServerPort +0x10, TransportProtocol +0x14, TransportFlags
//   +0x18 -- the PDB member order at retail STLport sizes); its only caller 0x0090B06C is the static GetAnyUDPPacket.
// Callees: vector<char>::resize(unsigned int, const char&) 0x004122D0 (landed), basic_string<char>::_M_assign_dispatch<char*>
//   0x00569140 (landed, through LAddress's implicit operator=), free 0x00BFEA14 (the list node). The packet itself is not
//   freed here.
// Shape: the front node held as an iterator (retail keeps the node in ebx and re-reads the packet pointer at every use; a
//   packet local is read once); `if (data.size() == 0 || data.size() > packet size) data.resize(packet size);` (as retail
//   compares: a larger buffer shrinks, a smaller non-empty one keeps its size and gets a truncated copy), an inline memcpy
//   of data.size() bytes (rep movsd/movsb), the memberwise LAddress copy, then the inline list erase of that node.
extern "C" void* __cdecl memcpy(void* dst, const void* src, unsigned int size);
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    void resize(size_type __new_size, const _Tp& __x);     // <char> 0x004122D0
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};

template <class _CharT> class char_traits { };
template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> > class basic_string {
public:
    _CharT* _M_start;                                       // +0x00
    _CharT* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_CharT*, _CharT, _Alloc> _M_end_of_storage;   // +0x08
    template <class _InputIter> basic_string& _M_assign_dispatch(_InputIter __f, _InputIter __l, const __false_type&);   // <char*> 0x00569140
    template <class _InputIter> basic_string& assign(_InputIter __first, _InputIter __last)
    {
        return _M_assign_dispatch(__first, __last, __false_type());
    }
    basic_string& operator=(const basic_string& __s)
    {
        if (&__s != this)
            assign(__s._M_start, __s._M_finish);
        return *this;
    }
};
typedef basic_string<char> string;

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
template <class _Tp> struct _List_iterator {
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    bool empty() const { return _M_node._M_data->_M_next == _M_node._M_data; }
    _Tp& front() { return *begin(); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void pop_front() { erase(begin()); }
};
}

namespace LHNetworkLib {
enum LSOCKET_PROTOCOL { LSOCKET_PROTOCOL_NONE = 0 };
enum LSOCKET_FLAGS { LSOCKET_FLAGS_NONE = 0 };
struct LAddress {                                           // retail 0x1c bytes
    std::string      ServerName;                            // +0x00
    unsigned long    ServerIP;                              // +0x0c
    unsigned short   ServerPort;                            // +0x10
    LSOCKET_PROTOCOL TransportProtocol;                     // +0x14
    LSOCKET_FLAGS    TransportFlags;                        // +0x18
};
struct DatagrammPacket {
    std::vector<char> Data;                                 // +0x00
    LAddress          Source;                               // +0x0c
};
class LSocket;
class LSocketServer {
public:
    LSocket*                     ListenerSocket;            // +0x00
    unsigned long                MaxConnections;            // +0x04
    bool                         Paused;                    // +0x08
    std::list<LSocket*>          ListOfAcceptedConnections; // +0x0c
    std::list<LSocket*>          ListOfConnectedSockets;    // +0x10
    std::list<DatagrammPacket*>  ListOfUDPDatagramms;       // +0x14
    bool GetUDPPacket(std::vector<char>& data, LAddress& source);
};

bool LSocketServer::GetUDPPacket(std::vector<char>& data, LAddress& source)
{
    if (ListOfUDPDatagramms.empty())
        return false;
    std::list<DatagrammPacket*>::iterator packet = ListOfUDPDatagramms.begin();
    if (data.size() == 0 || data.size() > (*packet)->Data.size())
        data.resize((*packet)->Data.size());
    memcpy(data._M_start, (*packet)->Data._M_start, data.size());
    source = (*packet)->Source;
    ListOfUDPDatagramms.erase(packet);
    return true;
}
}