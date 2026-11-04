// NUISystem::CComponent::`vector deleting destructor'(unsigned int) [thunk: this-=0x18] -- adjustor thunk: delete a UI component through its CActionParam base subobject
// Leaf: ??_ECComponent@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00535ac0, 8 bytes: `sub ecx,0x18 ; jmp 0x00535C10` (??_ECComponent@NUISystem@@UAEPAXI@Z).
// Identity (two sources): slot 0 of the CComponent vtable for the CActionParam base at +0x18 (vtable 0x0124605C) is this thunk, and its jump target 0x00535C10 is
//   slot 0 of the primary CComponent vtable 0x0124608C (the deleting destructor); VC7.1 spells the adjustor{24} thunk ??_ECComponent@NUISystem@@WBI@AEPAXI@Z.
//   The catalog's "vfunc_0" was a placeholder.
// TU: the CComponent vtable (and so both adjustor thunks) is emitted wherever the class's ctor or dtor is defined; the body below is
//   the landed NUISystem::CComponent::CComponent(long) 0x005334A0 source (graded there, not here). Only the thunk is this row.
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
    private:
    void _M_empty_initialize()
    {
        this->_M_header->_M_color = false;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
    public:
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