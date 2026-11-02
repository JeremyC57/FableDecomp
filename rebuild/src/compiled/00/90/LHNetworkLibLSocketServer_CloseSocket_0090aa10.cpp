// LHNetworkLib::LSocketServer::CloseSocket(LSocket* socket) -- find the socket in the server's connected-socket list and remove it
// Leaf: ?CloseSocket@LSocketServer@LHNetworkLib@@QAEXPAVLSocket@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090AA10, 41 bytes (int3 padding to 0x0090AA40), thiscall; the inlined STLport find over
//   the list at +0x10 (head node, `cmp [node+8],socket` -- _M_data of _List_node<LSocket*>), then `this+0x10` stays in ecx and
//   the found node's data address replaces the argument slot for the tail jmp to list<LSocket*>::remove.
// Identity (proven, two sources): (1) Ego_r.pdb ?CloseSocket@LSocketServer@LHNetworkLib@@QAEXPAVLSocket@2@@Z (namespace
//   LHNetworkLib per the PDB: every LSocket / LSocketServer symbol is in it); the only caller is LHNetworkLib::LSocket::Close
//   0x008F8C70, which passes its own `this` as the socket; (2) the list at +0x10 is ListOfConnectedSockets by the Ego_r.pdb
//   member order (ListenerSocket, MaxConnections, Paused, ID, ListOfAcceptedConnections +0x0C, ListOfConnectedSockets +0x10,
//   ListOfUDPDatagramms +0x14, UDPListenerAddress +0x18), confirmed by the three list-head allocations of the LSocketServer
//   constructor 0x0090AEC0. The landed binding without the namespace is wrong (NAME_FIXES).
// Callee: std::list<LHNetworkLib::LSocket*>::remove(LSocket* const&) 0x0090B8E0 (tail; the catalog's "list<CTCBase*>::remove"
//   and the namespace-less bound spelling are stand-ins -- NAME_FIXES gives the emitted STLport spelling).
// Layout: no shared header for LSocketServer; restated per Ego_r.pdb with the STLport 4-byte list (head-node proxy), ID and
//   UDPListenerAddress (LAddress, 0x1C) as opaque storage, sizeof 0x34.
// Shape: STLport 4.6 find (__find over input_iterator_tag: the `!(*__first == __val)` loop) and remove(*it) -- the source
//   passes the found element by reference, which is the `add eax,8 ; mov [esp+4],eax` before the tail jump.
#include "rebuild_abi.h"
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };

template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

// STLport 4.6 stl/_list.h
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Dummy> class _List_global {
public:
    typedef _List_node_base _Node;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};
struct _List_iterator_base {
    typedef bidirectional_iterator_tag iterator_category;
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _Tp value_type;
    typedef typename _Traits::pointer pointer;
    typedef typename _Traits::reference reference;
    typedef _List_iterator<_Tp, _Traits> _Self;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _List_iterator<_Tp, _Nonconst_traits<_Tp> >& __x) : _List_iterator_base(__x._M_node) { }
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Self& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
protected:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;
public:
    typedef _Tp value_type;
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    void remove(const _Tp& __val);                          // out of line (0x0090B8E0 for <LSocket*>)
};

// STLport 4.6 stl/_algobase.h
template <class _InputIter, class _Tp>
inline _InputIter __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, typename _InputIter::iterator_category());
}
}

namespace LHNetworkLib {
class LSocket;
struct DatagrammPacket;
struct LAddress;
class LSocketServer {
public:
    void CloseSocket(LSocket* socket);
    LSocket*                         ListenerSocket;            // +0x00
    unsigned long                    MaxConnections;            // +0x04
    bool                             Paused;                    // +0x08
    unsigned char                    ID_[0x2];                  // +0x0a unsigned short (GetID)
    std::list<LSocket*>              ListOfAcceptedConnections; // +0x0c
    std::list<LSocket*>              ListOfConnectedSockets;    // +0x10
    std::list<DatagrammPacket*>      ListOfUDPDatagramms;       // +0x14
    unsigned char                    UDPListenerAddress_[0x1c]; // +0x18 LAddress UDPListenerAddress
};
FABLE_STATIC_ASSERT(sizeof(LSocketServer) == 0x34);

void LSocketServer::CloseSocket(LSocket* socket)
{
    std::list<LSocket*>::iterator it = std::find(ListOfConnectedSockets.begin(), ListOfConnectedSockets.end(), socket);
    ListOfConnectedSockets.remove(*it);
}
}