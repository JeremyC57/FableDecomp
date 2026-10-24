// Behaviour test for 0x0056A680 std::map<long, vector<pair<long, CWideString> > >::operator[]: a missing key
// inserts one node holding an empty vector (and destroys the value_type temporary); a present key returns the
// same mapped vector without inserting.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_inserts = 0, g_frees = 0;

struct CWideString { void* PStringData; };
namespace stl {
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
};
template <class _Tp> class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    vector(const vector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        if (n) { _M_start = (_Tp*)std::malloc(n * sizeof(_Tp)); _M_finish = _M_end_of_storage = _M_start + n; }
    }
    ~vector() { if (_M_start) { ++g_frees; std::free(_M_start); } }
};
struct node_base { bool colour; node_base* parent; node_base* left; node_base* right; };
template <class V> struct node : node_base { V value; };
template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef node<value_type> N;
    node_base* header; unsigned int count; unsigned int cmp;
    map() : header((node_base*)std::calloc(1, sizeof(node_base))), count(0), cmp(0) { header->left = header->right = header; }
    node_base* insert_unique(node_base* pos, const value_type& v)
    {
        ++g_inserts;
        N* z = (N*)std::malloc(sizeof(N));
        new (&z->value) value_type(v);
        z->left = z->right = 0;
        // plain BST insert (the retail hint/rebalance is not modelled)
        node_base** link = &header->parent; node_base* parent = header;
        while (*link) { parent = *link; link = v.first < ((N*)*link)->value.first ? &(*link)->left : &(*link)->right; }
        *link = z; z->parent = parent; ++count;
        (void)pos;
        return z;
    }
    T& operator[](const K& k)
    {
        node_base* y = header; node_base* x = header->parent;
        while (x != 0)
            if (!(((N*)x)->value.first < k)) y = x, x = x->left; else x = x->right;
        if (y == header || k < ((N*)y)->value.first)
            y = insert_unique(y, value_type(k, T()));
        return ((N*)y)->value.second;
    }
};
}

int main()
{
    typedef stl::vector<stl::pair<long, CWideString> > V;
    stl::map<long, V> m;
    V& a = m[5];
    V& b = m[-3];
    V& c = m[9];
    V& a2 = m[5];
    V& b2 = m[-3];
    bool ok = &a == &a2 && &b == &b2 && &a != &b && &a != &c && m.count == 3 && g_inserts == 3 &&
              a._M_start == 0 && a._M_finish == 0 && c._M_end_of_storage == 0 && g_frees == 0;
    std::printf(ok ? "0056a680_TEST PASS\n" : "0056a680_TEST FAIL\n");
    return ok ? 0 : 1;
}