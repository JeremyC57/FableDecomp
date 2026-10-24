// std::_Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString>, allocator<CCharString> >::insert_unique(const CCharString* first, const CCharString* last) -- insert every string of a range into the set, skipping duplicates
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B9F10, length 49 bytes.
// Leaf: ??$insert_unique@PBV
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" (4 arguments) is propagated. The body is
// STLport's range `insert_unique(first, last)` over 4-byte elements calling `insert_unique(const value_type&)`
// @ 0x004AC380 (manifest "StdSet_Insert": its comparator is the inline less<CCharString> -- equal data pointers, null
// tests, then CBasicString<char>::operator< @ 0x00429950, nodes holding the key at +0x10). Callers: two
// `set<CCharString>(first, last)` range constructors inlined in CTCGraphicAppearance::PrepareForDraw (0x004BECB5,
// 0x004BF5C5: header node from operator new, then this call) and the tail-jump `set::insert(first, last)` 0x004BA910.
// Callees: _Rb_tree::insert_unique(const CCharString&) @ 0x004AC380 (pair<iterator, bool> through a hidden pointer).
// Shape: thiscall, `ret 8`; the discarded pair<iterator, bool> result lives in the 8-byte frame (`sub esp,8`), so
// pair and iterator need user constructors (a POD 8-byte pair returns in edx:eax and drops the frame).
class CCharString {
public:
    void* PStringData;                            // +0x0 CCharStringData*
};
namespace std {
template <class T> struct less {};
template <class T> struct _Identity {};
template <class T> class allocator {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base;
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};
template <class K, class V, class KoV, class Cmp, class A>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;                // +0x0
    unsigned int        _M_node_count;            // +0x4
    Cmp                 _M_key_compare;           // +0x8
    pair<iterator, bool> insert_unique(const V& v);   // 0x004AC380
    template <class II> void insert_unique(II first, II last)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }
};
template void _Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString>, allocator<CCharString> >
    ::insert_unique<const CCharString*>(const CCharString* first, const CCharString* last);
}