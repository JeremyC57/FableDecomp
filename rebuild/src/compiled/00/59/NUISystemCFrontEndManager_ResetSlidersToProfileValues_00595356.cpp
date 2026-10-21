// NUISystem::CFrontEndManager::ResetSlidersToProfileValues() -- re-read every slider and text slider on the top screen from the profile
// Leaf: ?ResetSlidersToProfileValues@CFrontEndManager@NUISystem@@QAEXXZ
// Retail: Fable.exe @ 0x00595356, 140 bytes, plain `ret`; one caller 0x00421159.
// Body: a local std::list<CComponent*> (ctor 0x0042ABAF with a default allocator temporary, clear 0x0042A18F, dtor
// 0x0042ABCA); m_stackScreens.top() (deque::back 0x0059B039) ->FindAllChildrenOfType(0x0f, list) (CComponent slot 6,
// +0x18, Ego_r.pdb ?FindAllChildrenOfType@CComponent@NUISystem@@UAEXW4ET...); each non-null entry gets
// CSlider::AdjustValueToProfile (0x0054A6F0, CSlider code range by RTTI); clear; the same with type 0x10 and
// CTextSlider::AdjustValueToProfile (0x00548B50, CTextSlider range). Both callee names are Ego_r.pdb QAEXXZ symbols.
// Layout: retail CFrontEndManager m_stackScreens at +0x20 (as the matched 0x005952D8); STLport list = one node pointer.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
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