// Behaviour test for 0055cbe0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

#include <new>
enum EEvent { EVENT_NONE, EVENT_CLICK = 3, EVENT_HOVER = 7 };
class CObserver {
public:
    int Processed;
    EEvent Watched;
    CObserver(EEvent w) : Processed(0), Watched(w) { }
    virtual ~CObserver() { }
    virtual void ProcessEvent(EEvent event) { CHECK(event == Watched); ++Processed; }
    virtual bool IsObservingEvent(EEvent event) { return event == Watched; }
};
struct __false_type { __false_type() { } };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { CObserver* _M_data; };
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    CObserver*& operator*() const { return ((_List_node*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& y) const { return _M_node != y._M_node; }
};
static int g_nodesLive = 0;
struct list {
    _List_node* _M_node;                                          // the sentinel
    list()
    {
        _M_node = (_List_node*)std::malloc(sizeof(_List_node));
        ++g_nodesLive;
        _M_node->_M_next = _M_node->_M_prev = _M_node;
    }
    list(const list& x)                                           // the STLport copy: a fresh sentinel, then insert(begin(), x)
    {
        _M_node = (_List_node*)std::malloc(sizeof(_List_node));
        ++g_nodesLive;
        _M_node->_M_next = _M_node->_M_prev = _M_node;
        _M_insert_dispatch(_M_node->_M_next, x._M_node->_M_next, x._M_node);
    }
    ~list()
    {
        _List_node* cur = (_List_node*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node* tmp = cur;
            cur = (_List_node*)cur->_M_next;
            std::free(tmp);
            --g_nodesLive;
        }
        std::free(_M_node);
        --g_nodesLive;
    }
    void _M_insert_dispatch(_List_node_base* pos, _List_node_base* first, _List_node_base* last)   // 0x0055CE90
    {
        for (; first != last; first = first->_M_next) {
            _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
            ++g_nodesLive;
            n->_M_data = ((_List_node*)first)->_M_data;
            n->_M_next = pos;
            n->_M_prev = pos->_M_prev;
            pos->_M_prev->_M_next = n;
            pos->_M_prev = n;
        }
    }
    void Append(CObserver* o)
    {
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        ++g_nodesLive;
        n->_M_data = o;
        n->_M_next = _M_node;
        n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n;
        _M_node->_M_prev = n;
    }
    _List_iterator begin() { return _List_iterator(_M_node->_M_next); }
    _List_iterator end() { return _List_iterator(_M_node); }
};
class CObservable {
public:
    list Observers;
    virtual ~CObservable() { }
    virtual void SetOwnedEvent(CObserver* owner, EEvent event)
    {
        list observers(Observers);
        for (_List_iterator it = observers.begin(); it != observers.end(); ++it) {
            if (*it == owner && (*it)->IsObservingEvent(event))
                (*it)->ProcessEvent(event);
        }
    }
};
static void RunTest()
{
    CObserver a(EVENT_CLICK), b(EVENT_CLICK), c(EVENT_HOVER);
    {
        CObservable o;
        o.Observers.Append(&a);
        o.Observers.Append(&b);
        o.Observers.Append(&c);
        o.Observers.Append(&a);                                   // a is registered twice
        int before = g_nodesLive;
        o.SetOwnedEvent(&a, EVENT_CLICK);
        CHECK(a.Processed == 2 && b.Processed == 0 && c.Processed == 0);   // only the owner, once per entry
        o.SetOwnedEvent(&c, EVENT_CLICK);                         // the owner does not observe it
        CHECK(c.Processed == 0);
        o.SetOwnedEvent(&c, EVENT_HOVER);
        CHECK(c.Processed == 1 && a.Processed == 2);
        CHECK(g_nodesLive == before);                             // the local copy is freed
    }
    CHECK(g_nodesLive == 0);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055cbe0_TEST FAIL\n");
        return 1;
    }
    std::printf("0055cbe0_TEST PASS\n");
    return 0;
}