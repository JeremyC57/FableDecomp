// NCameraModes::CPhysicsForceModelBase::GetPointIntersection(list<CPhysicsMeshObject>&, long, const C3DVector&, const C3DVector&, C3DVector&) -- first mesh object the segment from->to hits; its intersection position goes to out
// Leaf: ?GetPointIntersection@CPhysicsForceModelBase@NCameraModes@@
// Retail 0x008FBCB0, 0x6C bytes, thiscall `ret 14h` (`this` and the long are unused), bool. Manifest label
// "MeshContainer_RayIntersectionQuery" (_global) is descriptive. PDB name set:
// `?GetPointIntersection@CPhysicsForceModelBase@NCameraModes@@IAE_NAAV?$list@VCPhysicsMeshObject@@...@JABVC3DVector@@1AAV5@@Z`
// (the list overload; the other overload takes a CWorldMap). No direct callers (reached through the UpdateScan
// template's inlined body / not referenced by a call: xref 0).
// Callee: CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector&, const C3DVector&, CMeshIntersectionData&)
// const @ 0x00A12450.
// Layout: retail STLport list = one header-node pointer, node next +0 / prev +4 / CPhysicsMeshObject +8 (0x40 bytes:
// vptr, Mesh, PositionMatrix, Scale, OO_Scale); CMeshIntersectionData 0x1C bytes (IntersectionPosition +0, Normal +0xC,
// DistanceSquared +0x18) is the whole `sub esp,1Ch` frame.
// Shape: the iterator is advanced BEFORE the call (`mov eax,esi ; mov esi,[esi]`): `(it++)->...` on a node-pointer
// iterator; end() is re-read from the list after every call (`cmp esi,[ebp]`).
struct C3DVector {
    float X, Y, Z;
};

struct CMeshIntersectionData {
    C3DVector IntersectionPosition;             // +0x00
    C3DVector Normal;                           // +0x0c
    float     DistanceSquared;                  // +0x18
    CMeshIntersectionData() { }
};

struct CPhysicsMeshObject {
    virtual void v00();
    void*         Mesh;                         // +0x04 CIVCountedPointer<const CPhysicsMesh>
    float         PositionMatrix[12];           // +0x08 CMatrix3x4
    float         Scale;                        // +0x38
    float         OO_Scale;                     // +0x3c
    bool GetPointIntersectionFromTo(const C3DVector& from, const C3DVector& to, CMeshIntersectionData& data) const;   // 0x00A12450
};

namespace std {
template <class _Tp> class allocator { };

struct _List_node_base {
    _List_node_base* _M_next;                   // +0x00
    _List_node_base* _M_prev;                   // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                // +0x08
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator operator++(int) { _List_iterator __tmp = *this; _M_node = _M_node->_M_next; return __tmp; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                    // STLport list, 4 bytes in retail (the header node pointer)
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
}

namespace NCameraModes {

class CPhysicsForceModelBase {
public:
    bool GetPointIntersection(std::list<CPhysicsMeshObject>& meshes, long unused, const C3DVector& from,
                              const C3DVector& to, C3DVector& out);
};

bool CPhysicsForceModelBase::GetPointIntersection(std::list<CPhysicsMeshObject>& meshes, long, const C3DVector& from,
                                                  const C3DVector& to, C3DVector& out)
{
    CMeshIntersectionData data;
    for (std::list<CPhysicsMeshObject>::iterator it = meshes.begin(); it != meshes.end(); ) {
        if ((it++)->GetPointIntersectionFromTo(from, to, data)) {
            out = data.IntersectionPosition;
            return true;
        }
    }
    return false;
}

}