// std::_Push_heap(std::pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> >* first, int hole, int top, std::pair<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> > val, CKeyPairCompareLess<unsigned __int64,CCountedPointer<CActiveEntityScriptBase> > pred) -- percolate a UID-keyed active-entity script pointer up a max-heap, then release the by-value pointer
// Grade: DIFFER(212v207) -- ~CCountedPointer with a block local (`SCountBlock* count = m_pCount; if (count &&
//   --count->m_references == 0)`) keeps the block in esi as retail; left only: VC7.1 loads it into eax first (`mov
//   eax,[esp+24h] ; test eax,eax ; mov esi,eax`) and re-reads the count after the decrement (`dec [eax] ; cmp dword ptr
//   [esi],0`) where retail tests the flags of `dec dword ptr [esi]`. Tried this pass: nested ifs, `!--count` (identical)
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00CBABA0, 208 bytes incl. int3 padding (/O2). Callees: the pair's operator=
// 0x00CBA520 (landed SCountedRecord_00cba520::operator=), the count block's destroy function (+4,
// object at +8 in ecx), operator delete 0x00BFE9BC.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 16-byte element: an
// UNSIGNED 64-bit key +0 (high word first: `ja done ; jb move`, then `jae done`) and a
// CCountedPointer<CActiveEntityScriptBase> {object, count block} +8. The pair's implicit operator=
// (two key words + the counted pointer's assignment: release the old block when it differs, take the
// other's object/block, count up) is expanded in the loop but called out of line (0x00CBA520) for the
// final `first[hole] = val`; the natural STLport body with an inline pair operator= expands both
// sites under the lane compiler (267 bytes), so the model spells the loop copy as a __forceinline
// memberwise Assign and declares the pair's operator= out of line (review at landing). The
// by-value pointer is released in the callee (`--count == 0` test, no zero stores).
// /Gr: first=ecx, hole=edx; `ret 18h` = top + 16-byte pair + the empty comparator.
void __cdecl operator delete(void* block);                // 0x00BFE9BC
namespace N00cbaba0 {
struct SCountBlock {
    long m_references;                                    // +0
    void (__fastcall* m_pfnDestroy)(void*);               // +4 per-type deleter, object in ecx
    void* m_pObject;                                      // +8
};
template <class T> struct CCountedPointer {
    T* m_pObject;                                         // +0
    SCountBlock* m_pCount;                                // +4
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* object = other.m_pObject;
        SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) {
                --m_pCount->m_references;
                if (m_pCount->m_references == 0) {
                    m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                    operator delete(m_pCount);
                }
            }
            m_pObject = object;
            m_pCount = count;
            if (count)
                ++count->m_references;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        SCountBlock* count = m_pCount;
        if (count && --count->m_references == 0) {
            count->m_pfnDestroy(count->m_pObject);
            operator delete(count);
        }
    }
};
struct CActiveEntityScriptBase;
template<class K, class V> struct pair {
    K first;
    V second;
    pair& operator=(const pair& other);                   // 0x00CBA520 (the TU's out-of-line copy)
    __forceinline void Assign(const pair& other) { first = other.first; second = other.second; }   // the expanded copy
};
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        (_First + _Hole)->Assign(*(_First + _Idx));           // the expanded pair assignment
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > Elem;
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}
