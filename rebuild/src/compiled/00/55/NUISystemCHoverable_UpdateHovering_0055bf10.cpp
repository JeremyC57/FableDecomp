// NUISystem::CHoverable::UpdateHovering() -- per-frame hover tracking: read the mouse box from the front end, and when this component becomes hovered (and no other component claims the mouse) unhover every lower-priority hovered component, mark this one hovered and append it to the global hover list; unhover it when the mouse leaves
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055BF10, 594 bytes.
#include <new>

extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E

namespace NUISystem {
struct C2DVector { float X, Y; };
struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMouse {
    virtual void m000(); virtual void m001(); virtual void m002(); virtual void m003(); virtual void m004();
    virtual void m005(); virtual void m006(); virtual void m007(); virtual void m008(); virtual void m009();
    virtual void m010(); virtual void m011(); virtual void m012(); virtual void m013(); virtual void m014();
    virtual void m015();
    virtual C2DVector GetPosition();                 // slot 16 (+0x40)
    virtual void m017(); virtual void m018(); virtual void m019(); virtual void m020(); virtual void m021(); virtual void m022();
    virtual C2DVector GetSize();                     // slot 23 (+0x5C)
};
class CManager { public:
    void*         __vftable;
    unsigned char _pad04[0xa0];
    bool          FlagA4;                            // +0xA4 (restoring)
    unsigned char _pad_0xa5[0x13];
    CMouse*       PMouse;                            // +0xB8
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
    CMouse* GetMouse() const { return PMouse; }
};
template <class T> struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    T          _M_data;
};
template <class T> struct CRetailList {              // STLport list: one word, the sentinel node
    struct iterator {
        SListNode<T>* _M_node;
        iterator(SListNode<T>* n) : _M_node(n) {}
        iterator(const iterator& o) : _M_node(o._M_node) {}
        T& operator*() const { return _M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& o) const { return _M_node != o._M_node; }
    };
    SListNode<T>* _M_node;
    CRetailList(const CRetailList& other);           // 0x0055C4C0
    ~CRetailList();                                  // 0x0055C300
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    SListNode<T>* _M_create_node(const T& x)
    {
        SListNode<T>* node = (SListNode<T>*)malloc(sizeof(SListNode<T>));
        new (&node->_M_data) T(x);
        return node;
    }
    void insert(iterator position, const T& x)
    {
        SListNode<T>* node = _M_create_node(x);
        SListNode<T>* n = position._M_node;
        SListNode<T>* previous = n->_M_prev;
        node->_M_next = n;
        node->_M_prev = previous;
        previous->_M_next = node;
        n->_M_prev = node;
    }
    void push_back(const T& x) { insert(end(), x); }
};
class CComponent;
class CHoverable;
extern CRetailList<CHoverable*> g_HoverList;         // 0x013B8AD4
bool __fastcall GIsAnyAlreadyHovered(C2DBoxI* mouse, CComponent* component);   // 0x0055BB40
class CObserver { public:
    virtual void o00(); virtual void o01();
    virtual bool IsListeningTo(long event);          // slot 2 (+0x08)
};
struct CComponentBase {
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
    virtual long GetHoverPriority();                 // slot 129 (+0x204)
    virtual void c130(); virtual void c131();
    virtual bool CanBeHovered();                     // slot 132 (+0x210)
    virtual void HoverStateChanged();                // slot 133 (+0x214)
    virtual void c134(); virtual void c135(); virtual void c136(); virtual void c137(); virtual void c138();
    virtual void c139(); virtual void c140(); virtual void c141();
    virtual bool IsHovered(C2DBoxI& mouse);          // slot 142 (+0x238)
    virtual void OnHovered();                        // slot 143 (+0x23C)
    virtual void OnUnhovered();                      // slot 144 (+0x240)
};
class CHoverable : public CComponentBase, public CObserver { public:
    unsigned char _pad_0x08[0x158];
    bool          Hovered;                           // +0x160
    void UpdateHovering();
};

void CHoverable::UpdateHovering()
{
    if (CManager::GetInstance()->FlagA4)
        return;
    if (!IsListeningTo(0x19))
        return;
    C2DBoxI mouse;
    if (CManager::GetInstance()->GetMouse()) {
        mouse.TLX = (long)CManager::GetInstance()->GetMouse()->GetPosition().X;
        mouse.TLY = (long)CManager::GetInstance()->GetMouse()->GetPosition().Y;
        CMouse* pointer = CManager::GetInstance()->GetMouse();
        mouse.BRX = (long)(pointer->GetPosition().X + CManager::GetInstance()->GetMouse()->GetSize().X);
        pointer = CManager::GetInstance()->GetMouse();
        mouse.BRY = (long)(pointer->GetPosition().Y + CManager::GetInstance()->GetMouse()->GetSize().Y);
    }
    if (!Hovered) {
        if (IsHovered(mouse) && !GIsAnyAlreadyHovered(&mouse, (CComponent*)this)) {
            CRetailList<CHoverable*> hovered(g_HoverList);
            for (CRetailList<CHoverable*>::iterator it = hovered.begin(); it != hovered.end(); ++it) {
                CHoverable* other = *it;
                if (CanBeHovered() && other->CanBeHovered() && other->IsHovered(mouse)
                    && GetHoverPriority() < other->GetHoverPriority()) {
                    other->OnUnhovered();
                    other->Hovered = false;
                    other->HoverStateChanged();
                }
            }
            OnHovered();
            Hovered = true;
            HoverStateChanged();
            g_HoverList.push_back(this);
            return;
        }
    }
    if (Hovered && !IsHovered(mouse)) {
        OnUnhovered();
        Hovered = false;
        HoverStateChanged();
    }
}
}