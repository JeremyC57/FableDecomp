// Behaviour test for 007b9d30 -- CTCTavernTable's implicit destructor frees the seat bits, food and tankard pointers (destroying each), the occupants, then the CTCBase base
#include <cstdio>
#include <cstdlib>

// Instrumentation: every container release goes through TestFree; Log records the destruction order.
static int g_frees = 0;
static int g_log[64];
static int g_nlog = 0;
static void Log(int x) { if (g_nlog < 64) g_log[g_nlog++] = x; }
static void TestFree(void* p) { if (p) { ++g_frees; std::free(p); } }
static int Find(int x) { for (int i = 0; i < g_nlog; ++i) if (g_log[i] == x) return i; return -1; }
enum { LOG_ERASE = 1000 };

// A copy of the STLport 4.6 container model of the source (namespace stlp so the host's std is untouched).
namespace stlp {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)std::malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) TestFree(__p); }
};
template <class _T1, class _T2> struct pair { typedef _T1 first_type; _T1 first; _T2 second; };
template <class _Tp> struct less { bool operator()(const _Tp& a, const _Tp& b) const { return a < b; } };
template <class _Value, class _Tp, class _A> class _STLP_alloc_proxy : public _A { public: _Value _M_data; };
struct _Rb_tree_node_base { bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base() { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Value> class _Rb_tree : public _Rb_tree_base<_Value> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;
    _Rb_tree() : _M_node_count(0)
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x)                        // the out-of-line STLport body (_tree.c)
    {
        while (__x != 0) {
            _M_erase((_Link_type)__x->_M_right);
            _Link_type __y = (_Link_type)__x->_M_left;
            this->_M_header.deallocate(__x, 1);
            __x = __y;
        }
    }
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            Log(LOG_ERASE);                              // one entry per non-empty tree released
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    void push_right(const _Value& v)                     // test helper: append as the rightmost node (no rebalancing)
    {
        _Link_type n = this->_M_header.allocate(1);
        n->_M_value_field = v; n->_M_left = 0; n->_M_right = 0; n->_M_color = true;
        if (_M_node_count == 0) { _M_root() = n; n->_M_parent = this->_M_header._M_data; _M_leftmost() = n; }
        else { _M_rightmost()->_M_right = n; n->_M_parent = _M_rightmost(); }
        _M_rightmost() = n;
        ++_M_node_count;
    }
};
template <class _Key, class _Tp> class map { public: _Rb_tree<pair<_Key, _Tp> > _M_t; };
template <class _Key> class set { public: _Rb_tree<_Key> _M_t; };

template <class _Tp> class vector {                      // _Vector_base + vector: destroy the elements, free the storage
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        if (_M_start != 0) TestFree(_M_start);
    }
    void reserve_raw(unsigned int n) { _M_start = _M_finish = (_Tp*)std::malloc(n * sizeof(_Tp)); _M_end_of_storage = _M_start + n; }
};
} // namespace stlp


#include <new>
enum { LOG_BASE = 1, LOG_PTR = 100 };
class CTCBase { public: void* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs; virtual ~CTCBase() { Log(LOG_BASE); } };
class CBaseIntelligentPointer { public: long Id; virtual ~CBaseIntelligentPointer() { Log(LOG_PTR + Id); } };
class CIntelligentPointer : public CBaseIntelligentPointer { };
class CBitVector {                                      // STLport vector<bool>: free the word storage
public:
    unsigned int* StartP; unsigned int StartOffset; unsigned int* FinishP; unsigned int FinishOffset; unsigned int* EndOfStorage;
    ~CBitVector() { if (StartP) TestFree(StartP); }
};
class CTCTavernTable : public CTCBase {
public:
    bool BeingServed;
    stlp::vector<void*> Occupants;
    stlp::vector<CIntelligentPointer> Tankards;
    stlp::vector<CIntelligentPointer> Food;
    CBitVector SeatsCurrentlyUsed;
    long NumPeopleSeated;
};
int main()
{
    {
        CTCTavernTable t;
        t.Occupants.reserve_raw(2);
        t.Tankards.reserve_raw(2); new (t.Tankards._M_finish) CIntelligentPointer(); t.Tankards._M_finish->Id = 1; ++t.Tankards._M_finish;
        t.Food.reserve_raw(2); new (t.Food._M_finish) CIntelligentPointer(); t.Food._M_finish->Id = 2; ++t.Food._M_finish;
        t.SeatsCurrentlyUsed.StartP = (unsigned int*)std::malloc(4);
    }
    if (g_frees != 4) { std::printf("007b9d30_TEST FAIL\n"); return 1; }
    const int want[] = { LOG_PTR + 2, LOG_PTR + 1, LOG_BASE };
    if (g_nlog != 3) { std::printf("007b9d30_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (g_log[i] != want[i]) { std::printf("007b9d30_TEST FAIL\n"); return 1; }
    std::printf("007b9d30_TEST PASS\n");
    return 0;
}