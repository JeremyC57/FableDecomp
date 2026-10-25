// Behaviour test for CSteeringAgent::CreateLineList (0x00A1B470): each behaviour, then each input, then each
// constraint is asked (in list order) to add its lines to the same vector; empty lists are skipped.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class CNavigatorExternalInterface;

namespace std {
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_List_node_base*, _Alloc> _M_node;
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
};
}

static int g_log[16];
static int g_count = 0;
static std::vector<C3DVector>* g_seen = 0;

class CSB {
public:
    int Id;
    virtual ~CSB() {}
    virtual void CreateLineList(std::vector<C3DVector>& lines) { g_seen = &lines; if (g_count < 16) g_log[g_count++] = Id; }
};
class CSBInput {
public:
    int Id;
    virtual ~CSBInput() {}
    virtual void CreateLineList(std::vector<C3DVector>& lines) { if (&lines != g_seen) g_seen = 0; if (g_count < 16) g_log[g_count++] = 100 + Id; }
};
class CSBConstraint {
public:
    int Id;
    virtual ~CSBConstraint() {}
    virtual void CreateLineList(std::vector<C3DVector>& lines) { if (&lines != g_seen) g_seen = 0; if (g_count < 16) g_log[g_count++] = 200 + Id; }
};

class CSteeringAgent {
public:
    CNavigatorExternalInterface*  NavigatorInterface;
    bool                          DummyObstacle;
    float                         RepulsionMultiplier;
    std::list<CSB*>              SteeringBehaviours;
    std::list<CSBInput*>         SteeringBehaviourInputs;
    std::list<CSBConstraint*>    SteeringBehaviourConstraints;
    void CreateLineList(std::vector<C3DVector>& lines);
};

void CSteeringAgent::CreateLineList(std::vector<C3DVector>& lines)
{
    for (std::list<CSB*>::iterator b = SteeringBehaviours.begin(); b != SteeringBehaviours.end(); ++b)
        (*b)->CreateLineList(lines);
    for (std::list<CSBInput*>::iterator i = SteeringBehaviourInputs.begin(); i != SteeringBehaviourInputs.end(); ++i)
        (*i)->CreateLineList(lines);
    for (std::list<CSBConstraint*>::iterator c = SteeringBehaviourConstraints.begin(); c != SteeringBehaviourConstraints.end(); ++c)
        (*c)->CreateLineList(lines);
}

template <class T> static void Link(std::_List_node_base& head, std::_List_node<T*>* nodes, T** items, int n)
{
    std::_List_node_base* prev = &head;
    for (int i = 0; i < n; ++i) { nodes[i]._M_data = items[i]; prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = &head; head._M_prev = prev;
}

int main()
{
    int ok = 1;
    CSB b1, b2; b1.Id = 1; b2.Id = 2;
    CSBConstraint c1; c1.Id = 1;
    CSB* bs[2] = { &b1, &b2 };
    CSBConstraint* cs[1] = { &c1 };
    std::_List_node_base hb, hi, hc;
    std::_List_node<CSB*> nb[2];
    std::_List_node<CSBConstraint*> nc[1];
    Link(hb, nb, bs, 2);
    hi._M_next = &hi; hi._M_prev = &hi;                              // no inputs
    Link(hc, nc, cs, 1);
    CSteeringAgent agent;
    agent.SteeringBehaviours._M_node._M_data = &hb;
    agent.SteeringBehaviourInputs._M_node._M_data = &hi;
    agent.SteeringBehaviourConstraints._M_node._M_data = &hc;
    std::vector<C3DVector> lines;
    agent.CreateLineList(lines);
    if (g_count != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 201) ok = 0;
    if (g_seen != &lines) ok = 0;

    if (!ok) { std::printf("00a1b470_TEST FAIL\n"); return 1; }
    std::printf("00a1b470_TEST PASS\n");
    return 0;
}