#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __cdecl
#define __declspec(x) __attribute__((x))
#endif
#define private public
// std::_Rb_tree::_M_insert(x, y, value, hint) -- Link a new node and rebalance the tree.
// Leaf: ?_M_insert@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCUnprovenMapped_006f2110@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCUnprovenMapped_006f2110@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCUnprovenMapped_006f2110@@@std@@@3@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCUnprovenMapped_006f2110@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCUnprovenMapped_006f2110@@@std@@@2@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBVCCharString@@VCUnprovenMapped_006f2110@@@2@0@Z
#include <stddef.h>
#include <stdlib.h>
inline void* operator new(size_t,void* p) {return p;}
template<class T> class CBasicString {
public:
    T* PData;
#pragma auto_inline(off)
    __declspec(noinline) bool operator<(const CBasicString& other) const {
        const T* b=other.PData;const T* a=PData;
        for(;;) {
            if(*a==0 && *b==0)return false;
            if(*a<*b)return true;
            if(*a>*b)return false;
            ++a;++b;
        }
    }
#pragma auto_inline(on)
};
class CCharString {
public:
    CBasicString<char>* PStringData;
    CCharString();
    CCharString(const CCharString&);
    bool operator<(const CCharString& other) const {
        if(other.PStringData==PStringData)return false;
        if(PStringData==0)return true;
        if(other.PStringData==0)return false;
        return *PStringData < *other.PStringData;
    }
};
class CUnprovenMapped_006f2110 { public: unsigned int Bits; };
namespace std {
template<class A,class B> struct pair {
    typedef A first_type;A first;B second;
    pair(const A& a,const B& b):first(a),second(b){}
    pair(const pair& x):first(x.first),second(x.second){}
};
template<class T> struct less { bool operator()(const T& a,const T& b) const {return a<b;} };
template<class P> struct _Select1st { const typename P::first_type& operator()(const P& p) const {return p.first;} };
template<class T> class allocator { public: T* allocate(unsigned int n){return (T*)malloc(n*sizeof(T));} };
struct _Rb_tree_node_base { bool _M_color;_Rb_tree_node_base* _M_parent;_Rb_tree_node_base* _M_left;_Rb_tree_node_base* _M_right; };
template<class V> struct _Rb_tree_node : public _Rb_tree_node_base { V _M_value_field; };
template<class Dummy> class _Rb_global { public: static void __cdecl _Rebalance(_Rb_tree_node_base*,_Rb_tree_node_base*&); };
template<class T> struct _Nonconst_traits {};
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template<class V,class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* p){_M_node=p;}
};
template<class K,class V,class KeyOfValue,class Compare,class Alloc=allocator<V> > class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    typedef _Rb_tree_iterator<V,_Nonconst_traits<V> > iterator;
    _Link_type _M_header;
    unsigned int _M_node_count;
    Compare _M_key_compare;
    static const K& _S_key(_Rb_tree_node_base* p){return KeyOfValue()(((_Link_type)p)->_M_value_field);}
protected:
    _Link_type _M_create_node(const V& value) {
        _Link_type result=allocator<_Rb_tree_node<V> >().allocate(1);
        new(&result->_M_value_field) V(value);
        return result;
    }
private:
    iterator _M_insert(_Rb_tree_node_base* x,_Rb_tree_node_base* y,const V& v,_Rb_tree_node_base* w);
};
template<class K,class V,class KoV,class C,class A>
typename _Rb_tree<K,V,KoV,C,A>::iterator _Rb_tree<K,V,KoV,C,A>::_M_insert(_Rb_tree_node_base* x,_Rb_tree_node_base* y,const V& v,_Rb_tree_node_base* w)
{
    _Link_type z;
    if(y==_M_header || (w==0 && (x!=0 || _M_key_compare(KoV()(v),_S_key(y))))) {
        z=_M_create_node(v);y->_M_left=z;
        if(y==_M_header){_M_header->_M_parent=z;_M_header->_M_right=z;}
        else if(y==_M_header->_M_left)_M_header->_M_left=z;
    } else {
        z=_M_create_node(v);y->_M_right=z;
        if(y==_M_header->_M_right)_M_header->_M_right=z;
    }
    z->_M_parent=y;z->_M_left=0;z->_M_right=0;
    _Rb_global<bool>::_Rebalance(z,_M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}
}
typedef std::pair<const CCharString,CUnprovenMapped_006f2110>  Value;
typedef std::_Rb_tree<CCharString,std::pair<const CCharString,CUnprovenMapped_006f2110> ,std::_Select1st<std::pair<const CCharString,CUnprovenMapped_006f2110> >,std::less<CCharString> > Tree;
template Tree::iterator Tree::_M_insert(std::_Rb_tree_node_base*,std::_Rb_tree_node_base*,const Value&,std::_Rb_tree_node_base*);

#undef private
static int copies,rebalance_calls,bad;
static std::_Rb_tree_node_base* allocated[8];
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString& x):PStringData(x.PStringData){++copies;}
namespace std {
template<class D> void __cdecl _Rb_global<D>::_Rebalance(_Rb_tree_node_base* node,_Rb_tree_node_base*& root) {
    if(!node->_M_parent || node->_M_left || node->_M_right || !root)bad=1;
    if(node->_M_parent->_M_left!=node && node->_M_parent->_M_right!=node)bad=1;
    allocated[rebalance_calls++]=node;
}
}
int main() {
    char m[]="middle",a[]="alpha",z[]="zulu";
    CBasicString<char> bm,ba,bz;bm.PData=m;ba.PData=a;bz.PData=z;
    CCharString km,ka,kz;km.PStringData=&bm;ka.PStringData=&ba;kz.PStringData=&bz;
    CUnprovenMapped_006f2110 mapped;mapped.Bits=37;
    Value vm(km,mapped),va(ka,mapped),vz(kz,mapped);
    Tree tree;Tree::_Link_type header=(Tree::_Link_type)malloc(sizeof(std::_Rb_tree_node<Value>));
    header->_M_parent=0;header->_M_left=header;header->_M_right=header;
    tree._M_header=header;tree._M_node_count=0;copies=0;
    Tree::iterator root=tree._M_insert(0,header,vm,0);
    int ok=header->_M_parent==root._M_node && header->_M_left==root._M_node && header->_M_right==root._M_node;
    Tree::iterator left=tree._M_insert(0,root._M_node,va,0);
    Tree::iterator right=tree._M_insert(0,root._M_node,vz,0);
    ok=ok && !bad && tree._M_node_count==3 && copies==3 && rebalance_calls==3;
    ok=ok && root._M_node->_M_left==left._M_node && root._M_node->_M_right==right._M_node;
    ok=ok && header->_M_left==left._M_node && header->_M_right==right._M_node;
    for(int j=0;j<3;++j){Tree::_Link_type n=(Tree::_Link_type)allocated[j];ok=ok && n->_M_value_field.second.Bits==37;}
    Tree::iterator forced=tree._M_insert(0,right._M_node,va,right._M_node);
    ok=ok && right._M_node->_M_right==forced._M_node && header->_M_right==forced._M_node && tree._M_node_count==4;
    Tree::iterator forcedLeft=tree._M_insert(left._M_node,left._M_node,vz,0);
    ok=ok && left._M_node->_M_left==forcedLeft._M_node && header->_M_left==forcedLeft._M_node && tree._M_node_count==5;
    for(int k=0;k<rebalance_calls;++k){Tree::_Link_type n=(Tree::_Link_type)allocated[k];n->_M_value_field.~Value();free(n);}
    free(header);
    puts(ok ? "006f2110_TEST PASS" : "006f2110_TEST FAIL");return ok ? 0 : 1;
}