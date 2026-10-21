#include <cstdio>
class CWideString {
public:
    void* m_pData;
    CWideString(const CWideString& s);                   // 0x0099B720
    ~CWideString();                                      // 0x0099B510
};
namespace std {
template <class T> struct less {
    bool operator()(const T& x, const T& y) const;
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    __forceinline pair(const T1& a, const T2& b) : first(a), second(b) {}
    template <class U1, class U2> pair(const pair<U1, U2>& p);   // 0x00485125 for <CWideString,bool> -> <const CWideString,bool>
};
template <class T1, class T2> __forceinline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& y) const { return _M_node != y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_find(const K& k) const;       // 0x004856CD
    pair<iterator, bool> insert_unique(const value_type& v);   // 0x00485E5C
    iterator find(const K& k) { return iterator(_M_find(k)); }
    iterator end() { return iterator(_M_header); }
    pair<iterator, bool> insert(const value_type& v) { return insert_unique(v); }
};
}
class CUserProfileManager {
public:
    static bool __fastcall VerifySignature(const CWideString& filename);   // 0x00409730
};
namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char _pad00[0xc4];
    std::map<CWideString, bool> Signatures;              // +0xc4 (Ego_r.pdb +0xd8)
    bool VerifySignatureCached(const CWideString& filename);
};
bool CFrontEndManager::VerifySignatureCached(const CWideString& filename)
{
    std::map<CWideString, bool>::iterator i = Signatures.find(filename);
    if (i != Signatures.end())
        return (*i).second;
    bool valid = CUserProfileManager::VerifySignature(filename);
    Signatures.insert(std::make_pair(filename, valid));
    return valid;
}
}

static int g_verifies, g_inserts, g_copies, g_dtors; static bool g_answer;
CWideString::CWideString(const CWideString& s) { m_pData = s.m_pData; ++g_copies; }
CWideString::~CWideString() { ++g_dtors; }
bool __fastcall CUserProfileManager::VerifySignature(const CWideString& f) { ++g_verifies; return g_answer; }
namespace std {
template <class T1, class T2> template <class U1, class U2> pair<T1, T2>::pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
static _Rb_tree_node<pair<const CWideString, bool> >* g_nodes[4]; static int g_count;
template <class K, class T, class C> _Rb_tree_node_base* map<K, T, C>::_M_find(const K& k) const
{
    for (int i = 0; i < g_count; ++i) if (g_nodes[i]->_M_value_field.first.m_pData == k.m_pData) return g_nodes[i];
    return _M_header;
}
template <class K, class T, class C> pair<typename map<K, T, C>::iterator, bool> map<K, T, C>::insert_unique(const value_type& v)
{
    ++g_inserts;
    _Rb_tree_node<value_type>* n = (_Rb_tree_node<value_type>*)operator new(sizeof(_Rb_tree_node<value_type>));
    *(void**)&n->_M_value_field.first = v.first.m_pData; *(bool*)&n->_M_value_field.second = v.second;
    g_nodes[g_count++] = (_Rb_tree_node<pair<const CWideString, bool> >*)n;
    return pair<iterator, bool>(iterator(n), true);
}
}
int main()
{
    static std::_Rb_tree_node_base header;
    static unsigned char raw[sizeof(NUISystem::CFrontEndManager)];
    NUISystem::CFrontEndManager& m = *(NUISystem::CFrontEndManager*)raw; m.Signatures._M_header = &header;
    struct R { void* p; } a, b; a.p = (void*)"save1"; b.p = (void*)"save2";
    const CWideString& fa = *(CWideString*)&a; const CWideString& fb = *(CWideString*)&b;
    g_answer = true; bool ok = m.VerifySignatureCached(fa) && g_verifies == 1 && g_inserts == 1;
    ok = ok && g_copies == 2 && g_dtors == 2;                          // pair copy + value_type copy, both destroyed
    g_answer = false; ok = ok && m.VerifySignatureCached(fa) && g_verifies == 1 && g_inserts == 1;   // cached
    ok = ok && !m.VerifySignatureCached(fb) && g_verifies == 2 && g_inserts == 2;
    g_answer = true; ok = ok && !m.VerifySignatureCached(fb) && g_verifies == 2;
    std::printf(ok ? "005957d9_TEST PASS\n" : "005957d9_TEST FAIL\n");
    return ok ? 0 : 1;
}