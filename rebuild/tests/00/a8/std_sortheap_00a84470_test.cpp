// behaviour test for 00a84470: SGI sort_heap over (unsigned __int64, list<payload>) pairs; each list must follow its key and the nodes must balance
#include <cstdio>
#include <cstring>
#include <cstdlib>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_nodes = 0;
struct SListPayload { unsigned long m_words[4]; };
struct _List_node {
    _List_node*  _M_next;
    _List_node*  _M_prev;
    SListPayload _M_data;
};
struct list {
    _List_node* _M_node;                        // sentinel
    list() { Init(); }
    list(const list& x) { Init(); Append(x); }
    ~list() { clear(); std::free(_M_node); --g_nodes; }
    list& operator=(const list& x) { if (this != &x) { clear(); Append(x); } return *this; }
    void Init() { _M_node = (_List_node*)std::malloc(sizeof(_List_node)); ++g_nodes; _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void push_back(const SListPayload& v)
    {
        _List_node* node = (_List_node*)std::malloc(sizeof(_List_node));
        ++g_nodes;
        node->_M_data = v;
        node->_M_next = _M_node;
        node->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = node;
        _M_node->_M_prev = node;
    }
    void Append(const list& x) { for (_List_node* p = x._M_node->_M_next; p != x._M_node; p = p->_M_next) push_back(p->_M_data); }
    void clear()
    {
        _List_node* p = _M_node->_M_next;
        while (p != _M_node) { _List_node* next = p->_M_next; std::free(p); --g_nodes; p = next; }
        _M_node->_M_next = _M_node->_M_prev = _M_node;
    }
    unsigned int size() const { unsigned int n = 0; for (_List_node* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
};
struct SKeyedList {
    u64  first;
    list second;
    SKeyedList() : first(0) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SKeyedList& a, const SKeyedList& b) const { return a.first < b.first; }
};
typedef SKeyedList ElemT;
typedef CKeyPairCompareLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    unsigned int count = (unsigned int)(k % 3) + 1;
    unsigned int i = 0;
    e.first = ((u64)k << 33) + 5u;
    e.second.clear();
    for (i = 0; i < count; ++i) {
        SListPayload v;
        v.m_words[0] = (unsigned long)k; v.m_words[1] = i; v.m_words[2] = (unsigned long)(k * k); v.m_words[3] = 0xabcdu;
        e.second.push_back(v);
    }
}
static int KeyOf(const ElemT& e) { return int(e.first >> 33); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    unsigned int i = 0;
    _List_node* p = e.second._M_node->_M_next;
    if ((unsigned)(e.first & 0xffffffffu) != 5u || e.second.size() != (unsigned int)(k % 3) + 1) return false;
    for (i = 0; p != e.second._M_node; p = p->_M_next, ++i)
        if (p->_M_data.m_words[0] != (unsigned long)k || p->_M_data.m_words[1] != i || p->_M_data.m_words[2] != (unsigned long)(k * k)
            || p->_M_data.m_words[3] != 0xabcdu)
            return false;
    return true;
}
static bool Live(int) { return true; }
static bool Final() { return g_nodes == 0; }

static void PushHeapT(ElemT* first, long hole, long top, ElemT value, CompT comp)
{
    long parent = (hole - 1) / 2;
    while (top < hole && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeapT(ElemT* first, long hole, long len, ElemT value, CompT comp)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < len; idx = 2 * idx + 2) {
        if (comp(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx];
        hole = idx;
    }
    if (idx == len) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeapT(first, hole, top, value, comp);
}

// the out-of-line callee (__pop_heap in retail), modelled with the standard hole algorithm
static void __pop_heap(ElemT* first, ElemT* last, ElemT* result, ElemT value, CompT comp, int*)
{
    *result = *first;
    AdjustHeapT(first, 0, long(last - first), value, comp);
}
// the reconstructed body: SGI sort_heap with pop_heap / __pop_heap_aux inline
inline void __pop_heap_aux(ElemT* first, ElemT* last, ElemT*, CompT comp)
{
    __pop_heap(first, last - 1, last - 1, ElemT(*(last - 1)), comp, (int*)0);
}
inline void pop_heap(ElemT* first, ElemT* last, CompT comp)
{
    __pop_heap_aux(first, last, (ElemT*)0, comp);
}
static void sort_heap(ElemT* first, ElemT* last, CompT comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}
#define SORT(a, n, comp) sort_heap(a, a + n, comp)

int main()
{
    static const int keys[9] = { 5, 31, 12, 7, 64, 0, 99, 40, 2 };
    const int n = 9;
    int ok = 1;
    {
        ElemT a[9];
        CompT comp = MakeComp();
        int i = 0;
        for (i = 0; i < n; ++i) Fill(a[i], keys[i]);
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        for (i = 0; i < n; ++i) if (!Check(a[i])) ok = 0;
        if (KeyOf(a[0]) != 0 || KeyOf(a[n - 1]) != 99) ok = 0;
        if (!Live(n)) ok = 0;
        // re-heap the sorted output and sort again; a one-element range is left as it is
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        SORT(a, 1, comp);
        if (KeyOf(a[0]) != 0 || !Check(a[0])) ok = 0;
        if (!Live(n)) ok = 0;
    }
    if (!Final()) ok = 0;
    if (!ok) { std::printf("00a84470_TEST FAIL\n"); return 1; }
    std::printf("00a84470_TEST PASS\n");
    return 0;
}