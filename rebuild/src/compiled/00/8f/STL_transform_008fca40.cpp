// _STL::transform(list<const CThing*>::iterator, list<const CThing*>::iterator, back_insert_iterator<list<CPhysicsMeshObject>>, NCameraModes::CGetMeshObject) -- append each thing's physics mesh object to the list
// Leaf: ??$transform@
// Retail 0x008FCA40, 0xC1 bytes, __fastcall (hidden back_insert_iterator result in ecx; first, last, the output
//   iterator and the empty functor on the stack, `ret 10h`). Manifest label "DoThingEvasion_BuildMeshReferenceList"
//   is descriptive; the FableWin PDB name set has the same instantiation
//   (`??$transform@V?$_List_iterator@...PBVCThing@@...V?$back_insert_iterator@V?$list@VCPhysicsMeshObject@@...VCGetMeshObject@NCameraModes@@@std@@...`).
// Callees: CTCPhysicsBase::GetPhysicsMeshObject() const @ 0x006B0850 (by value into the 0x40-byte frame temporary),
//   malloc @ 0x00BFEA0E (0x48-byte list node), ~CPhysicsMeshObject @ 0x00A127D0.
// Layout: CThing PhysicsTC +0x60; CPhysicsMeshObject (landed dtor 0x00A127D0 model) = CBaseClass vptr, Mesh
//   CIVCountedPointer<const CPhysicsMesh> +4 (count +4 of the mesh), PositionMatrix CMatrix3x4 +8, Scale +0x38,
//   OO_Scale +0x3C; vtable 0x0125D084.
// Shape: STLport 4.x `transform` (`for (; first != last; ++first, ++result) *result = op(*first); return result;`),
//   back_insert_iterator::operator= -> list::push_back -> insert(end(), x) (null-tested placement copy: the implicit
//   CPhysicsMeshObject copy with the counted pointer's `if (other.Object) ++RefCount`, the matrix as `rep movsd`).
//   The input iterator must be the STLport 4.6 shape -- `_List_iterator_base` (`_M_incr`, `==`/`!=`) under a
//   `_List_iterator` with a user copy constructor: with a plain one-struct iterator VC7.1 sinks the ebp/edi pushes
//   below the empty-range test (191v193).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                      // 0x00BFEA14
inline void* operator new(unsigned int, void* __p) { return __p; }

class CPhysicsMesh {
public:
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    mutable long RefCount;                      // +0x04
};

template <class T> class CIVCountedPointer {
public:
    T* Object;                                  // +0x00
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (other.Object)
            ++other.Object->RefCount;
    }
};

class CMatrix3x4 {                              // 12 floats
public:
    float M[12];
};

class CBaseClass {                              // vtable 0x01231710
public:
    virtual void v00();
    CBaseClass();                               // 0x0099A2D0
    ~CBaseClass();                              // 0x0099A2E0
};

class CPhysicsMeshObject : public CBaseClass {  // vtable 0x0125D084, 0x40 bytes
public:
    CIVCountedPointer<const CPhysicsMesh> Mesh; // +0x04
    CMatrix3x4 PositionMatrix;                  // +0x08
    float Scale;                                // +0x38
    float OO_Scale;                             // +0x3c
    virtual void v00();
    ~CPhysicsMeshObject();                      // 0x00A127D0
};

class CTCPhysicsBase {
public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};

class CThing {
public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x00
    _List_node_base* _M_prev;                   // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) {}
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    typedef _Tp value_type;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
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
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template <class _Container> class back_insert_iterator {
public:
    _Container* container;
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
    back_insert_iterator& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};

template <class _InputIter, class _OutputIter, class _UnaryOperation>
_OutputIter __fastcall transform(_InputIter __first, _InputIter __last, _OutputIter __result, _UnaryOperation __opr)
{
    for ( ; __first != __last; ++__first, ++__result)
        *__result = __opr(*__first);
    return __result;
}
}

namespace NCameraModes {
class CGetMeshObject {
public:
    const CPhysicsMeshObject operator()(const CThing* thing) const { return thing->PhysicsTC->GetPhysicsMeshObject(); }
};
}

namespace NCameraModes {
// Instantiation point (the CPhysicsForceModelBase TU's thing-evasion path builds its mesh list this way).
void BuildMeshList(_STL::list<const CThing*>& things, _STL::list<CPhysicsMeshObject>& meshes)
{
    _STL::transform(things.begin(), things.end(), _STL::back_insert_iterator<_STL::list<CPhysicsMeshObject> >(meshes),
                    CGetMeshObject());
}
}