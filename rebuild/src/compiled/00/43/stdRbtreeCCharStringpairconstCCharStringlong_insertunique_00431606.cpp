// std::_Rb_tree<CCharString, pair<const CCharString, long>, ...>::insert_unique(const value_type&) -- insert a (string, long) pair into the map unless the key is present
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- the inlined comparator's eq test is a goto into the !b arm's result=false block (the merged false block then sits after the !b test with the eq je forward, as retail; early returns place it there too but jump-thread the bool)
// Leaf: ?insert_unique@_Rb_tree@std@@QAE?AU?$pair@U_Rb_tree_iterator@std@@_N@2@ABQAU?$CBasicString@D@@@Z
// TYPE NOTE (found after the 4 grades were spent): the tree is map<CCharString, long> -- its _M_insert is the next row 0x004316CF
// (matched with a CCharString key and an inlined CCharString::operator<) and its _M_create_node is the landed 0x00431773. The
// model below was graded as a set of CBasicString<char>* under CComparePointer, which loads the same words (value.first's data
// pointer, then the C string at the block's +0) and is byte-equivalent; Fable 5.1 should re-model it with the 0x004316CF types.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00431606, length 201 bytes, `ret 8` (__fastcall: hidden pair<iterator,bool> result + the value).
// Name: STLport 4.x `_Rb_tree::insert_unique(const value_type&)` of the map<CCharString, long> tree (see TYPE NOTE; the graded
// stand-in borrows the landed 0x007AD060 set<CBasicString<char>*> model); eight callers (0x004315E2, 0x004751D4, 0x00516B1B, ...) share this
// folded body. The catalog's "Map_InsertStringKeyNode" is a label.
// Callees: _Rb_tree::_M_insert(x, y, v, w) @ 0x004316CF (next row), _Rb_global<bool>::_M_decrement(node) @ 0x004294E2 (landed,
// static __cdecl: `push ; call ; pop ecx`), Compare(const char*, const char*) @ 0x00411570 (landed, __fastcall).
// Pragmas: #pragma optimize("y", off) + #pragma optimize("s", on) -- frame pointer, `this` parked in [ebp-4] across the loop.
// Shape: STLport verbatim: `y = header, x = root, comp = true; while (x) { y = x; comp = cmp(v, key(x)); x = comp ? left : right; }`,
// then the leftmost test, the decrement and the second compare. The comparator is CComparePointer<CBasicString<char> > (null
// pointers first, equal pointers never less) whose inner `*a < *b` is CBasicString<char>::operator< INLINED here as
// `Compare(a->PData, b->PData) < 0` (`setl al`) -- in the landed 0x007AD060 TU it is an out-of-line strcmp loop. Both _M_insert
// calls reuse the value's stack slot [ebp+0xC] for their hidden iterator result.
#pragma optimize("y", off)
#pragma optimize("s", on)
long __fastcall Compare(const char* a, const char* b);  // 0x00411570

template <class C> struct CBasicString {
    C* PData;                                            // +0x00
    __forceinline bool operator<(const CBasicString& other) const { return Compare(PData, other.PData) < 0; }
};

template <class T> struct CComparePointer {
    __forceinline bool operator()(const T* a, const T* b) const
    {
        bool result;
        if (b == a)
            goto isFalse;
        if (!a)
            result = true;
        else if (!b) {
        isFalse:
            result = false;
        }
        else
            result = *a < *b;
        return result;
    }
};

namespace std {
struct _Rb_tree_node {
    char           _M_color;                             // +0x00
    _Rb_tree_node* _M_parent;                            // +0x04
    _Rb_tree_node* _M_left;                              // +0x08
    _Rb_tree_node* _M_right;                             // +0x0C
    CBasicString<char>* _M_value_field;                  // +0x10
};
struct _Rb_global_inst {
    static _Rb_tree_node* __cdecl _M_decrement(_Rb_tree_node* x);   // 0x004294E2
};
struct _Rb_tree_iterator {
    _Rb_tree_node* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node* x) : _M_node(x) {}
    void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }
};
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
};
class _Rb_tree {
public:
    typedef CBasicString<char>* value_type;
    _Rb_tree_node*                       _M_header;      // +0x00
    unsigned long                        _M_node_count;  // +0x04
    CComparePointer<CBasicString<char> > _M_key_compare;
private:
    _Rb_tree_iterator _M_insert(_Rb_tree_node* x, _Rb_tree_node* y, const value_type& v, _Rb_tree_node* w);   // 0x004316CF
public:
    pair<_Rb_tree_iterator, bool> insert_unique(const value_type& v);
};

pair<_Rb_tree_iterator, bool> _Rb_tree::insert_unique(const value_type& v)
{
    _Rb_tree_node* y = _M_header;
    _Rb_tree_node* x = _M_header->_M_parent;
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = _M_key_compare(v, x->_M_value_field);
        x = comp ? x->_M_left : x->_M_right;
    }
    _Rb_tree_iterator j(y);
    if (comp) {
        if (j._M_node == _M_header->_M_left)
            return pair<_Rb_tree_iterator, bool>(_M_insert(y, y, v, 0), true);
        else
            j._M_decrement();
    }
    if (_M_key_compare(j._M_node->_M_value_field, v))
        return pair<_Rb_tree_iterator, bool>(_M_insert(x, y, v, 0), true);
    return pair<_Rb_tree_iterator, bool>(j, false);
}
}