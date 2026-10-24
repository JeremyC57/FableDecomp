// Behaviour test of _Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > >::_M_erase(_Rb_tree_node* x) (retail 0x005E1F30)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

extern "C" void __cdecl free(void* p);

struct CCharString {
    void* PStringData;
    ~CCharString();                                        // 0x0099EAE0
};

struct CAnimComponentBase {                                // CIVCountedPointeeBase-derived: vptr, reference count at +4
    virtual void v00();
    virtual void Delete();                                 // slot 1 (+0x04): destroy on the last reference
    long IVRefCount;                                       // +0x04
};

template <class T> struct CIVCountedPointer {              // one word: the intrusively counted object
    T* Object;
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> class allocator {                       // STLport allocator over malloc / free
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

struct _Rb_tree_node_base {
    char                _M_color;                          // +0x0
    _Rb_tree_node_base* _M_parent;                         // +0x4
    _Rb_tree_node_base* _M_left;                           // +0x8
    _Rb_tree_node_base* _M_right;                          // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                      // +0x10
};

template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    _Node*        _M_header;                               // +0x0
    unsigned long _M_node_count;                           // +0x4
    static _Node* _S_left(_Node* x) { return (_Node*)x->_M_left; }
    static _Node* _S_right(_Node* x) { return (_Node*)x->_M_right; }
    void _M_erase(_Node* x);
};

template <class K, class V> void _Rb_tree<K, V>::_M_erase(_Node* x)
{
    while (x != 0) {
        _M_erase(_S_right(x));
        _Node* y = _S_left(x);
        _Destroy(&x->_M_value_field);
        allocator<_Node>().deallocate(x, 1);
        x = y;
    }
}

template class _Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > >;

// ---- test bodies of the retail callees ----
extern "C" void* __cdecl malloc(unsigned int size);
static int g_strDtors = 0;
static int g_deleted = 0;
CCharString::~CCharString() { ++g_strDtors; }
void CAnimComponentBase::v00() { }
void CAnimComponentBase::Delete() { ++g_deleted; }

typedef _Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > > CTree;
static int g_names[4];
static CTree::_Node* NewNode(int i, CAnimComponentBase* component, CTree::_Node* left, CTree::_Node* right)
{
    CTree::_Node* n = (CTree::_Node*)malloc(sizeof(CTree::_Node));
    n->_M_color = 0;
    n->_M_parent = 0;
    n->_M_left = left;
    n->_M_right = right;
    const_cast<CCharString&>(n->_M_value_field.first).PStringData = &g_names[i];
    n->_M_value_field.second.Object = component;
    return n;
}
int main()
{
    static CAnimComponentBase c0, c1, c3;
    c0.IVRefCount = 1;
    c1.IVRefCount = 2;                                      // still referenced elsewhere: released, not deleted
    c3.IVRefCount = 1;
    CTree::_Node* n3 = NewNode(3, &c3, 0, 0);
    CTree::_Node* n1 = NewNode(1, &c1, 0, n3);              // a left child with a right child
    CTree::_Node* n2 = NewNode(2, 0, 0, 0);                 // a null component
    CTree::_Node* root = NewNode(0, &c0, n1, n2);
    CTree tree;
    tree._M_header = 0;
    tree._M_node_count = 4;
    tree._M_erase(root);
    bool ok = g_strDtors == 4 && g_deleted == 2 && c0.IVRefCount == 0 && c1.IVRefCount == 1 && c3.IVRefCount == 0;
    tree._M_erase(0);
    ok = ok && g_strDtors == 4 && g_deleted == 2;
    if (!ok) { std::printf("005e1f30_TEST FAIL\n"); return 1; }
    std::printf("005e1f30_TEST PASS\n");
    return 0;
}