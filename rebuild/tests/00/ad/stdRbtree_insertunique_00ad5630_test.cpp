#include <stdio.h>
// std::_Rb_tree::insert_unique(iterator,const value_type&) -- Original STLport hinted unique insertion with signed lexicographic pair keys.
// Leaf: ?insert_unique@?$_Rb_tree@U?$pair@JJ@std@@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@2@U?$_Select1st@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@U?$less@U?$pair@JJ@std@@@2@V?$allocator@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@@2@U32@ABU?$pair@$$CBU?$pair@JJ@std@@V?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@2@@Z
// Identity: proven. Own callers00ad6094/00ad6799 and insert00ad4620/unhinted00ad4720 establish pair<long,long> key and CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> value; original optimizer PDB template corroborates.
// Layout: genuine tree/base/node/iterator hierarchy, allocator proxy header0, nodecount4, comparator8; node value16. Retail engine vector12 within texture morph.
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define NOINLINE __attribute__((noinline))
typedef __SIZE_TYPE__ PlacementSize;
#else
#define NOINLINE __declspec(noinline)
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p){return p;}
class CCPPointerInfo { public:
    unsigned long RefCount;                                     
    void (__fastcall* DeleteFunc)(void* data);          
    void* Data;                                         
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               
    CCPPointerInfo* Info;                               
    CCountedPointer():Data(0),Info(0){}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace QuadricOptimiserInternals {
class CVertexPairContraction;
class CContractionReference {
public:
    CCountedPointer<CVertexPairContraction> Contraction;   
    unsigned long                           SubIndex;      
};
}


namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool __fastcall operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               
    _Base_ptr   _M_parent;                              
    _Base_ptr   _M_left;                                
    _Base_ptr   _M_right;                               
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                         
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   
};
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    NOINLINE _Self& operator++() { _M_increment(); return *this; }   
    NOINLINE _Self& operator--()           
    {
        if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
            _M_node = _M_node->_M_right;
        else if (_M_node->_M_left != 0) {
            _Base_ptr __y = _M_node->_M_left;
            while (__y->_M_right != 0)
                __y = __y->_M_right;
            _M_node = __y;
        }
        else {
            _Base_ptr __y = _M_node->_M_parent;
            while (_M_node == __y->_M_left) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            _M_node = __y;
        }
        return *this;
    }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template<class V,class A>class _Rb_tree_base{public:_STLP_alloc_proxy<_Rb_tree_node<V>*,allocator<_Rb_tree_node<V> > > _M_header;};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree:public _Rb_tree_base<_Value,_Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    using _Rb_tree_base<_Value,_Alloc>::_M_header;   
    unsigned int _M_node_count;                                    
    _Compare     _M_key_compare;                                   

    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    void _M_deallocate_node(_Link_type __p) { if (__p != 0) free(__p); }
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
protected:
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
public:
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    
    
    static const _Key& _S_key(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field.first; }

    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }

private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
private:
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
public:
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_unique(iterator __position, const value_type& __x);
    unsigned int size() const { return _M_node_count; }
    _Base_ptr _M_lower_bound(const key_type& __k) const;
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        __y->_M_value_field.~value_type();
        _M_deallocate_node(__y);
        --_M_node_count;
    }
};

}   

namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator __position, const value_type& __v)
{
    if (__position._M_node == _M_header._M_data->_M_left) {   
        if (size() <= 0)
            return insert_unique(__v).first;
        if (_M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
            return _M_insert(__position._M_node, __position._M_node, __v);
        else {
            bool __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if (__comp_pos_v == false)
                return __position;
            iterator __after = __position;
            ++__after;
            if (__after._M_node == _M_header._M_data)
                return _M_insert(0, __position._M_node, __v, __position._M_node);
            if (_M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else
                return insert_unique(__v).first;
        }
    }
    else if (__position._M_node == _M_header._M_data) {   
        if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
            return _M_insert(0, _M_rightmost(), __v, __position._M_node);
        else
            return insert_unique(__v).first;
    }
    else {
        iterator __before = __position;
        --__before;
        bool __comp_v_pos = _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node));
        if (__comp_v_pos && _M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v))) {
            if (_S_right(__before._M_node) == 0)
                return _M_insert(0, __before._M_node, __v, __before._M_node);
            else
                return _M_insert(__position._M_node, __position._M_node, __v);
        }
        else {
            iterator __after = __position;
            ++__after;
            bool __comp_pos_v = !__comp_v_pos;
            if (!__comp_v_pos)
                __comp_pos_v = _M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v));
            if ((!__comp_v_pos) && __comp_pos_v &&
                (__after._M_node == _M_header._M_data || _M_key_compare(_KeyOfValue()(__v), _S_key(__after._M_node)))) {
                if (_S_right(__position._M_node) == 0)
                    return _M_insert(0, __position._M_node, __v, __position._M_node);
                else
                    return _M_insert(__after._M_node, __after._M_node, __v);
            }
            else {
                if (__comp_v_pos == __comp_pos_v)
                    return __position;
                else
                    return insert_unique(__v).first;
            }
        }
    }
}
}   

namespace std{template<class T,class A>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,A> _M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:vector(){this->_M_start=this->_M_finish=this->_M_end_of_storage._M_data=0;}};}
class CCompositeBlendDesc;class CEngineTextureMorph{public:long OriginalTextureIndex,MorphID;std::vector<CCompositeBlendDesc> Blends;CEngineTextureMorph():OriginalTextureIndex(0),MorphID(0){}};
typedef std::pair<long,long> Key;typedef std::pair<const Key, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> > Value;typedef std::_Rb_tree<Key,Value,std::_Select1st<Value>,std::less<Key>,std::allocator<Value> > Tree;
template Tree::iterator Tree::insert_unique(Tree::iterator,const Value&);
namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x_, _Base_ptr __y_, const value_type& __v,
                                                                 _Base_ptr __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == _M_header._M_data ||
        (__w == 0 &&                    
         (__x != 0 ||                   
          _M_key_compare(_KeyOfValue()(__v), _S_key(__y))))) {
        __z = _M_create_node(__v);
        _S_left(__y) = __z;             
        if (__y == _M_header._M_data) {
            _M_root() = __z;
            _M_rightmost() = __z;
        }
        else if (__y == _M_leftmost())
            _M_leftmost() = __z;        
    }
    else {
        __z = _M_create_node(__v);
        _S_right(__y) = __z;
        if (__y == _M_rightmost())
            _M_rightmost() = __z;       
    }
    __z->_M_parent = __y;
    _S_left(__z) = 0;
    _S_right(__z) = 0;
    _Rb_global_inst::_Rebalance(__z, _M_header._M_data->_M_parent);
    ++_M_node_count;
    return iterator(__z);
}
}   
namespace std {
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const value_type& __v)
{
    _Link_type __y = _M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(/* __x*/ __y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
}   

namespace std{template <> void __cdecl _Rb_global<bool>::_Rebalance(_Base_ptr __x, _Base_ptr&) { __x->_M_color = true; }}
template Tree::iterator Tree::_M_insert(std::_Rb_tree_node_base*,std::_Rb_tree_node_base*,const Value&,std::_Rb_tree_node_base*);
template std::pair<Tree::iterator,bool> Tree::insert_unique(const Value&);
static Value Make(long a,long b){return Value(Key(a,b),CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>());}
static void Drop(Tree::_Link_type p){if(!p)return;Drop((Tree::_Link_type)p->_M_left);Drop((Tree::_Link_type)p->_M_right);p->_M_value_field.~Value();free(p);}
int main(){Tree t;Tree::_Link_type h=(Tree::_Link_type)malloc(sizeof(*h));h->_M_color=false;h->_M_parent=0;h->_M_left=h;h->_M_right=h;t._M_header._M_data=h;t._M_node_count=0;bool ok=true;
Tree::iterator r=t.insert_unique(t.begin(),Make(0,5));ok=ok&&(*r).first.first==0&&(*r).first.second==5;
r=t.insert_unique(t.begin(),Make(-1,9));r=t.insert_unique(t.end(),Make(1,-8));
Tree::iterator p=t.begin();++p;r=t.insert_unique(p,Make(0,3));ok=ok&&(*r).first.second==3&&t.size()==4;
p=r;r=t.insert_unique(p,Make(0,3));ok=ok&&r==p&&t.size()==4;
r=t.insert_unique(t.begin(),Make(0,7));p=r;r=t.insert_unique(p,Make(0,8));
r=t.insert_unique(t.end(),Make(-1,8));r=t.insert_unique(t.begin(),Make(1,-9));
long aa[]={-1,-1,0,0,0,0,1,1};long bb[]={8,9,3,5,7,8,-9,-8};int n=0;for(p=t.begin();p!=t.end()&&n<9;++p,++n){if(n>=8){ok=false;break;}ok=ok&&(*p).first.first==aa[n]&&(*p).first.second==bb[n];}ok=ok&&n==8&&t.size()==8;Drop(t._M_root());free(h);puts(ok?"00ad5630_TEST PASS":"00ad5630_TEST FAIL");return ok?0:1;}