// NUISystem::CComponent::CComponent(long defIndex) -- builds the UI component: CUIInterface/CObserver/CActionParam(1) bases, empty name, state map, child vectors, deletion and action lists and position-children set, zeroed geometry, colours and timers, null parents, stores the def index, reads the def (GetDataFromDef) and then clears the three transition times
// Leaf: ??0CComponent@NUISystem
// Retail: Fable.exe (TLC, VC7.1) @ 0x005334A0, 629 bytes (640 with padding), `ret 4`, no EH frame.
// Callees: NUISystem::CObserver::CObserver 0x0052D9E0 (catalog "CFrontEndManager::Initialize" is wrong),
//   CCharString::CCharString() 0x0099E4B0, malloc 0x00BFEA0E (the STLport node allocator: map header 0x38, list nodes
//   0x0C / 0x10, set header 0x14), NUISystem::CComponent::GetDataFromDef 0x005331A0.
// Vtables: CUIInterface 0x01246044, CActionParam 0x0122F83C, then CComponent's own 0x0124608C (+0), 0x01246064 (+4, the
//   CObserver base), 0x0124605C (+0x18, the CActionParam base) -- RTTI base offsets 0 / 4 / 0x18.
// Layout: local model -- NUISystem::CComponent has no engine header (engine/_quarantine/CComponent.h is another class);
//   retail is 0x134 bytes, Ego_r.pdb 0x15c: the STLport map/set/vector are 12 bytes and the lists 4 (PDB 16/16/12), so
//   Name +0x20, States +0x24, Time +0x30, Position..RenderZoom +0x34..+0x80, colours +0x84..+0x94, timers +0x98..+0xac,
//   Children +0xb0, ChildrenToDelete +0xbc, PParent +0xc8, Size +0xcc, Deletion {Method +0xd4, list +0xd8},
//   PParentViewport +0xdc, ActionsIAmParamOf +0xe0, ShapeChildren +0xe4, the relative vectors +0xf0..+0x114,
//   PPositionParent +0x118, PositionChildren +0x11c, PActionsOnDestruction +0x128, Flags/Type/FlagsSetTwo/Layer
//   +0x12c..+0x12f, DefIndex +0x130 (the same offsets as the landed CComponent::Initialise 0x005339B0).
// Shape: members zeroed in the mem-initializer list (Time, Size, Type and Deletion.Method are not); PParent, Deletion.Method
//   and PParentViewport are cleared in the body (stored after DefIndex). CRGBColour's ctor assigns in its BODY in R, G, B, A
//   order (mem-initializers run in B,G,R,A declaration order); the STLport list ctor links a LOCAL node before storing it
//   (retail stores the member after next/prev, no reload), while the _Rb_tree header goes through the member (reloaded).
#include <stdlib.h>
namespace std {
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Identity {};
template <class P> struct _Select1st {};
template <class A, class B> struct pair { A first; B second; };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;
};
template <class V, class A> class _Rb_tree_base {
public:
    _Rb_tree_node<V>* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>)); }
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    size_t _M_node_count;
    C      _M_key_compare;
    _Rb_tree() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    _Rb_tree<K, K, _Identity<K>, C, A> _M_t;
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    _Rb_tree<K, pair<const K, T>, _Select1st<pair<const K, T> >, C, A> _M_t;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T, class A> class _List_base {
public:
    _List_node<T>* _M_node;
    _List_base() : _M_node(0)
    {
        _List_node<T>* n = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node = n;
    }
};
template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
};
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
}

class CCharString { public:
    void* m_pData;
    CCharString();                          // 0x0099E4B0
};

namespace NUISystem {
enum EEvent { EVENT_NONE };
enum EActionParamType { ACTION_PARAM_COMPONENT = 1 };
enum EDeletionMethod { DELETION_METHOD_NONE };

struct C2DVector {
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct CRGBColour {                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CUIState { public:                           // PDB 0x24 bytes
    unsigned long StateChangeFlag;
    long          StateChangeType;
    C2DVector     Position;
    C2DVector     Zoom;
    unsigned long Colour;
    float         UpdateTime;
    bool          LinearChange;
};
class CComponent;
class CAction;
template <class T> struct CCountedPointer { T* Data; void* Info; };

class CUIInterface {                        // vtable 0x01246044
public:
    virtual ~CUIInterface();
};
class CObserver {                           // vtable 0x01246020, 0x14 bytes in retail
public:
    CObserver();                            // 0x0052D9E0
    virtual ~CObserver();
    std::set<EEvent> EventsToObserve;       // +0x04
    bool             PreventObservation;    // +0x10
};
class CActionParam {                        // vtable 0x0122F83C
public:
    CActionParam(EActionParamType type) : ActionParamType(type) {}
    virtual ~CActionParam();
    EActionParamType ActionParamType;       // +0x04
};
class CDeletion { public:
    EDeletionMethod         Method;               // +0x00
    std::list<CComponent*>  AssociatedParents;    // +0x04
};

class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    CComponent(long defIndex);
    virtual ~CComponent();
    void GetDataFromDef();                  // 0x005331A0

    CCharString Name;                                       // +0x20
    std::map<unsigned long, CUIState> States;               // +0x24
    float       Time;                                       // +0x30
    C2DVector   Position;                                   // +0x34
    C2DVector   TargetPosition;                             // +0x3c
    C2DVector   InitialPosition;                            // +0x44
    C2DVector   ParentPosition;                             // +0x4c
    C2DVector   RenderPosition;                             // +0x54
    C2DVector   Zoom;                                       // +0x5c
    C2DVector   TargetZoom;                                 // +0x64
    C2DVector   InitialZoom;                                // +0x6c
    C2DVector   ParentZoom;                                 // +0x74
    C2DVector   RenderZoom;                                 // +0x7c
    CRGBColour  Colour;                                     // +0x84
    CRGBColour  TargetColour;                               // +0x88
    CRGBColour  InitialColour;                              // +0x8c
    CRGBColour  ParentColour;                               // +0x90
    CRGBColour  RenderColour;                               // +0x94
    float       PositionTimeElapsed;                        // +0x98
    float       PositionTime;                               // +0x9c
    float       ZoomTimeElapsed;                            // +0xa0
    float       ZoomTime;                                   // +0xa4
    float       ColourTimeElapsed;                          // +0xa8
    float       ColourTime;                                 // +0xac
    std::vector<CCountedPointer<CComponent> > Children;         // +0xb0
    std::vector<CCountedPointer<CComponent> > ChildrenToDelete; // +0xbc
    CComponent* PParent;                                    // +0xc8
    C2DVector   Size;                                       // +0xcc
    CDeletion   Deletion;                                   // +0xd4
    CComponent* PParentViewport;                            // +0xdc
    std::list<CCountedPointer<CAction> > ActionsIAmParamOf; // +0xe0
    std::vector<unsigned long> ShapeChildren;               // +0xe4
    C2DVector   RenderPositionSave;                         // +0xf0
    C2DVector   RelativeRenderPosition;                     // +0xf8
    C2DVector   RelativeParentPosition;                     // +0x100
    C2DVector   RelativeRenderZoom;                         // +0x108
    C2DVector   RelativeParentZoom;                         // +0x110
    CComponent* PPositionParent;                            // +0x118
    std::set<CComponent*> PositionChildren;                 // +0x11c
    std::list<CCountedPointer<CAction> >* PActionsOnDestruction;   // +0x128
    unsigned char Flags;                                    // +0x12c
    unsigned char Type;                                     // +0x12d
    unsigned char FlagsSetTwo;                              // +0x12e
    char        Layer;                                      // +0x12f
    long        DefIndex;                                   // +0x130
};

CComponent::CComponent(long defIndex)
    : CActionParam(ACTION_PARAM_COMPONENT),
      Position(0.0f, 0.0f), TargetPosition(0.0f, 0.0f), InitialPosition(0.0f, 0.0f), ParentPosition(0.0f, 0.0f),
      RenderPosition(0.0f, 0.0f), Zoom(0.0f, 0.0f), TargetZoom(0.0f, 0.0f), InitialZoom(0.0f, 0.0f),
      ParentZoom(0.0f, 0.0f), RenderZoom(0.0f, 0.0f),
      Colour(0, 0, 0, 0), TargetColour(0, 0, 0, 0), InitialColour(0, 0, 0, 0), ParentColour(0, 0, 0, 0),
      RenderColour(0, 0, 0, 0),
      PositionTimeElapsed(0.0f), PositionTime(0.0f), ZoomTimeElapsed(0.0f), ZoomTime(0.0f),
      ColourTimeElapsed(0.0f), ColourTime(0.0f),
      RenderPositionSave(0.0f, 0.0f), RelativeRenderPosition(0.0f, 0.0f), RelativeParentPosition(0.0f, 0.0f),
      RelativeRenderZoom(0.0f, 0.0f), RelativeParentZoom(0.0f, 0.0f),
      PPositionParent(0), PActionsOnDestruction(0), Flags(0), FlagsSetTwo(0), Layer(0), DefIndex(defIndex)
{
    Deletion.Method = DELETION_METHOD_NONE;
    PParent = 0;
    PParentViewport = 0;
    GetDataFromDef();
    PositionTime = 0.0f;
    ZoomTime = 0.0f;
    ColourTime = 0.0f;
}
}