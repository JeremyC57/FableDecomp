// NUISystem::CHoverable::GIsAnyAlreadyHovered(C2DBoxI& mouse, CComponent* component) -- whether some component already in the global hovered list claims the mouse box against component: when both can be hovered, a hovered one under the mouse with priority not above component's; otherwise one under the mouse with the same priority
// Leaf: ?GIsAnyAlreadyHovered@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055BB40, 164 bytes (176 with padding), plain `ret` (Ego_r.pdb
//   `?GIsAnyAlreadyHovered@CHoverable@NUISystem@@KI_NAAVC2DBoxI@@PAVCComponent@2@@Z`: protected static __fastcall, the
//   box in ecx, the component in edx).
// Callees: component virtual slots 132 CanBeHovered (+0x210), 142 IsHovered(C2DBoxI&) (+0x238), 129 GetHoverPriority
//   (+0x204), named as in the landed CHoverable::UpdateHovering 0x0055BF10.
// Callers: CHoverable::UpdateHovering 0x0055BF10 (call 0x0055C037, landed).
// Label: catalogued as "GIsAnyAlreadyHovered" (unscoped); Ego_r.pdb scopes it to CHoverable.
// Data: CHoverable::GAlreadyHovered 0x013B8AD4 (STLport list<CHoverable*>, the sentinel pointer only; landed static
//   initialiser 0x0121A6D0).
// Shape: the list's begin is read through the global once, the end re-read from it after every pass (the virtual calls
//   may change it); `component->GetHoverPriority() >= hovered->GetHoverPriority()` (and the `==` form) -- VC7.1 calls the
//   right operand first and compares `cmp component, hovered`; writing `hovered <= component` swaps the cmp operands.
namespace NUISystem {
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
};

template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> class CRetailList {                  // STLport list: one word, the sentinel node
public:
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};

class CComponentRoot {                                  // the primary base (vptr at +0)
public:
    virtual void c000(); virtual void c001(); virtual void c002(); virtual void c003(); virtual void c004();
    virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013(); virtual void c014();
    virtual void c015(); virtual void c016(); virtual void c017(); virtual void c018(); virtual void c019();
    virtual void c020(); virtual void c021(); virtual void c022(); virtual void c023(); virtual void c024();
    virtual void c025(); virtual void c026(); virtual void c027(); virtual void c028(); virtual void c029();
    virtual void c030(); virtual void c031(); virtual void c032(); virtual void c033(); virtual void c034();
    virtual void c035(); virtual void c036(); virtual void c037(); virtual void c038(); virtual void c039();
    virtual void c040(); virtual void c041(); virtual void c042(); virtual void c043(); virtual void c044();
    virtual void c045(); virtual void c046(); virtual void c047(); virtual void c048(); virtual void c049();
    virtual void c050(); virtual void c051(); virtual void c052(); virtual void c053(); virtual void c054();
    virtual void c055(); virtual void c056(); virtual void c057(); virtual void c058(); virtual void c059();
    virtual void c060(); virtual void c061(); virtual void c062(); virtual void c063(); virtual void c064();
    virtual void c065(); virtual void c066(); virtual void c067(); virtual void c068(); virtual void c069();
    virtual void c070(); virtual void c071(); virtual void c072(); virtual void c073(); virtual void c074();
    virtual void c075(); virtual void c076(); virtual void c077(); virtual void c078(); virtual void c079();
    virtual void c080(); virtual void c081(); virtual void c082(); virtual void c083(); virtual void c084();
    virtual void c085(); virtual void c086(); virtual void c087(); virtual void c088(); virtual void c089();
    virtual void c090(); virtual void c091(); virtual void c092(); virtual void c093(); virtual void c094();
    virtual void c095(); virtual void c096(); virtual void c097(); virtual void c098(); virtual void c099();
    virtual void c100(); virtual void c101(); virtual void c102(); virtual void c103(); virtual void c104();
    virtual void c105(); virtual void c106(); virtual void c107(); virtual void c108(); virtual void c109();
    virtual void c110(); virtual void c111(); virtual void c112(); virtual void c113(); virtual void c114();
    virtual void c115(); virtual void c116(); virtual void c117(); virtual void c118(); virtual void c119();
    virtual void c120(); virtual void c121(); virtual void c122(); virtual void c123(); virtual void c124();
    virtual void c125(); virtual void c126(); virtual void c127(); virtual void c128();
    virtual long GetHoverPriority();                    // slot 129 (+0x204)
    virtual void c130(); virtual void c131();
    virtual bool CanBeHovered();                        // slot 132 (+0x210)
    virtual void c133(); virtual void c134(); virtual void c135(); virtual void c136(); virtual void c137();
    virtual void c138(); virtual void c139(); virtual void c140(); virtual void c141();
    virtual bool IsHovered(C2DBoxI& mouse);             // slot 142 (+0x238)
};
class CObserver {                                       // the second base (vptr at +4)
public:
    virtual void o00();
};
class CComponent : public CComponentRoot, public CObserver {
};

class CHoverable : public CComponent {
public:
    static CRetailList<CHoverable*> GAlreadyHovered;    // 0x013B8AD4
    static bool __fastcall GIsAnyAlreadyHovered(C2DBoxI& mouse, CComponent* component);
};

bool __fastcall CHoverable::GIsAnyAlreadyHovered(C2DBoxI& mouse, CComponent* component)
{
    for (CRetailList<CHoverable*>::iterator it = GAlreadyHovered.begin(); it != GAlreadyHovered.end(); ++it) {
        CHoverable* hovered = *it;
        if (hovered->CanBeHovered() && component->CanBeHovered()) {
            if (hovered->IsHovered(mouse) && component->GetHoverPriority() >= hovered->GetHoverPriority())
                return true;
        }
        else {
            if (hovered->IsHovered(mouse) && component->GetHoverPriority() == hovered->GetHoverPriority())
                return true;
        }
    }
    return false;
}
}