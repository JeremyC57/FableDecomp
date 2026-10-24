#include <cstdio>
#include <cstring>
struct CCharString {
    const char* PData;
    CCharString(const char* s) : PData(s) { }
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData) return false;
        if (PData == 0) return true;
        if (other.PData == 0) return false;
        return std::strcmp(PData, other.PData) < 0;
    }
};
struct CContainedBank {
    unsigned long DataVersion, EntryCount, InfoOffset, InfoSize, Alignment;
    CContainedBank() : DataVersion(0), EntryCount(0), InfoOffset(0), InfoSize(0), Alignment(0) { }
};
template <class T> struct less { bool operator()(const T& x, const T& y) const { return x < y; } };
template <class T1, class T2> struct pair { T1 first; T2 second; pair(const T1& a, const T2& b) : first(a), second(b) { } };
typedef pair<const CCharString, CContainedBank> value_type;
struct NodeBase { NodeBase* next; };
struct NodeV : NodeBase { value_type v; NodeV(const value_type& x) : v(x) { } };
struct iterator {
    NodeBase* _M_node;
    iterator(NodeBase* x) : _M_node(x) { }
    bool operator==(const iterator& y) const { return _M_node == y._M_node; }
    value_type& operator*() const { return static_cast<NodeV*>(_M_node)->v; }
};
/* sorted circular list with header sentinel standing in for the red-black tree */
struct BankMap {
    NodeBase header; unsigned int count; less<CCharString> _M_key_compare;
    NodeBase* nodes[8]; unsigned int used;
    BankMap() : count(0), used(0) { header.next = &header; }
    ~BankMap() { unsigned int k = 0; for (k = 0; k < used; ++k) delete static_cast<NodeV*>(nodes[k]); }
    NodeBase* _M_lower_bound(const CCharString& k)
    {
        NodeBase* p = header.next;
        while (p != &header && static_cast<NodeV*>(p)->v.first < k) p = p->next;
        return p;
    }
    iterator insert_unique(iterator, const value_type& v)
    {
        NodeBase* prev = &header;
        while (prev->next != &header && static_cast<NodeV*>(prev->next)->v.first < v.first) prev = prev->next;
        NodeV* n = new NodeV(v); n->next = prev->next; prev->next = n; nodes[used++] = n; ++count;
        return iterator(n);
    }
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }
    iterator lower_bound(const CCharString& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(&header); }
    less<CCharString> key_comp() const { return _M_key_compare; }
    CContainedBank& operator[](const CCharString& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, CContainedBank()));
        return (*i).second;
    }
};
int main()
{
    BankMap m;
    m[CCharString("level")].EntryCount = 5;
    m[CCharString("audio")].EntryCount = 7;
    int bad = 0;
    if (m.count != 2) bad = 1;
    if (m[CCharString("level")].EntryCount != 5 || m[CCharString("audio")].EntryCount != 7) bad = 2;
    if (m.count != 2) bad = 3;
    if (m[CCharString("zeta")].EntryCount != 0 || m[CCharString("zeta")].Alignment != 0 || m.count != 3) bad = 4;
    if (bad) { std::printf("009ac530_TEST FAIL\n"); return 1; }
    std::printf("009ac530_TEST PASS\n");
    return 0;
}