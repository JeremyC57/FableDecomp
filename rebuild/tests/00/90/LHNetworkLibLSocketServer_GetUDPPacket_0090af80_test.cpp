// Behaviour test for 0x0090AF80 LHNetworkLib::LSocketServer::GetUDPPacket: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#endif
#pragma intrinsic(memcpy)

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

static int g_assigns = 0;
template <> void tstd::vector<char>::resize(unsigned int n, const char& x)
{
    char* p = (char*)malloc(n ? n : 1); unsigned int old = size();
    if (old) memcpy(p, _M_start, old < n ? old : n);
    for (unsigned int i = old; i < n; ++i) p[i] = x;
    free(_M_start); _M_start = p; _M_finish = p + n; _M_end_of_storage._M_data = p + n;
}
template <> template <> tstd::string& tstd::string::_M_assign_dispatch<char*>(char* f, char* l, const __false_type&)
{
    ++g_assigns; _M_start = f; _M_finish = l; return *this;          // records the source range
}
typedef tstd::_List_node<LHNetworkLib::DatagrammPacket*> Node;
int main()
{
    LHNetworkLib::LSocketServer* s = (LHNetworkLib::LSocketServer*)calloc(1, sizeof(LHNetworkLib::LSocketServer));
    Node* header = (Node*)malloc(sizeof(Node)); header->_M_next = header; header->_M_prev = header;
    s->ListOfUDPDatagramms._M_node._M_data = header;
    tstd::vector<char>* data = (tstd::vector<char>*)calloc(1, sizeof(tstd::vector<char>));
    LHNetworkLib::LAddress* from = (LHNetworkLib::LAddress*)calloc(1, sizeof(LHNetworkLib::LAddress));
    int ok = !s->GetUDPPacket(*data, *from);
    static char bytes[] = "hello!", host[] = "peer";
    LHNetworkLib::DatagrammPacket* pk = (LHNetworkLib::DatagrammPacket*)calloc(1, sizeof(LHNetworkLib::DatagrammPacket));
    pk->Data._M_start = bytes; pk->Data._M_finish = bytes + 6;
    pk->Source.ServerName._M_start = host; pk->Source.ServerName._M_finish = host + 4;
    pk->Source.ServerIP = 0x7f000001; pk->Source.ServerPort = 4000;
    pk->Source.TransportProtocol = (LHNetworkLib::LSOCKET_PROTOCOL)1; pk->Source.TransportFlags = (LHNetworkLib::LSOCKET_FLAGS)2;
    Node* n = (Node*)malloc(sizeof(Node)); n->_M_data = pk; n->_M_next = header; n->_M_prev = header; header->_M_next = n; header->_M_prev = n;
    ok &= s->GetUDPPacket(*data, *from);                              // empty buffer: resized to 6, copied
    ok &= data->size() == 6 && memcmp(data->_M_start, "hello!", 6) == 0 && header->_M_next == header && header->_M_prev == header;
    ok &= g_assigns == 1 && from->ServerName._M_start == host && from->ServerIP == 0x7f000001 && from->ServerPort == 4000;
    ok &= from->TransportProtocol == 1 && from->TransportFlags == 2;
    n = (Node*)malloc(sizeof(Node)); n->_M_data = pk; n->_M_next = header; n->_M_prev = header; header->_M_next = n; header->_M_prev = n;
    data->resize(3, 'z');                                             // a smaller non-empty buffer keeps its size
    ok &= s->GetUDPPacket(*data, *from) && data->size() == 3 && memcmp(data->_M_start, "hel", 3) == 0;
    n = (Node*)malloc(sizeof(Node)); n->_M_data = pk; n->_M_next = header; n->_M_prev = header; header->_M_next = n; header->_M_prev = n;
    data->resize(10, 'z');                                            // a larger one shrinks to the packet
    ok &= s->GetUDPPacket(*data, *from) && data->size() == 6;
    if (ok) printf("0090af80_TEST PASS\n");
    return ok ? 0 : 1;
}