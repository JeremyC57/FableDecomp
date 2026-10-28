// std::list<CPhysicsMeshObject>::insert(iterator, const CPhysicsMeshObject&) -- copy a physics mesh object into a new node linked before the position
// Leaf: ?insert@?$list@VCPhysicsMeshObject@@V?$allocator@VCPhysicsMeshObject@@@std@@@std@@QAE?AU?$_List_iterator@VCPhysicsMeshObject@@U?$_Nonconst_traits@VCPhysicsMeshObject@@@std@@@2@U32@ABVCPhysicsMeshObject@@@Z
// Retail: Fable.exe @ 0x008FC890, 99 bytes, thiscall (`this` unused: the allocator is empty), hidden iterator result at
// [esp+4], the position iterator by value, the element by reference; `ret 0Ch`.
// Identity (proven): 0x48-byte nodes {next, prev, element} whose element copy stores vtable 0x0125D084 (rtti-vtables:
// CPhysicsMeshObject) and copies Ego_r.pdb CPhysicsMeshObject (0x40: vptr, CIVCountedPointer<const CPhysicsMesh> Mesh +4
// with the IVRefCount bump, CMatrix3x4 PositionMatrix +8 as 12 dwords, float Scale +0x38, float OO_Scale +0x3C).
// Ego_r.pdb has list<CPhysicsMeshObject> (back_insert_iterator<list<CPhysicsMeshObject> >). Callers: 0x008FC960
// (push_back shape: insert(end(), x)) and 0x008FC9B0. Catalog "_gapscan::sub_008fc890".
// Callees: malloc @ 0x00BFEA0E (STLport allocator<_Node>::allocate(1)).
// Layout: local models (CPhysicsMeshObject has no engine header; CIVCountedPointeeBase.h carries the vptr as data, so the
// pointee is used only for IVRefCount). CMatrix3x4 is a union in the PDB: modelled as its 12 floats.
// Shape: STLport 4.6 insert(pos, x) with _M_create_node inline: allocate(1), the placement-new null test on &node->_M_data
// around CPhysicsMeshObject's implicit copy constructor, then the four link stores.
#include "engine/CIVCountedPointeeBase.h"

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;                           // CIVCountedPointeeBase::AddRef
    }
    CIVCountedPointeeBase* Object;                          // +0x00
};

class C3DVector {
public:
    float X, Y, Z;
};

class CMatrix3x4 {                                          // Ego_r.pdb: a 48-byte union
public:
    float M[12];
};

class CPhysicsMesh;

class CPhysicsMeshObject {                                  // vtable 0x0125D084
public:
    virtual ~CPhysicsMeshObject();                          // slot 0 (0x006A6BA0, the deleting dtor)

    CIVCountedPointer<const CPhysicsMesh> Mesh;             // +0x04
    CMatrix3x4 PositionMatrix;                              // +0x08
    float Scale;                                            // +0x38
    float OO_Scale;                                         // +0x3C
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
public:
    _Tp* _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; };

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node> _M_node;                       // sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    public:
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
};

template list<CPhysicsMeshObject>::iterator list<CPhysicsMeshObject>::insert(list<CPhysicsMeshObject>::iterator,
                                                                             const CPhysicsMeshObject&);

} // namespace std