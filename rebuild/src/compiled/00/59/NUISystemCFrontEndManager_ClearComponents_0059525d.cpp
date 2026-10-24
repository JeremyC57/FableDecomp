// NUISystem::CFrontEndManager::ClearComponents() -- empty the screen stack, delete every registered screen component and clear the screen map
// Leaf: ClearComponents@CFrontEndManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059525D, 70 bytes, plain `ret` via the tail `jmp` (thiscall, no arguments).
// Name: FableWin.pdb `?ClearComponents@CFrontEndManager@NUISystem@@QAEXXZ`; no direct callers in the image.
// Callees: deque<CComponent*>::pop_back() @ 0x0059AEE5 (the inline stack::pop), the STLport tree successor
// `_Rb_global<bool>::_M_increment` @ 0x004292C0 (`__cdecl`), each component's scalar deleting destructor (vtable
// slot 0 with 1), map<FRONTEND_SCREEN, CComponent*>::clear() @ 0x0059AF2A (the landed "CTCCarryable::OnKill" is
// that `_Rb_tree::clear`), reached by a tail `jmp`.
// Label: the catalog "CCreatureAction_FireMissileWeapon_Finalize" is propagated and wrong.
// Body: `while (!m_stackScreens.empty()) m_stackScreens.pop(); for (it = m_mapScreens.begin(); it !=
// m_mapScreens.end(); ++it) delete it->second; m_mapScreens.clear();`.
// Shape: the stack in esi (`empty()` compares the two cursors `[esi+10h]` / `[esi]`, the loop entered at its test),
// the map in edi (end() re-read each pass, the node value at +0x14). Size TU (`#pragma optimize("s", on)`), frameless.
// Layout: retail CFrontEndManager m_stackScreens +0x20 (STLport deque, 0x28 bytes), m_vComponents +0x48, m_mapScreens
// +0x54 -- engine/CFrontEndManager.h carries the Ego_r.pdb (Dinkumware) layout, so the class is modelled locally, as
// the landed GotoNextScreen 0x00596763 does.
#pragma optimize("s", on)
#include <stddef.h>

namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent();                           // slot 0: the scalar deleting destructor
};
}

enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };

namespace std {
template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
};
template <class T> class deque {
public:
    _Deque_iterator<T> _M_start;                     // +0x00
    _Deque_iterator<T> _M_finish;                    // +0x10
    T**                _M_map;                       // +0x20
    size_t             _M_map_size;                  // +0x24
    bool empty() const { return _M_finish._M_cur == _M_start._M_cur; }
    void pop_back();                                 // 0x0059AEE5
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    bool empty() const { return c.empty(); }
    void pop() { c.pop_back(); }
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* x);   // 0x004292C0
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                // +0x10
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    V* operator->() const { return &static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};
template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                   // +0x00
    size_t              _M_node_count;               // +0x04
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void clear();                                    // 0x0059AF2A
};
}

namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char                                   _pad00[0x20];     // vtable, MediaPlayerErrorString, ...
    std::stack<CComponent*>                         m_stackScreens;   // +0x20
    std::vector<CComponent*>                        m_vComponents;    // +0x48
    std::map<FRONTEND_SCREEN, CComponent*>          m_mapScreens;     // +0x54
    void ClearComponents();
};

void CFrontEndManager::ClearComponents()
{
    while (!m_stackScreens.empty())
        m_stackScreens.pop();
    for (std::map<FRONTEND_SCREEN, CComponent*>::iterator it = m_mapScreens.begin(); it != m_mapScreens.end(); ++it)
        delete it->second;
    m_mapScreens.clear();
}
}