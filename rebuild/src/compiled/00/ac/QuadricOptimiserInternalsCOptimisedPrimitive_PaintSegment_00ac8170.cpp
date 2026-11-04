// QuadricOptimiserInternals::COptimisedPrimitive::PaintSegment(unsigned long face, long segment, std::list<long>& faces) -- flood-fill one mesh segment: mark the start face with the segment number, then breadth-first through each face's three adjacency slots, marking every existing, still unpainted (-1) neighbour and appending each visited face to the caller's list
// Leaf: ?PaintSegment@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXKJAAV?$list@JV?$allocator@J@std@@@std@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC8170, 295 bytes, thiscall, `ret 0Ch`. Speed TU (no pragma).
// Identity (proven): Ego_r.pdb ?PaintSegment@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXKJAAV?$list@JV?$allocator@J@std@@@std@@@Z
//   (private; unsigned long, long, list<long>&). Its sole caller 0x00AC85A2 is ?PaintSegments@COptimisedPrimitive@QuadricOptimiser
//   Internals@@AAEXXZ (0x00AC8540): for every existing, unpainted face it builds a local std::list<long>, calls
//   PaintSegment(i, SegmentCount, list) and increments SegmentCount (+0x40). The body walks COptimisedFace (Ego_r.pdb, 0x48 bytes:
//   Edges[3] +0x18 as the adjacency slots, Exists +0x3C, Segment +0x44) in Faces (+0x1C, CArray at STLport size 0xC). The catalog
//   label PaintSegments_AddSegmentLink is a propagated guess.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport list node allocator: header, push_back nodes, pop_front, ~list).
// Shape: the open list is an STLport list<long> local (header allocated by the _List_base ctor, nodes by push_back's
//   _M_create_node with the placement-new null test). Retail reads Faces' data pointer BEFORE the local list is constructed and
//   stores the start face's segment after it: a `COptimisedFace* faceData = &Faces[0];` local taken first gives exactly that
//   (a `Faces[face]` store before the list stores before malloc; after it reloads through this; a reference to Faces[face] taken
//   first computes the full address early: probes 1-3). Inside the loop `COptimisedFace& currentFace = Faces[current]` is taken
//   after faces.push_back (one edge pointer per pass, counter in the dead `face` argument slot); the neighbour tests re-index
//   Faces (push_back may run between).
// Layout: engine/COptimisedPrimitive.h is the flattened Ego_r layout (containers as byte arrays, Parameters split), so the class
//   is restated with its real member types (CArray<T> over the STLport vector); COptimisedFace has no engine header.
//   STLport 4.6 text (stl/_list.h, _list.c, _vector.h) reduced to what the body uses.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp> struct _Const_traits { };
// ---- vector (stl/_vector.h), the CArray base
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp& reference;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    reference operator[](unsigned int __n) { return *(_M_start + __n); }
};
// ---- list (stl/_list.h, stl/_list.c)
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _List_base(const allocator_type& __a) : _M_node(__a, (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_base<_Tp, _Alloc> _Base;
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef _Tp& reference;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
    ~list() { }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    reference front() { return *begin(); }
    template <class _T1, class _T2> static void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
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
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_front() { erase(begin()); }
};
} // namespace std

template <class T> class CArray : public std::vector<T> { };

#include "engine/C3DVector.h"
class CChunkedFileChunk;
#include "engine/CCountedPointer.h"
namespace QuadricOptimiserInternals {
class CUniqueVertex;
class COptimisedVertex;
class COptimisedEdge;
class CVertexPairContraction;
class COptimisedFace {                                   // Ego_r.pdb, 0x48 bytes
public:
    unsigned long VertexIndices[3];                      // +0x00
    unsigned long UniqueVertexIndices[3];                // +0x0C
    long Edges[3];                                       // +0x18
    C3DVector UTangent;                                  // +0x24
    C3DVector VTangent;                                  // +0x30
    bool Exists;                                         // +0x3C
    unsigned long OutputIndex;                           // +0x40
    long Segment;                                        // +0x44
};
class COptimisedPrimitive {
public:
    long MaterialIndex;                                  // +0x00
    CArray<CUniqueVertex> UniqueVertices;                // +0x04
    CArray<COptimisedVertex> Vertices;                   // +0x10
    CArray<COptimisedFace> Faces;                        // +0x1C
    CArray<COptimisedEdge> Edges;                        // +0x28
    unsigned char VertexPairContractions[0xc];           // +0x34 multimap<float, CCountedPointer<CVertexPairContraction> >
    long SegmentCount;                                   // +0x40
    CArray<float> SegmentSizes;                          // +0x44
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunksToKeep;   // +0x50
private:
    void PaintSegment(unsigned long face, long segment, std::list<long>& faces);
};

void COptimisedPrimitive::PaintSegment(unsigned long face, long segment, std::list<long>& faces)
{
    COptimisedFace* faceData = &Faces[0];
    std::list<long> open;
    faceData[face].Segment = segment;
    open.push_back(face);
    while (!open.empty()) {
        long current = open.front();
        open.pop_front();
        faces.push_back(current);
        COptimisedFace& currentFace = Faces[current];
        for (long i = 0; i < 3; ++i) {
            long adjacent = currentFace.Edges[i];
            if (adjacent != -1 && Faces[adjacent].Exists && Faces[adjacent].Segment == -1) {
                Faces[adjacent].Segment = segment;
                open.push_back(adjacent);
            }
        }
    }
}
}