// CRetailTree_erase00569a80::Erase(node* x) -- STLport _Rb_tree::_M_erase for the scoreboard map<long, vector<pair<long, CWideString> > >: free a subtree (recurse right, destroy the entry vector, free the node, continue left)
// Leaf: ?Erase@CRetailTree_erase00569a80@@QAEXPAUnode@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00569A80, 102 bytes, thiscall `ret 4`, self-recursive (this spilled to the
//   `push ecx` word and reloaded for the recursion). Identity: the landed tree destructors 0x0056A320 / 0x0056A5C0
//   bind this address as CRetailTree_erase00569a80::Erase(node*) (the catalog convention for these erase
//   instantiations, as the landed sibling 0x0056DE50), so the row keeps that symbol. The catalog label
//   "TreeNode_DestructWithCCharString" is descriptive.
// Callees: CWideString::~CWideString() @ 0x0099B510 (each element's second member), free @ 0x00BFEA14 (the
//   vector storage, then the node).
// Layout: STLport node -- colour +0, parent +4, left +8, right +0xC, value +0x10 = pair<const long, vector<
//   pair<long, CWideString> > > (key +0x10, vector start/finish/end +0x14/+0x18/+0x1C); 8-byte elements with the
//   CWideString at +4 (the scoreboard operator[] 0x0056A680 builds the same value).
// Shape: `while (x) { _M_erase(right(x)); y = left(x); destroy_node(x); x = y; }` with the STLport vector
//   destructor inline: `_Destroy(_M_start, _M_finish)` element loop (finish cached in ebx), then the
//   _Vector_base `if (_M_start) deallocate` (start re-read), then the node's free.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                     // 0x0099B510
};

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

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
    ~_Vector_base() { if (_M_start) free(_M_start); }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}

typedef std::pair<const long, std::vector<std::pair<long, CWideString> > > SScoreboardMapValue;

struct CRetailTree_erase00569a80 {
    struct node {
        unsigned char       colour;
        unsigned char       _pad[3];
        node*               parent;                     // +0x04
        node*               left;                       // +0x08
        node*               right;                      // +0x0C
        SScoreboardMapValue value;                      // +0x10
    };
    node*         head;
    unsigned long size;
    void Erase(node* root);
};

void CRetailTree_erase00569a80::Erase(node* x)
{
    while (x != 0) {
        Erase(x->right);
        node* y = x->left;
        x->value.~SScoreboardMapValue();
        free(x);
        x = y;
    }
}