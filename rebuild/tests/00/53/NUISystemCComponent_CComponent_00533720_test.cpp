#include <stdio.h>
#include <string.h>
// NUISystem::CComponent::CComponent(const CComponent& other) -- copy constructor: builds a fresh UI component exactly as CComponent(long) does (CUIInterface/CObserver/CActionParam(1) bases, empty name, state map, child vectors, deletion and action lists, position-children set, zeroed geometry, colours and timers, null parents), takes only the def index from the other component, reads the def (GetDataFromDef) and then clears the three transition times
// Leaf: ??0CComponent@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00533720, 641 bytes (656 with padding), `ret 4`, no EH frame. Caller: the
//   CDraggableInto copy-style constructor 0x0052CCA0 (Ego_r.pdb names `??0CComponent@NUISystem@@QAE@ABV01@@Z`).
// Callees: NUISystem::CObserver::CObserver 0x0052D9E0, CCharString::CCharString() 0x0099E4B0, malloc 0x00BFEA0E (the
//   STLport node allocator: map header 0x38, list nodes 0x0C / 0x10, set header 0x14), CComponent::GetDataFromDef 0x005331A0.
// Vtables: CUIInterface 0x01246044, CActionParam 0x0122F83C, then CComponent's own 0x0124608C (+0), 0x01246064 (+4),
//   0x0124605C (+0x18).
// Layout: local model -- NUISystem::CComponent has no engine header (engine/_quarantine/CComponent.h is another class);
//   retail is 0x134 bytes, Ego_r.pdb 0x15c: the STLport map/set/vector are 12 bytes and the lists 4 (PDB 16/16/12), so
//   Name +0x20, States +0x24, Time +0x30, Position..RenderZoom +0x34..+0x80, colours +0x84..+0x94, timers +0x98..+0xac,
//   Children +0xb0, ChildrenToDelete +0xbc, PParent +0xc8, Size +0xcc, Deletion {Method +0xd4, list +0xd8},
//   PParentViewport +0xdc, ActionsIAmParamOf +0xe0, ShapeChildren +0xe4, the relative vectors +0xf0..+0x114,
//   PPositionParent +0x118, PositionChildren +0x11c, PActionsOnDestruction +0x128, Flags/Type/FlagsSetTwo/Layer
//   +0x12c..+0x12f, DefIndex +0x130 (the same offsets as the landed CComponent::Initialise 0x005339B0).
// Shape: the sibling CComponent(long) 0x005334A0 with DefIndex zeroed in the mem-initializer list (retail keeps that
//   dead store, scheduled ahead of PActionsOnDestruction) and assigned from the other component in the body, after the
//   three pointer clears and before GetDataFromDef. Members zeroed in the mem-initializer list (Time, Size, Type and Deletion.Method are not); PParent, Deletion.Method
//   and PParentViewport are cleared in the body. CRGBColour's ctor assigns in its BODY in R, G, B, A
//   order (mem-initializers run in B,G,R,A declaration order); the STLport list ctor links a LOCAL node before storing it
//   (retail stores the member after next/prev, no reload), while the _Rb_tree header goes through the member (reloaded).
#include <stdlib.h>
#include <new>
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
    CComponent(const CComponent& other);
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

CComponent::CComponent(const CComponent& other)
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
      PPositionParent(0), PActionsOnDestruction(0), Flags(0), FlagsSetTwo(0), Layer(0), DefIndex(0)
{
    Deletion.Method = DELETION_METHOD_NONE;
    PParent = 0;
    PParentViewport = 0;
    DefIndex = other.DefIndex;
    GetDataFromDef();
    PositionTime = 0.0f;
    ZoomTime = 0.0f;
    ColourTime = 0.0f;
}
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_names, g_observers, g_defReads;
static float g_seenPositionTime; static long g_seenDefIndex; static void* g_seenParent;
CCharString::CCharString() : m_pData(0) { g_names++; }
namespace NUISystem {
CUIInterface::~CUIInterface() {}
CObserver::CObserver() : PreventObservation(false) { g_observers++; }
CObserver::~CObserver() {}
CActionParam::~CActionParam() {}
CComponent::~CComponent() {}
void CComponent::GetDataFromDef()
{
    g_defReads++; g_seenDefIndex = DefIndex; g_seenParent = PParent; g_seenPositionTime = PositionTime;
    PositionTime = 2.0f; ZoomTime = 3.0f; ColourTime = 4.0f; PositionTimeElapsed = 5.0f; Type = 7;
}
}
int main()
{
    using namespace NUISystem;
    static unsigned char buf[sizeof(CComponent)];
    static unsigned char buf2[sizeof(CComponent)];
    memset(buf, 0xcd, sizeof(buf));
    memset(buf2, 0xcd, sizeof(buf2));
    CComponent* src = new (buf) CComponent(1234);
    src->PParent = src; src->Layer = 5; src->Flags = 3;
    CComponent* c = new (buf2) CComponent(*src);
    CHECK(g_names == 2 && g_observers == 2 && g_defReads == 2 && g_seenDefIndex == 1234 && g_seenParent == 0 && g_seenPositionTime == 0.0f);
    CHECK(c->ActionParamType == ACTION_PARAM_COMPONENT && c->DefIndex == 1234);
    // nothing but the def index is shared with the source: fresh containers, cleared state
    CHECK(c->States._M_t._M_header != src->States._M_t._M_header && c->Deletion.AssociatedParents._M_node != src->Deletion.AssociatedParents._M_node);
    CHECK(c->States._M_t._M_node_count == 0 && c->States._M_t._M_header->_M_color == false && c->States._M_t._M_header->_M_parent == 0);
    CHECK(c->States._M_t._M_header->_M_left == c->States._M_t._M_header && c->States._M_t._M_header->_M_right == c->States._M_t._M_header);
    CHECK(c->PositionChildren._M_t._M_node_count == 0 && c->PositionChildren._M_t._M_header->_M_left == c->PositionChildren._M_t._M_header);
    CHECK(c->Deletion.AssociatedParents._M_node->_M_next == c->Deletion.AssociatedParents._M_node && c->Deletion.AssociatedParents._M_node->_M_prev == c->Deletion.AssociatedParents._M_node);
    CHECK(c->ActionsIAmParamOf._M_node->_M_next == c->ActionsIAmParamOf._M_node && c->ActionsIAmParamOf._M_node->_M_prev == c->ActionsIAmParamOf._M_node);
    CHECK(c->Children._M_start == 0 && c->Children._M_finish == 0 && c->Children._M_end_of_storage == 0);
    CHECK(c->ChildrenToDelete._M_start == 0 && c->ShapeChildren._M_end_of_storage == 0);
    CHECK(c->Position.X == 0.0f && c->Position.Y == 0.0f && c->RenderZoom.X == 0.0f && c->RenderZoom.Y == 0.0f);
    CHECK(c->Colour.R == 0 && c->Colour.G == 0 && c->Colour.B == 0 && c->Colour.A == 0 && c->RenderColour.A == 0);
    CHECK(c->PositionTime == 0.0f && c->ZoomTime == 0.0f && c->ColourTime == 0.0f && c->PositionTimeElapsed == 5.0f);
    CHECK(c->PParent == 0 && c->PParentViewport == 0 && c->PPositionParent == 0 && c->PActionsOnDestruction == 0 && c->Deletion.Method == DELETION_METHOD_NONE);
    CHECK(c->Flags == 0 && c->FlagsSetTwo == 0 && c->Layer == 0 && c->Type == 7);
    if (fails == 0) printf("00533720_TEST PASS\n"); else printf("00533720_TEST FAIL\n");
    return fails;
}