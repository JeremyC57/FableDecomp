#include <cstdio>
#include <cstdlib>
// Behaviour test for CEngineSimulationManager::AddSimulationInfo(info, paused, fullUpdateOnly) @ 0x00B817D0: paused wins,
// then full-update-only, else always-update; the node is appended at the list's end and holds a counted reference.
inline void* operator new(size_t, void* where, int) { return where; }
class CEnginePrimitiveSimulationInfo { public:
    void* vptr;
    long  IVRefCount;
};
template <class T> class CIVCountedPointer {
public:
    T* Data;
    CIVCountedPointer(T* p) : Data(p) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Data(other.Data)
    {
        if (Data)
            ++Data->IVRefCount;
    }
};
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
    _List_node_base* _M_node;
    list() { _M_node = (_List_node_base*)malloc(sizeof(_List_node_base)); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; }
    unsigned int size() const { unsigned int n = 0; for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
    _Tp& back() { return ((_Node*)_M_node->_M_prev)->_M_data; }
    void push_back(const _Tp& __x)
    {
        _Node* __tmp = (_Node*)malloc(sizeof(_Node));
        new (&__tmp->_M_data, 0) _Tp(__x);
        _List_node_base* __n = _M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
};
typedef CIVCountedPointer<CEnginePrimitiveSimulationInfo> CSimulationInfoPtr;
class CEngineSimulationManager {
public:
    list<CSimulationInfoPtr> FullUpdateOnlySimulationInfoList;
    list<CSimulationInfoPtr> AlwaysUpdateSimulationInfoList;
    list<CSimulationInfoPtr> PausedSimulationInfoList;
    void AddSimulationInfo(const CSimulationInfoPtr& info, bool paused, bool fullUpdateOnly)
    {
        if (paused)
            PausedSimulationInfoList.push_back(info);
        else if (fullUpdateOnly)
            FullUpdateOnlySimulationInfoList.push_back(info);
        else
            AlwaysUpdateSimulationInfoList.push_back(info);
    }
};
int main()
{
    CEngineSimulationManager m;
    CEnginePrimitiveSimulationInfo a, b, c, d;
    a.IVRefCount = 1; b.IVRefCount = 1; c.IVRefCount = 1; d.IVRefCount = 1;
    CSimulationInfoPtr pa(&a), pb(&b), pc(&c), pd(&d), pnull(0);
    m.AddSimulationInfo(pa, true, true);
    m.AddSimulationInfo(pb, false, true);
    m.AddSimulationInfo(pc, false, false);
    m.AddSimulationInfo(pd, false, false);
    m.AddSimulationInfo(pnull, true, false);
    if (m.PausedSimulationInfoList.size() != 2 || m.FullUpdateOnlySimulationInfoList.size() != 1 || m.AlwaysUpdateSimulationInfoList.size() != 2) { std::printf("00b817d0_TEST FAIL\n"); return 1; }
    if (a.IVRefCount != 2 || b.IVRefCount != 2 || c.IVRefCount != 2 || d.IVRefCount != 2) { std::printf("00b817d0_TEST FAIL\n"); return 1; }
    if (m.AlwaysUpdateSimulationInfoList.back().Data != &d || m.PausedSimulationInfoList.back().Data != 0 || m.FullUpdateOnlySimulationInfoList.back().Data != &b) { std::printf("00b817d0_TEST FAIL\n"); return 1; }
    _List_node_base* first = m.AlwaysUpdateSimulationInfoList._M_node->_M_next;
    if (((_List_node<CSimulationInfoPtr>*)first)->_M_data.Data != &c || first->_M_prev != m.AlwaysUpdateSimulationInfoList._M_node) { std::printf("00b817d0_TEST FAIL\n"); return 1; }
    std::printf("00b817d0_TEST PASS\n");
    return 0;
}