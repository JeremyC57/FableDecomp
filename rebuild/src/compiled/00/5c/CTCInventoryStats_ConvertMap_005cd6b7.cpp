// CTCInventoryStats::ConvertMap(const map<unsigned long, long>& creatures, list<pair<CWideString, long> >& out) -- folds a creatures-killed map (creature group -> count) into a list of (group display name, count) pairs, summing the counts of groups that share a display name
// Leaf: ?ConvertMap@CTCInventoryStats@@
// Retail: Fable.exe @ 0x005CD6B7, 176 bytes, __fastcall `ret 8`. Ego_r.pdb
// `?ConvertMap@CTCInventoryStats@@QAEXABV?$map@KJU?$less@K@std@@...@std@@AAV?$list@U?$pair@VCWideString@@J@std@@...@3@@Z`.
// Callers: 0x005CE97C, 0x005D2F72 (the stats screen's kill lists).
// Callees: CTCInventoryStats::GetCreatureGroupAsString(unsigned long) @ 0x005CC9AD (CWideString by hidden pointer),
// CWideString::operator=='s Compare_Unsigned16Array @ 0x0040F0A0 (cdecl bool), CWideString::CWideString(const CWideString&)
// @ 0x0099B720, ~CWideString @ 0x0099B510, list<pair<CWideString, long> >::push_back(const value_type&) @ 0x005D86B2,
// std::_Rb_global<bool>::_M_increment(_Rb_tree_node_base*) @ 0x004292C0 (STLport, cdecl).
// Shape: frame-pointer size TU; `this` spilled to [ebp-8] (edi = map node, esi = list node, ebx = the cached list end);
// the name search is a for loop over a cached end() with a found flag: the match adds the count in a block VC puts after
// the epilogue and the flag is threaded away; otherwise push_back of a pair temporary built by the pair constructor
// (copy, then the count; `__forceinline`, a plain inline stays a call under /Os: 167v176) and destroyed after the call.
// A break + `li != out.end()` re-test keeps `out` in ebx, re-reads end() and homes the name in the dead `out` slot.
// Layout: STLport map header node (begin = header->_M_left +8; key +0x10, value +0x14), list sentinel (_M_next +0, data +8).
// Label: the catalog's CTCInventoryStats_AggregateCreatureGroupStats is this method.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return Compare_Unsigned16Array(PStringData, other.PStringData);
    }
    static bool __cdecl Compare_Unsigned16Array(const void* a, const void* b);   // 0x0040F0A0
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    __forceinline pair(const T1& a, const T2& b) : first(a), second(b) {}   // /Os: plain inline stays a call
};
struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};
template <class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* node);   // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class K> struct less { };
template <class K, class V, class Cmp = less<K> > class map {
public:
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    class const_iterator {
    public:
        _Rb_tree_node_base* _M_node;
        const_iterator(_Rb_tree_node_base* node) : _M_node(node) {}
        const value_type* operator->() const { return &static_cast<_Node*>(_M_node)->_M_value_field; }
        const_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
        bool operator!=(const const_iterator& other) const { return _M_node != other._M_node; }
    };
    _Rb_tree_node_base* _M_header;                      // +0x00 (the allocated header node)
    const_iterator begin() const { return _M_header->_M_left; }
    const_iterator end() const { return _M_header; }
};
template <class T> struct _List_node {
    _List_node* _M_next;                                // +0x00
    _List_node* _M_prev;                                // +0x04
    T           _M_data;                                // +0x08
};
template <class T> class list {
public:
    class iterator {
    public:
        _List_node<T>* _M_node;
        iterator() {}
        iterator(_List_node<T>* node) : _M_node(node) {}
        T* operator->() const { return &_M_node->_M_data; }
        iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const iterator& other) const { return _M_node != other._M_node; }
    };
    _List_node<T>* _M_node;                             // +0x00 (the sentinel)
    iterator begin() { return _M_node->_M_next; }
    iterator end() { return _M_node; }
    void push_back(const T& value);                     // 0x005D86B2 for pair<CWideString, long>
};
}
class CTCInventoryStats {
public:
    CWideString GetCreatureGroupAsString(unsigned long group);   // 0x005CC9AD
    void ConvertMap(const std::map<unsigned long, long>& creatures, std::list<std::pair<CWideString, long> >& out);
};
void CTCInventoryStats::ConvertMap(const std::map<unsigned long, long>& creatures, std::list<std::pair<CWideString, long> >& out)
{
    for (std::map<unsigned long, long>::const_iterator it = creatures.begin(); it != creatures.end(); ++it) {
        CWideString name = GetCreatureGroupAsString(it->first);
        std::list<std::pair<CWideString, long> >::iterator end = out.end();
        bool found = false;
        for (std::list<std::pair<CWideString, long> >::iterator li = out.begin(); li != end; ++li) {
            if (li->first == name) {
                li->second += it->second;
                found = true;
                break;
            }
        }
        if (!found)
            out.push_back(std::pair<CWideString, long>(name, it->second));
    }
}