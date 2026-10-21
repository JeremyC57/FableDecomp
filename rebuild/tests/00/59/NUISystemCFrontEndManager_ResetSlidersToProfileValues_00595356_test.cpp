#include <cstdio>
namespace std {
template <class T> class allocator {
public:
    allocator() {}
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;
    explicit list(const A& a = A());                     // 0x0042ABAF
    ~list();                                             // 0x0042ABCA
    void clear();                                        // 0x0042A18F
    iterator begin() { iterator i; i._M_node = _M_node->_M_next; return i; }
    iterator end() { iterator i; i._M_node = _M_node; return i; }
};
template <class T> class deque {
public:
    unsigned char _body[0x28];
    T& back();                                           // 0x0059B039 for CComponent*
};
template <class T, class C = deque<T> > class stack {
public:
    C c;
    T& top() { return c.back(); }
};
}
namespace NUISystem {
class CComponent {
public:
    enum EType { ETYPE_SLIDER = 0x0f, ETYPE_TEXT_SLIDER = 0x10 };   // values as pushed (names not recovered)
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005();
    virtual void FindAllChildrenOfType(EType type, std::list<CComponent*>& found);   // slot 6 (+0x18)
};
class CSlider : public CComponent {
public:
    void AdjustValueToProfile();                         // 0x0054A6F0
};
class CTextSlider : public CComponent {
public:
    void AdjustValueToProfile();                         // 0x00548B50
};
class CFrontEndManager {
public:
    unsigned char                  _pad00[0x20];
    std::stack<CComponent*>        m_stackScreens;       // +0x20 (Ego_r.pdb +0x2c)
    void ResetSlidersToProfileValues();
};
void CFrontEndManager::ResetSlidersToProfileValues()
{
    std::list<CComponent*> components;
    m_stackScreens.top()->FindAllChildrenOfType(CComponent::ETYPE_SLIDER, components);
    for (std::list<CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    {
        CSlider* slider = static_cast<CSlider*>(*it);
        if (slider)
            slider->AdjustValueToProfile();
    }
    components.clear();
    m_stackScreens.top()->FindAllChildrenOfType(CComponent::ETYPE_TEXT_SLIDER, components);
    for (std::list<CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    {
        CTextSlider* slider = static_cast<CTextSlider*>(*it);
        if (slider)
            slider->AdjustValueToProfile();
    }
}
}

void NUISystem::CComponent::_v000() {}
void NUISystem::CComponent::_v001() {}
void NUISystem::CComponent::_v002() {}
void NUISystem::CComponent::_v003() {}
void NUISystem::CComponent::_v004() {}
void NUISystem::CComponent::_v005() {}
typedef std::_List_node<NUISystem::CComponent*> Node;
template <class T, class A> std::list<T, A>::list(const A&) { _M_node = new Node; _M_node->_M_next = _M_node->_M_prev = _M_node; }
static int g_cleared, g_destroyed;
template <class T, class A> void std::list<T, A>::clear()
{
    ++g_cleared; Node* n = _M_node->_M_next;
    while (n != _M_node) { Node* x = n; n = n->_M_next; delete x; }
    _M_node->_M_next = _M_node->_M_prev = _M_node;
}
template <class T, class A> std::list<T, A>::~list() { clear(); delete _M_node; ++g_destroyed; }
static NUISystem::CComponent* g_top;
template <class T> T& std::deque<T>::back() { return *(T*)&g_top; }
static NUISystem::CComponent g_objs[6];
static int g_type[6], g_slider[6], g_text[6], g_queries[2];
static int IdOf(const void* p) { return int((const NUISystem::CComponent*)p - g_objs); }
void NUISystem::CComponent::FindAllChildrenOfType(EType type, std::list<CComponent*>& found)
{
    ++g_queries[type - 0x0f];
    bool empty = found._M_node->_M_next == found._M_node;
    if (!empty) return;   // must be cleared between the two queries
    Node* tailn = found._M_node;
    for (int i = 0; i < 6; ++i)
        if (g_type[i] == type)
        {
            Node* n = new Node; n->_M_data = &g_objs[i];
            n->_M_next = found._M_node; n->_M_prev = tailn; tailn->_M_next = n; found._M_node->_M_prev = n; tailn = n;
        }
    if (type == 0x0f) { Node* n = new Node; n->_M_data = 0; n->_M_next = found._M_node; n->_M_prev = tailn; tailn->_M_next = n; found._M_node->_M_prev = n; }
}
void NUISystem::CSlider::AdjustValueToProfile() { ++g_slider[IdOf(this)]; }
void NUISystem::CTextSlider::AdjustValueToProfile() { ++g_text[IdOf(this)]; }
int main()
{
    g_type[0] = 0x0f; g_type[1] = 0x10; g_type[2] = 0x0f; g_type[3] = 1; g_type[4] = 0x10; g_type[5] = 0x10;
    g_top = &g_objs[3];
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)];
    ((NUISystem::CFrontEndManager*)raw)->ResetSlidersToProfileValues();
    bool ok = g_queries[0] == 1 && g_queries[1] == 1 && g_cleared == 2 && g_destroyed == 1
           && g_slider[0] == 1 && g_slider[2] == 1 && g_slider[1] == 0 && g_text[1] == 1 && g_text[4] == 1 && g_text[5] == 1
           && g_text[0] == 0 && g_slider[3] == 0 && g_text[3] == 0;
    std::printf(ok ? "00595356_TEST PASS\n" : "00595356_TEST FAIL\n");
    return ok ? 0 : 1;
}