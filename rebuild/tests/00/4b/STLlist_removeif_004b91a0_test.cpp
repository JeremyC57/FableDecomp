// Behaviour test for STLport list<CQuestCard>::remove_if<CIsQuestCardWithFeatIndex> (0x004B91A0).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_dtors = 0, g_freed = 0;

class CCharString {
public:
    const char* PData;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) {}
    ~CCharString() { ++g_dtors; }
    bool operator==(const CCharString& other) const
    {
        if (PData == other.PData) return true;
        if (PData && other.PData && std::strlen(PData) == std::strlen(other.PData))
            return std::strcmp(PData, other.PData) ? false : true;
        return false;
    }
};

class CQuestCard {
public:
    CCharString DefName, ScriptName;
    bool Replayable;
    long FeatIndex;
    CQuestCard(const char* def, long feat) : DefName(def), ScriptName("script"), Replayable(false), FeatIndex(feat) {}
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };

template <class T> struct list {
    typedef _List_node<T> _Node;
    _List_node_base* _M_node;
    list() { _M_node = (_List_node_base*)std::malloc(sizeof(_List_node_base)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void push_back(const T& v)
    {
        _Node* n = (_Node*)std::malloc(sizeof(_Node));
        new (&n->_M_data) T(v);
        n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
    void erase(_List_node_base* pos)
    {
        _List_node_base* next = pos->_M_next;
        _List_node_base* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        ((_Node*)pos)->_M_data.~T();
        ++g_freed;
        std::free(pos);
    }
    template <class P> void remove_if(P pred)
    {
        _List_node_base* first = _M_node->_M_next;
        _List_node_base* last = _M_node;
        while (first != last) {
            _List_node_base* next = first->_M_next;
            if (pred(((_Node*)first)->_M_data))
                erase(first);
            first = next;
        }
    }
    int size() const { int n = 0; for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
    T& at(int i) { _List_node_base* p = _M_node->_M_next; while (i--) p = p->_M_next; return ((_Node*)p)->_M_data; }
};

class CIsQuestCardWithFeatIndex {
public:
    long FeatIndex;
    bool operator()(const CQuestCard& card) const { return card.FeatIndex == FeatIndex; }
};

int main()
{
    list<CQuestCard> l;
    l.push_back(CQuestCard("A", 1)); l.push_back(CQuestCard("B", 2)); l.push_back(CQuestCard("C", 1)); l.push_back(CQuestCard("D", 3));
    g_dtors = 0;
    CIsQuestCardWithFeatIndex pred; pred.FeatIndex = 1;
    l.remove_if(pred);
    if (l.size() != 2 || l.at(0).FeatIndex != 2 || l.at(1).FeatIndex != 3 || g_dtors != 4 || g_freed != 2) { std::printf("004b91a0_TEST FAIL\n"); return 1; }
    pred.FeatIndex = 9;
    l.remove_if(pred);
    if (l.size() != 2) { std::printf("004b91a0_TEST FAIL\n"); return 1; }
    std::printf("004b91a0_TEST PASS\n");
    return 0;
}