// CSBCCorridor::CSBCCorridor() -- the corridor steering constraint: STLport wall list with its self-linked sentinel node, end point zeroed
// Leaf: ??0CSBCCorridor@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21F50, 52 bytes, thiscall, plain `ret`.
// Identity: FableWin.pdb ??0CSBCCorridor@@QAE@XZ; vtable 0x0129CCB0 (RTTI .?AVCSBCCorridor@@, bases CSBConstraint :
//   CSBInput : CSB; slots ??_G 0x00A22330, Process 0x00A880C0, GetType 0x00A21F90 (`return 3`), CreateLineList
//   0x00A887B0, Reset 0x00A88080). The one caller is the CNavigatorBase constructor 0x00A1C8C8 (`new CSBCCorridor`,
//   operator new(0x2C) just before).
// Callees: malloc 0x00BFEA0E (the list's sentinel node, 0x20 bytes = next, prev, CWall).
// Layout (retail, 4-byte STLport list): CSB { vptr +0, CKinematic Output +4 }, CSBInput { CKinematic Input +0xC },
//   CSBConstraint { bool MoveBeforeOrientation +0x14 }, CSBCCorridor { list<CWall> Walls +0x18, C2DVector
//   CurrentEndPoint +0x1C, float BestLeft +0x24, float BestRight +0x28 } = 0x2C bytes (Ego_r's list is 12 bytes, so
//   its offsets run 8 high past +0x18).
// Shape: the base constructors are empty (only the final vptr store survives); STLport 4.x `_List_base(a) :
//   _M_node(a, 0) { _Node* n = _M_node.allocate(1); n->_M_next = n; n->_M_prev = n; _M_node._M_data = n; }` (the
//   zero store, malloc, the two self links, then the head store), then `CurrentEndPoint(0.0f, 0.0f)`.
// Label correction: catalogued as `vector<bool>::vector<bool>` (propagated); the body is CSBCCorridor's constructor.
extern "C" void* __cdecl malloc(unsigned int size);                 // 0x00BFEA0E

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};

class C3DVector;
class CSteeringAgent;

class CKinematic {
public:
    C2DVector Vector;
    CKinematic() { }
};

class CWall {
public:
    C2DVector Point1;                                               // +0x00
    C2DVector Point2;                                               // +0x08
    C2DVector ToInternal;                                           // +0x10
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                                       // +0x00
    _List_node_base* _M_prev;                                       // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                                    // +0x08
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;    // +0x00
    _List_base(const _Alloc& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base();
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector;
}

class CSB {
public:
    enum ESB_TYPE { ESB_TYPE_CCORRIDOR = 3 };
    virtual ~CSB();                                                 // slot 0 (??_G)
    virtual bool Process(const CSteeringAgent* agent) = 0;          // slot 1
    virtual ESB_TYPE GetType() = 0;                                 // slot 2
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);    // slot 3
    virtual void Reset();                                           // slot 4
    CSB() { }
    CKinematic Output;                                              // +0x04
};

class CSBInput : public CSB {
public:
    CSBInput() { }
    CKinematic Input;                                               // +0x0C
};

class CSBConstraint : public CSBInput {
public:
    CSBConstraint() { }
    bool MoveBeforeOrientation;                                     // +0x14
};

class CSBCCorridor : public CSBConstraint {
public:
    CSBCCorridor();
    virtual ~CSBCCorridor();
    virtual bool Process(const CSteeringAgent* agent);              // 0x00A880C0
    virtual ESB_TYPE GetType();                                     // 0x00A21F90
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);    // 0x00A887B0
    virtual void Reset();                                           // 0x00A88080

    _STL::list<CWall> Walls;                                        // +0x18
    C2DVector         CurrentEndPoint;                              // +0x1C
    float             BestLeft;                                     // +0x24
    float             BestRight;                                    // +0x28
};

CSBCCorridor::CSBCCorridor() : CurrentEndPoint(0.0f, 0.0f)
{
}