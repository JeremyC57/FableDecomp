// NUISystem::CFrontEndManager::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle) -- draw every registered front-end screen
// Leaf: ?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z
// Retail: Fable.exe @ 0x00595222, 57 bytes, `ret 8`; one caller (0x0042E091); Ego_r.pdb
// ?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z.
// Body: walks m_mapScreens (STLport map, header node pointer at +0x54; begin = header->_M_left, end re-read every pass;
// ++ is the static _Rb_global<bool>::_M_increment 0x004292C0, cdecl) and, for each non-null screen (node value +0x14),
// calls CComponent::Draw(engine, handle, 0, 0, 0) (vtable slot 2; Ego_r.pdb
// ?Draw@CComponent@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAV12@@Z).
// Layout: retail CFrontEndManager m_mapScreens +0x54 as modelled at 0x005966F2; no engine/ header.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
class CIEngine;
class CEnginePrimitiveHandle;
namespace std {
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class T> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x);   // 0x004292C0
};
template <class K, class V> struct pair {
    K first;
    V second;
};
template <class K, class V> class map {
public:
    typedef pair<const K, V> value_type;
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* x) : _M_node(x) {}
        value_type* operator->() const { return &static_cast<_Rb_tree_node<value_type>*>(_M_node)->_M_value_field; }
        iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    unsigned int        _M_key_compare;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};
}
namespace NUISystem {
enum EFrontEndScreen { FRONTEND_SCREEN_NONE };
class CComponent {
public:
    virtual ~CComponent();
    virtual void _slot01();
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* handle, long a, long* b, CComponent* c);   // slot 2
};
class CFrontEndManager {
public:
    unsigned char                            _pad00[0x54];
    std::map<EFrontEndScreen, CComponent*>   m_mapScreens;   // +0x54
    void Draw(CIEngine& engine, CEnginePrimitiveHandle* handle);
};
void CFrontEndManager::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle)
{
    for (std::map<EFrontEndScreen, CComponent*>::iterator it = m_mapScreens.begin(); it != m_mapScreens.end(); ++it)
        if (it->second)
            it->second->Draw(engine, handle, 0, 0, 0);
}
}