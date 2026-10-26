#include <stdlib.h>
#include <new>
#include <stdio.h>
static int g_copies = 0;
class CCharString {
public:
    void* PData;
    CCharString(void* p) : PData(p) {}
    CCharString(const CCharString& s);
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* d) : Data(d), Info(0) {}
    CCountedPointer(const CCountedPointer& p);
};
namespace NUISystem { class CComponent; }
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    size_t              _M_node_count;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
};
}

template std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_Link_type std::_Rb_tree<CCharString, std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >, std::_Select1st<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > >, std::less<CCharString>, std::allocator<std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > > >::_M_create_node(const std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> >&);

CCharString::CCharString(const CCharString& s) : PData(s.PData) { g_copies += 1; }
template <class T> CCountedPointer<T>::CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { g_copies += 10; }
typedef std::pair<const CCharString, CCountedPointer<NUISystem::CComponent> > V;
typedef std::_Rb_tree<CCharString, V, std::_Select1st<V>, std::less<CCharString>, std::allocator<V> > T;
int main()
{
    int key; NUISystem::CComponent* comp = (NUISystem::CComponent*)&key;
    V v = { CCharString(&key), CCountedPointer<NUISystem::CComponent>(comp) }; g_copies = 0;
    T t; T::_Link_type n = t._M_create_node(v);
    int ok = n->_M_value_field.first.PData == &key && n->_M_value_field.second.Data == comp && g_copies == 11 && sizeof(*n) == 0x1c;
    free(n);
    printf(ok ? "0042be2f_TEST PASS\n" : "0042be2f_TEST FAIL\n");
    return ok ? 0 : 1;
}