// CNavSwitchableLeafNode::~CNavSwitchableLeafNode() -- compiler-generated destructor: free the UniqueIDsOpen vector-map block, then ~CNavNavigableLeafNode
// Leaf: ??1CNavSwitchableLeafNode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A84F00, 27 bytes, thiscall, tail jump to CNavNavigableLeafNode::~CNavNavigableLeafNode 0x00A76A80.
// Identity (proven): the sole caller 0x00A84EE3 is the scalar deleting destructor 0x00A84EE0, slot 0 of vtable 0x0129DE04
//   (rtti-vtables.tsv, RTTI .?AVCNavSwitchableLeafNode@@ : CNavNavigableLeafNode : CANavQuadTreeNode : CBaseClassNonCopyable).
//   No vtable store inside: the destructor is compiler-generated (Ego_r.pdb ??1CNavSwitchableLeafNode@@UAE@XZ). The member
//   freed (+0x20) is Ego_r CNavSwitchableLeafNode::UniqueIDsOpen (CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<..> >).
//   The tail target 0x00A76A80 is the base destructor called by the CNavNavigableLeafNode ??_G 0x00A80340 (slot 0 of 0x0129DDA4).
//   The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (STLport allocator<T>::deallocate), ??1CNavNavigableLeafNode@@UAE@XZ 0x00A76A80 (tail jump).
// Layout: retail CNavSwitchableLeafNode 0x30 (engine/CNavSwitchableLeafNode.h, flattened): the CNavNavigableLeafNode base
//   (0x20), UniqueIDsOpen +0x20 = CVectorMap (STLport vector<pair<unsigned __int64, bool> > 12 bytes + Compare +0x0C + Dirty
//   +0x0D, as the landed CVectorMap::Insert 0x00A83770). The base is opaque here (its own dtor is out of line).
// TU: the PDB constructor ??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEEABV?$vector@_KV?$allocator@_K@std@@@std@@@Z is defined
//   with an empty body only to emit the vtable, the ??_G and this implicit ??1 (not graded).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x08
};
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                           // +0x0C
    bool Dirty;                                             // +0x0D
};

#include "engine/CBaseClassNonCopyable.h"

class C2DVector;
class CANavQuadTreeNode : public CBaseClassNonCopyable {
public:
    unsigned char _body[0x10 - 4];
};
class CNavRegion;
class CNavNavigableLeafNode : public CANavQuadTreeNode {    // vtable 0x0129DDA4
public:
    CNavNavigableLeafNode(const C2DVector& centre, unsigned char level, unsigned char cost, unsigned char flags);
    virtual ~CNavNavigableLeafNode();                       // 0x00A76A80
    CNavRegion*   PRegion;                                  // +0x10
    unsigned char Neighbours[8];                            // +0x14 CSmallVector<const CANavQuadTreeNode*, 8>
    unsigned char PreferabilityCost;                        // +0x1C
};

class CNavSwitchableLeafNode : public CNavNavigableLeafNode {   // vtable 0x0129DE04
public:
    CNavSwitchableLeafNode(const C2DVector& centre, unsigned char level, unsigned char cost, unsigned char flags,
                           const std::vector<unsigned __int64>& uniqueIDs);
    CVectorMap<unsigned __int64, bool, CKeyPairCompareLess<unsigned __int64, bool> > UniqueIDsOpen;   // +0x20
};

CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& centre, unsigned char level, unsigned char cost,
                                               unsigned char flags, const std::vector<unsigned __int64>& uniqueIDs)
    : CNavNavigableLeafNode(centre, level, cost, flags)
{
}