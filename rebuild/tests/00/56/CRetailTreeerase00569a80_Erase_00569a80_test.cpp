// Behaviour test for 0x00569A80 CRetailTree_erase00569a80::Erase (STLport _Rb_tree::_M_erase): every node, every
// element's CWideString and every vector buffer of a hand-built tree is destroyed/freed exactly once.
#include <cstdio>
#include <cstdlib>

static int g_strings = 0, g_frees = 0;
static void Free(void* p) { if (p) ++g_frees; std::free(p); }

class CWideString { public:
    void* PStringData;
    ~CWideString() { ++g_strings; }
};
namespace stl {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    ~_Vector_base() { if (_M_start) Free(_M_start); }
};
template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
typedef stl::pair<long, stl::vector<stl::pair<long, CWideString> > > SValue;
struct Tree {
    struct node {
        unsigned char colour; unsigned char _pad[3];
        node* parent; node* left; node* right;
        SValue value;
    };
    void Erase(node* x)
    {
        while (x != 0) {
            Erase(x->right);
            node* y = x->left;
            x->value.~SValue();
            Free(x);
            x = y;
        }
    }
};

static Tree::node* Make(long key, int elements, Tree::node* l, Tree::node* r)
{
    Tree::node* n = (Tree::node*)std::malloc(sizeof(Tree::node));
    n->left = l; n->right = r; n->parent = 0; n->value.first = key;
    typedef stl::pair<long, CWideString> E;
    E* p = elements ? (E*)std::malloc(sizeof(E) * elements) : 0;
    n->value.second._M_start = p; n->value.second._M_finish = p + elements; n->value.second._M_end_of_storage = p + elements;
    return n;
}

int main()
{
    //        4
    //      2   6
    //     1 3 5 7(empty vector)
    Tree::node* root = Make(4, 2, Make(2, 1, Make(1, 3, 0, 0), Make(3, 0, 0, 0)),
                                  Make(6, 1, Make(5, 2, 0, 0), Make(7, 0, 0, 0)));
    Tree t;
    t.Erase(root);
    t.Erase(0);
    // 7 nodes freed + 5 non-empty buffers freed; 2+1+3+1+2 = 9 element strings destroyed
    if (g_frees != 12 || g_strings != 9) { std::printf("00569a80_TEST FAIL\n"); return 1; }
    std::printf("00569a80_TEST PASS\n");
    return 0;
}