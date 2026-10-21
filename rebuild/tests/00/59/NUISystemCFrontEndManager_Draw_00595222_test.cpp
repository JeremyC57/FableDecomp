#include <cstdio>
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

#include <cstdlib>
static int g_draws = 0; static bool g_ok = true;
static CIEngine* g_engine = (CIEngine*)0x1234; static CEnginePrimitiveHandle* g_handle = (CEnginePrimitiveHandle*)0x5678;
namespace NUISystem {
CComponent::~CComponent() {}
void CComponent::_slot01() {}
void CComponent::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle, long a, long* b, CComponent* c)
{ ++g_draws; if (&engine != g_engine || handle != g_handle || a || b || c) g_ok = false; }
}
// simple in-order successor for a right-leaning chain (each node's _M_right is the next, last points at header)
template <> std::_Rb_tree_node_base* std::_Rb_global<bool>::_M_increment(std::_Rb_tree_node_base* x) { return x->_M_right; }
typedef std::_Rb_tree_node<std::map<NUISystem::EFrontEndScreen, NUISystem::CComponent*>::value_type> Node;
int main()
{
    using namespace NUISystem;
    static CFrontEndManager m; static std::_Rb_tree_node_base header; static CComponent c1, c3; Node* n = (Node*)std::calloc(3, sizeof(Node));
    m.m_mapScreens._M_header = &header;
    header._M_left = &n[0]; n[0]._M_right = &n[1]; n[1]._M_right = &n[2]; n[2]._M_right = &header;
    n[0]._M_value_field.second = &c1; n[1]._M_value_field.second = 0; n[2]._M_value_field.second = &c3;
    m.Draw(*g_engine, g_handle);
    bool ok = g_ok && g_draws == 2;
    header._M_left = &header; g_draws = 0; m.Draw(*g_engine, g_handle);
    ok = ok && g_draws == 0;
    if (ok) std::printf("00595222_TEST PASS\n");
    return 0;
}