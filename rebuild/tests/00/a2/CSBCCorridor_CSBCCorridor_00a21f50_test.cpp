// Behaviour test for CSBCCorridor::CSBCCorridor() (0x00A21F50): the corridor starts with an empty wall list (one
// freshly allocated sentinel node linked to itself), its end point at the origin, and reports the corridor type (3).
#include <cstdio>
#include <cstdlib>

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
    C2DVector Point1, Point2, ToInternal;
};

namespace _STL {
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    _Node* _M_node;
    list()
    {
        _Node* __n = (_Node*)std::malloc(sizeof(_Node));
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node = __n;
    }
    ~list() { std::free(_M_node); }
    bool empty() const { return _M_node->_M_next == _M_node; }
};
template <class _Tp> class vector;
}

class CSB {
public:
    enum ESB_TYPE { ESB_TYPE_CCORRIDOR = 3 };
    virtual ~CSB() { }
    virtual bool Process(const CSteeringAgent* agent) = 0;
    virtual ESB_TYPE GetType() = 0;
    virtual void Reset() { }
    CSB() { }
    CKinematic Output;
};

class CSBInput : public CSB {
public:
    CSBInput() { }
    CKinematic Input;
};

class CSBConstraint : public CSBInput {
public:
    CSBConstraint() { }
    bool MoveBeforeOrientation;
};

class CSBCCorridor : public CSBConstraint {
public:
    CSBCCorridor() : CurrentEndPoint(0.0f, 0.0f) { }
    virtual bool Process(const CSteeringAgent*) { return false; }
    virtual ESB_TYPE GetType() { return ESB_TYPE_CCORRIDOR; }
    _STL::list<CWall> Walls;
    C2DVector         CurrentEndPoint;
    float             BestLeft;
    float             BestRight;
};

int main()
{
    int fails = 0;
    {
        CSBCCorridor* c = new CSBCCorridor;
        CSB* base = c;
        if (base->GetType() != CSB::ESB_TYPE_CCORRIDOR || (int)base->GetType() != 3) ++fails;
        if (!c->Walls.empty() || c->Walls._M_node == 0 || c->Walls._M_node->_M_prev != c->Walls._M_node) ++fails;
        if (c->CurrentEndPoint.X != 0.0f || c->CurrentEndPoint.Y != 0.0f) ++fails;
        CSBCCorridor other;
        if (other.Walls._M_node == c->Walls._M_node) ++fails;     // each corridor owns its own sentinel
        delete base;
    }
    if (fails) { std::printf("00a21f50_TEST FAIL\n"); return 1; }
    std::printf("00a21f50_TEST PASS\n");
    return 0;
}