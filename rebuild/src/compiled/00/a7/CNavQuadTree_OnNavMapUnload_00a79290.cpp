// CNavQuadTree::OnNavMapUnload(long mapIndex) -- a neighbouring navigation map was unloaded: erase every connector node registered under its map index (the equal range of the ConnectorNodes map)
// Leaf: ?OnNavMapUnload@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79290, 74 bytes (+ int3 padding to 80), thiscall `ret 4`.
// Identity: Ego_r.pdb `?OnNavMapUnload@CNavQuadTree@@QAEXJ@Z`; the only caller is CNavigatorManager::RemoveNavigationMap
//   (0x00A155F0: every other map's quad tree gets the unloaded index). ConnectorNodes (CVectorMap<long,
//   CCountedPointer<CNavConnectorNode> >, 12-byte entries) sits at PDB +0x9c -> retail +0x78 (five PDB vectors 16 -> 12,
//   the CVectorMap 0x14 -> 0x10, the list 0xc -> 4). The catalog's "CVectorMap_EraseRange_Keys" is descriptive.
//   Engine header CNavQuadTree.h carries the PDB offsets, so CNavQuadTree is modelled locally.
// Callees: CVectorMap::LowerBound 0x00A7DD40, CVectorMap::UpperBound 0x00A7DDA0 (landed), STLport __copy(first, last,
//   result, const random_access_iterator_tag&, int*) 0x00A80DA0, __destroy_aux(first, last, const __false_type&)
//   0x00A80E40 (landed).
// Shape: the bounds are named locals (LowerBound called first), then STLport's inline `vector::erase(first, last)`:
//   `i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i;`. Both empty tags are homed on the dead
//   key slot because __copy and __destroy_aux have same-TU noinline bodies that never touch their tags.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct SCountBlock {
    long  m_references;                                     // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0
    SCountBlock* Info;                                      // +4
    void Release()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            Release();
            Data = other.Data;
            Info = other.Info;
            if (Info) ++Info->m_references;
        }
        else
            Data = other.Data;
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

class CNavConnectorNode;

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
};

#pragma auto_inline(off)
// 0x00A80DA0 / 0x00A80E40: same-TU stand-ins for the out-of-line copy and destroy loops; neither touches its tag.
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        (*__first).second.~CCountedPointer();
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter>
inline _OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
}

template <class K, class V> class CKeyPairCompareLess {
};

template <class K, class V, class C> class CVectorMap : public _STL::vector<_STL::pair<K, V> > {
public:
    typedef _STL::pair<K, V>* iterator;
    C    Compare;                                           // +0x0c
    bool Dirty;                                             // +0x0d
    iterator LowerBound(const K& key);                      // 0x00A7DD40
    iterator UpperBound(const K& key);                      // 0x00A7DDA0
};

class CNavQuadTree {
public:
    unsigned char _pad00[0x78];                             // vptr, flags, map data, vectors, SwitchableNodes, Regions
    CVectorMap<long, CCountedPointer<CNavConnectorNode>, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > >
                  ConnectorNodes;                           // +0x78
    void OnNavMapUnload(long mapIndex);
};

void CNavQuadTree::OnNavMapUnload(long mapIndex)
{
    _STL::pair<long, CCountedPointer<CNavConnectorNode> >* first = ConnectorNodes.LowerBound(mapIndex);
    _STL::pair<long, CCountedPointer<CNavConnectorNode> >* last = ConnectorNodes.UpperBound(mapIndex);
    ConnectorNodes.erase(first, last);
}