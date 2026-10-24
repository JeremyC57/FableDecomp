#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00be0ac0"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

#include <cstdlib>
#include <new>
struct CRefCountedObject { LONG32 m_refCount; };
struct CRefPointer {
    CRefCountedObject* m_pObject;
    explicit CRefPointer(CRefCountedObject* p) : m_pObject(p) { if (m_pObject) ++m_pObject->m_refCount; }
    CRefPointer(const CRefPointer& other) : m_pObject(other.m_pObject) { if (m_pObject) ++m_pObject->m_refCount; }
};
struct SEntry { LONG32 m_value; bool m_flag; LONG32 m_data; CRefPointer m_pObject; };
template <class T1, class T2> struct pair { T1 first; T2 second; };
struct _Rb_tree_node_base { bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template <class V> struct _Rb_tree_node : _Rb_tree_node_base { V _M_value_field; };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class V> struct _Rb_tree {
    typedef _Rb_tree_node<V>* _Link_type;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)std::malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
        return tmp;
    }
};
typedef pair<const LONG32, SEntry> TValue;
int main()
{
    CRefCountedObject obj; obj.m_refCount = 1;
    CRefPointer ptr(&obj);
    SEntry e = { 11, true, 33, ptr };
    TValue v = { 5, e };
    _Rb_tree<TValue> tree;
    _Rb_tree_node<TValue>* n = tree._M_create_node(v);
    CHECK(n != 0 && n->_M_value_field.first == 5 && n->_M_value_field.second.m_value == 11 && n->_M_value_field.second.m_flag);
    CHECK(n->_M_value_field.second.m_data == 33 && n->_M_value_field.second.m_pObject.m_pObject == &obj && obj.m_refCount == 5);
    std::free(n);
    std::printf("00be0ac0_TEST PASS\n");
    return 0;
}