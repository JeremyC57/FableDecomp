// NUISystem::CComponent::~CComponent() -- detach the component: drop it from every action that holds it as a parameter, unregister the observer, leave the parent viewport and position parent, orphan the position children, hand the destruction actions to the front-end manager and delete them, orphan the children, then destroy the members and the CActionParam / CObserver / CUIInterface bases
// Leaf: ??1CComponent@NUISystem
// Retail 0x00532D90, 1040 bytes, plain `ret`, one dword of frame (the destroy-range tag).
// Callees: CFrontEndManager::GetInstance 0x0041E5F2 (`__fastcall`) slots 5 (+0x14, RemoveObserver) and 14 (+0x38, take a
//   destruction action); free 0x00BFEA14, operator delete 0x00BFE9BC; list<CCountedPointer<CAction> >::clear 0x005356A0;
//   _Destroy_range<CCountedPointer> 0x00535420 (`__fastcall`); the tree erasers 0x00534C60 (set<CComponent*>),
//   0x0042A1B6 (map<unsigned long, CUIState>), 0x0052DCA0 (set<EEvent>); ~CCharString 0x0099EAE0.
//   CComponent virtual slots 48 (+0xc0), 51 (+0xcc, SetParent), 52 (+0xd0, GetParent), 89 (+0x164), 127 (+0x1fc),
//   128 (+0x200).
// Vtables 0x0124608C / 0x01246064 / 0x0124605C (+0 / +4 / +0x18), then CActionParam 0x0122F83C, CObserver 0x01246020,
//   CUIInterface 0x01246044.
// Layout: the landed CComponent ctor 0x005334A0 model (retail 0x134 bytes; no engine header for NUISystem::CComponent).
//   CAction per Ego_r.pdb with 4-byte lists: UIActionParams +4, ThingActionParams +8, Valid +0xc.
// Shape: the tree iterator steps through a static node-in/node-out increment (a member-walking operator++ keeps the
//   node in the loop register); the destruction-action list is read once into a local; `this == child->GetParent()`;
//   the destroy-range tag is a named uninitialised local (a `__false_type()` temporary is zero-stored).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14
void operator delete(void* p);                   // 0x00BFE9BC

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T> struct CPodList {             // STLport list<T>, T trivially destructible
    _List_node<T>* _M_node;
    _List_iterator<T> begin() const { return _List_iterator<T>(_M_node->_M_next); }
    _List_iterator<T> end() const { return _List_iterator<T>(_M_node); }
    void erase(_List_iterator<T> pos)
    {
        _List_node<T>* next = pos._M_node->_M_next;
        _List_node<T>* prev = pos._M_node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(pos._M_node);
    }
    void remove(const T& value)
    {
        _List_iterator<T> first = begin();
        _List_iterator<T> last = end();
        while (first != last) {
            _List_iterator<T> next = first;
            ++next;
            if (value == *first)
                erase(first);
            first = next;
        }
    }
    void clear()
    {
        _List_node<T>* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CPodList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
template <class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
template <class T> struct CRetailList {          // STLport list<CCountedPointer<T> >
    _List_node<CCountedPointer<T> >* _M_node;
    _List_iterator<CCountedPointer<T> > begin() const { return _List_iterator<CCountedPointer<T> >(_M_node->_M_next); }
    _List_iterator<CCountedPointer<T> > end() const { return _List_iterator<CCountedPointer<T> >(_M_node); }
    void clear();                                // 0x005356A0
    ~CRetailList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct __false_type {};
struct CCountedPointerAny { void* Data; void* Info; };
void __fastcall DestroyRange_00535420(CCountedPointerAny* first, CCountedPointerAny* last, const __false_type& tag);
template <class T> struct CCountedVector {       // STLport vector<CCountedPointer<T> >
    CCountedPointer<T>* _M_start;
    CCountedPointer<T>* _M_finish;
    CCountedPointer<T>* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CCountedPointer<T>& operator[](unsigned long i) { return *(_M_start + i); }
    ~CCountedVector()
    {
        __false_type tag;
        DestroyRange_00535420((CCountedPointerAny*)_M_start, (CCountedPointerAny*)_M_finish, tag);
        if (_M_start)
            free(_M_start);
    }
};
template <class T> struct CPodVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CPodVector()
    {
        if (_M_start)
            free(_M_start);
    }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) {}
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    bool operator!=(const _Rb_tree_iterator& x) const { return _M_node != x._M_node; }
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        } else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
    _Rb_tree_iterator& operator++()
    {
        _M_node = _M_increment(_M_node);
        return *this;
    }
};
template <class V, int Tag> struct CRetailTree {  // STLport _Rb_tree {header, count}
    _Rb_tree_node<V>* _M_header;
    unsigned long     _M_node_count;
    bool              _M_key_compare;
    _Rb_tree_iterator<V> begin() const { return _Rb_tree_iterator<V>(_M_header->_M_left); }
    _Rb_tree_iterator<V> end() const { return _Rb_tree_iterator<V>(_M_header); }
    void _M_erase_out(_Rb_tree_node<V>* x);      // the out-of-line recursive eraser (per instantiation)
    void _M_erase(_Rb_tree_node<V>* x)
    {
        while (x != 0) {
            _M_erase_out((_Rb_tree_node<V>*)x->_M_right);
            _Rb_tree_node<V>* y = (_Rb_tree_node<V>*)x->_M_left;
            free(x);
            x = y;
        }
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Rb_tree_node<V>*)_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    ~CRetailTree()
    {
        clear();
        if (_M_header)
            free(_M_header);
    }
};

class CCharString { public:
    void* m_pData;
    ~CCharString();                              // 0x0099EAE0
};

namespace NUISystem {
enum EEvent { EVENT_NONE };
enum EDeletionMethod { DELETION_METHOD_NONE };
struct C2DVector { float X, Y; };
struct CStatePair { unsigned long first; struct { unsigned long a[8]; bool b; } second; };
class CUIState { public:
    unsigned long StateChangeFlag;
    long          StateChangeType;
    C2DVector     Position;
    C2DVector     Zoom;
    unsigned long Colour;
    float         UpdateTime;
    bool          LinearChange;
};
class CComponent;
class CActionParam;
class CAction { public:
    long                    ActionType;          // +0x00
    CPodList<CActionParam*> UIActionParams;      // +0x04
    CPodList<long>          ThingActionParams;   // +0x08
    bool                    Valid;               // +0x0c
};
class CUIInterface {                             // vtable 0x01246044
public:
    virtual ~CUIInterface() {}
};
class CObserver {                                // vtable 0x01246020
public:
    virtual ~CObserver() {}
    CRetailTree<EEvent, 0> EventsToObserve;      // +0x04 (eraser 0x0052DCA0)
    bool                   PreventObservation;   // +0x10
};
class CActionParam {                             // vtable 0x0122F83C
public:
    virtual ~CActionParam() {}
    long ActionParamType;                        // +0x04
};
class CDeletion { public:
    EDeletionMethod        Method;               // +0x00
    CPodList<CComponent*>  AssociatedParents;    // +0x04
};
}
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void RemoveObserver(NUISystem::CObserver* observer);                          // slot 5 (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual void AddDestructionAction(CCountedPointer<NUISystem::CAction>& action);        // slot 14 (+0x38)
};
namespace NUISystem {
#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    virtual ~CComponent();                                       // slot 0
    VPAD32(p0) VPAD8(p1) VPAD4(p2) VPAD2(p3) VPAD1(p4)           // slots 1..47
    virtual void SetDeleted(long method);                        // slot 48 (+0xc0)
    VPAD2(p5)                                                    // slots 49..50
    virtual void SetParent(CComponent* parent);                  // slot 51 (+0xcc)
    virtual CComponent* GetParent();                             // slot 52 (+0xd0)
    VPAD32(p6) VPAD4(p7)                                         // slots 53..88
    virtual void RemoveViewportChild(CComponent* child);         // slot 89 (+0x164)
    VPAD32(p8) VPAD4(p9) VPAD1(pa)                               // slots 90..126
    virtual void RemovePositionChild(CComponent* child);         // slot 127 (+0x1fc)
    virtual void RemovePositionParent(CComponent* parent);       // slot 128 (+0x200)

    CCharString Name;                                            // +0x20
    CRetailTree<CStatePair, 1> States;                           // +0x24 (eraser 0x0042A1B6)
    float       Time;                                            // +0x30
    C2DVector   Position[10];                                    // +0x34 Position .. RenderZoom
    unsigned long Colours[5];                                    // +0x84
    float       Timers[6];                                       // +0x98
    CCountedVector<CComponent> Children;                         // +0xb0
    CCountedVector<CComponent> ChildrenToDelete;                 // +0xbc
    CComponent* PParent;                                         // +0xc8
    C2DVector   Size;                                            // +0xcc
    CDeletion   Deletion;                                        // +0xd4
    CComponent* PParentViewport;                                 // +0xdc
    CRetailList<CAction> ActionsIAmParamOf;                      // +0xe0
    CPodVector<unsigned long> ShapeChildren;                     // +0xe4
    C2DVector   Relative[5];                                     // +0xf0
    CComponent* PPositionParent;                                 // +0x118
    CRetailTree<CComponent*, 2> PositionChildren;                // +0x11c (eraser 0x00534C60)
    CRetailList<CAction>* PActionsOnDestruction;                 // +0x128
    unsigned char Flags;                                         // +0x12c
    unsigned char Type;                                          // +0x12d
    unsigned char FlagsSetTwo;                                   // +0x12e
    char        Layer;                                           // +0x12f
    long        DefIndex;                                        // +0x130
};

CComponent::~CComponent()
{
    for (_List_iterator<CCountedPointer<CAction> > it = ActionsIAmParamOf.begin(); it != ActionsIAmParamOf.end(); ++it) {
        (*it)->UIActionParams.remove(this);
        (*it)->Valid = false;
    }
    CFrontEndManager::GetInstance()->RemoveObserver(this);
    if (PParentViewport)
        PParentViewport->RemoveViewportChild(this);
    if (PPositionParent)
        PPositionParent->RemovePositionChild(this);
    for (_Rb_tree_iterator<CComponent*> pc = PositionChildren.begin(); pc != PositionChildren.end(); ++pc)
        (*pc)->RemovePositionParent(this);
    CRetailList<CAction>* actions = PActionsOnDestruction;
    if (actions) {
        for (_List_iterator<CCountedPointer<CAction> > it = actions->begin(); it != actions->end(); ++it)
            CFrontEndManager::GetInstance()->AddDestructionAction(*it);
    }
    delete PActionsOnDestruction;
    for (unsigned long i = 0; i < Children.size(); ++i) {
        if (this == Children[i]->GetParent())
            Children[i]->SetParent(0);
    }
    for (unsigned long i = 0; i < ChildrenToDelete.size(); ++i) {
        if (this == ChildrenToDelete[i]->GetParent()) {
            ChildrenToDelete[i]->SetParent(0);
            if (ChildrenToDelete[i]->Deletion.Method == DELETION_METHOD_NONE)
                ChildrenToDelete[i]->SetDeleted(0);
        }
    }
}
}