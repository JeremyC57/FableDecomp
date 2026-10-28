// CTCAIScratchpad::AddGossipVillages(CCharString name) -- Create an empty category only if it is absent.
// Leaf: ?AddGossipVillages@CTCAIScratchpad@@SIXVCCharString@@@Z
// Identity: PDB single-string static overload; own map013bae50 and find008ac200/operator[]007aebe0/vector assignment00432ee9.
#include <new>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __declspec(x)
#endif
// Layout: real STLport map12/vector12, CCharString one pointer. Only static CTCAIScratchpad members are used.
class CCharString {
public:
    CCharString& operator=(const CCharString&);
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator<(const CCharString& other) const;
    void* PStringData;                                   // +0x00
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
template<class T,class A>class _Vector_base {public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,A> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A> {public:
vector():_Vector_base<T,A>(){}
~vector(){for(T* p=this->_M_start;p!=this->_M_finish;++p)p->~T();if(this->_M_start)free(this->_M_start);}
unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}
unsigned int capacity()const{return (unsigned int)(this->_M_end_of_storage._M_data-this->_M_start);}
__declspec(noinline) vector& operator=(const vector& other){if(this!=&other){unsigned int n=other.size();if(n>capacity()){T* p=n?(T*)malloc(n*sizeof(T)):0;for(unsigned int i=0;i<n;++i)new(p+i)T(other._M_start[i]);for(T* old=this->_M_start;old!=this->_M_finish;++old)old->~T();if(this->_M_start)free(this->_M_start);this->_M_start=p;this->_M_finish=p+n;this->_M_end_of_storage._M_data=p+n;}else{unsigned int oldSize=size();unsigned int commonSize=oldSize<n?oldSize:n;for(unsigned int i=0;i<commonSize;++i)this->_M_start[i]=other._M_start[i];if(n<oldSize){for(T* p=this->_M_start+n;p!=this->_M_finish;++p)p->~T();}else{for(unsigned int j=oldSize;j<n;++j)new(this->_M_start+j)T(other._M_start[j]);}this->_M_finish=this->_M_start+n;}}return *this;}
};
struct _Rb_tree_node_base {
    bool _M_color;                                       // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
template <class _Tp> struct _Nonconst_traits { };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _STLP_alloc_proxy<_Link_type, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    unsigned int _M_node_count;                                                    // +0x04
    _Compare _M_key_compare;                                                       // +0x08
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    _Link_type _M_find(const _Key& __k) const;                                     // 0x008AC200 for this tree
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    _Tp& operator[](const _Key& __k);                                              // 0x007AEBE0 for this map
};
}   // namespace std
class CTCAIScratchpad {public:static std::map<CCharString,std::vector<CCharString> > GossipVillages;static void __fastcall AddGossipVillages(CCharString name);};
void __fastcall CTCAIScratchpad::AddGossipVillages(CCharString name){std::map<CCharString,std::vector<CCharString> >::iterator it=GossipVillages.find(name);if(it==GossipVillages.end())GossipVillages[name]=std::vector<CCharString>();}