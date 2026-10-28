#include <stdio.h>
#include <string.h>
// std::_Rb_tree_iterator::operator++(int) -- Return the old iterator and advance to its successor.
// Leaf: ??E?$_Rb_tree_iterator@VCUnprovenValue_006da200@@U?$_Nonconst_traits@VCUnprovenValue_006da200@@@std@@@std@@QAE?AU01@H@Z
class CUnprovenValue_006da200;
namespace std {
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template<class Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x) {
        if(x->_M_right) {
            x=x->_M_right;
            while(x->_M_left) x=x->_M_left;
        } else {
            _Rb_tree_node_base* y=x->_M_parent;
            while(x==y->_M_right) { x=y; y=y->_M_parent; }
            if(x->_M_right!=y) x=y;
        }
        return x;
    }
};
template<class Value> struct _Nonconst_traits {};
struct _Rb_tree_base_iterator { _Rb_tree_node_base* _M_node; };
template<class Value,class Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<Value,Traits> _Self;
    _Rb_tree_iterator() { _M_node=0; }
    _Rb_tree_iterator(const _Self& x) { _M_node=x._M_node; }
    _Self operator++(int) {
        _Self old=*this;
        _M_node=_Rb_global<bool>::_M_increment(_M_node);
        return old;
    }
};
template _Rb_tree_iterator<CUnprovenValue_006da200,_Nonconst_traits<CUnprovenValue_006da200> > _Rb_tree_iterator<CUnprovenValue_006da200,_Nonconst_traits<CUnprovenValue_006da200> >::operator++(int);
}

int main() {
    std::_Rb_tree_node_base h,n1,n2,n3,n4;
    memset(&h,0,sizeof(h));memset(&n1,0,sizeof(n1));memset(&n2,0,sizeof(n2));memset(&n3,0,sizeof(n3));memset(&n4,0,sizeof(n4));
    h._M_parent=&n2;h._M_left=&n1;h._M_right=&n4;
    n2._M_parent=&h;n2._M_left=&n1;n2._M_right=&n4;
    n1._M_parent=&n2;n4._M_parent=&n2;n4._M_left=&n3;n3._M_parent=&n4;
    typedef std::_Rb_tree_iterator<CUnprovenValue_006da200,std::_Nonconst_traits<CUnprovenValue_006da200> > Iterator;
    Iterator it;it._M_node=&n1;Iterator old=it++;
    int ok=old._M_node==&n1 && it._M_node==&n2;
    old=it++;ok=ok && old._M_node==&n2 && it._M_node==&n3;
    old=it++;ok=ok && old._M_node==&n3 && it._M_node==&n4;
    old=it++;ok=ok && old._M_node==&n4 && it._M_node==&h;
    puts(ok ? "006da200_TEST PASS" : "006da200_TEST FAIL");return ok ? 0 : 1;
}