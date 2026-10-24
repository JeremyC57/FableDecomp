// Behaviour test for 009e5200 CSymbolMap::CSymbolMap() (copy of the model; malloc is the CRT one, the tree header is checked).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_base = 0;
struct CBase { CBase() { ++g_base; } virtual ~CBase() {} };
struct _Rb_tree_node_base { unsigned char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template <class V> struct _Rb_tree_node : _Rb_tree_node_base { V _M_value_field; };
template <class V> struct _Rb_tree_base {
    _Rb_tree_node<V>* _M_header;
    _Rb_tree_base() : _M_header(0)
    {
        _M_header = (_Rb_tree_node<V>*)malloc(sizeof(_Rb_tree_node<V>));
    }
};
template <class K, class V> struct _Rb_tree : _Rb_tree_base<V> {
    unsigned int _M_node_count;
    char         _M_key_compare;
    _Rb_tree() : _M_node_count(0)
    {
        this->_M_header->_M_color = 0;
        this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header;
        this->_M_header->_M_right = this->_M_header;
    }
};
template <class K> struct set { _Rb_tree<K, K> _M_t; };
template <class T> struct CAllocProxy { T* _M_data; CAllocProxy(T* p) : _M_data(p) {} };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> struct CKeyValuePair { K first; V second; };
template <class K, class V> struct CVectorMap {
    CVector<CKeyValuePair<K, V> > Data;
    char                          Compare;
    bool                          Dirty;
    CVectorMap() : Dirty(false) {}
};
struct CSymbolMap : CBase {
    CVectorMap<unsigned long, long> SLONGMap;
    set<unsigned long>              NamesUsed;
    bool                            CheckForDuplicateSymbols;
    CSymbolMap();
};
CSymbolMap::CSymbolMap()
    : CheckForDuplicateSymbols(false)
{
}
int main()
{
    static unsigned char raw[sizeof(CSymbolMap)];
    std::memset(raw, 0xCD, sizeof(raw));
    CSymbolMap* m = new (raw) CSymbolMap;
    int ok = 1;
    if (g_base != 1 || m->CheckForDuplicateSymbols) ok = 0;
    if (m->SLONGMap.Data._M_start || m->SLONGMap.Data._M_finish || m->SLONGMap.Data._M_end_of_storage._M_data || m->SLONGMap.Dirty) ok = 0;
    _Rb_tree_node_base* h = m->NamesUsed._M_t._M_header;
    if (!h || m->NamesUsed._M_t._M_node_count != 0) ok = 0;
    if (ok && (h->_M_color != 0 || h->_M_parent != 0 || h->_M_left != h || h->_M_right != h)) ok = 0;
    free(h);
    if (!ok) { std::printf("009e5200_TEST FAIL\n"); return 1; }
    std::printf("009e5200_TEST PASS\n");
    return 0;
}