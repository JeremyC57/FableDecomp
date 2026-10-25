// std::__pop_heap(first, last, result, pair<long,CDefPointer<const CContextSensitiveItemDef>> val, CContextSensitivePriorityCompare comp, int*) -- STLport heap pop step of the context-sensitive item priority sort: move the top to *result, then sift val down from the root
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005946B2, 63 bytes (a /Os module: unaligned entry), /Gr `__fastcall` (first in
//   ecx -> edi, last in edx -> esi; result, the 8-byte val, the comparator word and the distance pointer on the stack,
//   `ret 14h`).
// Identity: STLport 4.6 `__pop_heap(__first, __last, __result, __val, __comp, _Distance*)`; the __adjust_heap it calls
//   (0x00593E3E) is catalogued `_Adjust_heap<pair<long, CDefPointer<const CContextSensitiveItemDef>>*, int, ...,
//   CContextSensitivePriorityCompare>`, and the element helpers are the swap 0x005938F1's (copy 0x00592DFB, assignment
//   0x0059308A, destructor 0x005930F3). Callers: the sort_heap / pop_heap loops 0x0059476A and 0x00594CB2. The catalog's
//   "swap<CTCBase::CRegisteredTCInfo>" is propagated.
// Callees: pair::operator= 0x0059308A (`*result = *first`), pair copy ctor 0x00592DFB (val copied into the argument
//   area), __adjust_heap 0x00593E3E (`__fastcall` first, hole 0 in edx), ~pair 0x005930F3 (the by-value val).
// Shape: the STLport body in a size TU: the pair helpers stay out of line; the by-value val argument is copy-built in
//   the argument area (`push ecx ; push ecx ; mov ecx,esp`), the empty comparator pushed from its own slot.
#pragma optimize("s", on)
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                        // slot 1 (+4)
    unsigned long IVRefCount;                           // +0x04
};

template <class T>
class CDefPointer {
public:
    T* Object;                                          // +0x00
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++((CDefPointeeBase*)Object)->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) { if (--((CDefPointeeBase*)Object)->IVRefCount == 0) ((CDefPointeeBase*)Object)->Delete(); Object = 0; } }
    CDefPointer& operator=(const CDefPointer& p)
    {
        if (Object != p.Object) {
            if (Object) { if (--((CDefPointeeBase*)Object)->IVRefCount == 0) ((CDefPointeeBase*)Object)->Delete(); Object = 0; }
            Object = p.Object;
            if (Object) ++((CDefPointeeBase*)Object)->IVRefCount;
        }
        return *this;
    }
};

class CContextSensitiveItemDef;

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

typedef std::pair<long, CDefPointer<const CContextSensitiveItemDef> > TContextItemPair;

class CContextSensitivePriorityCompare {                // empty, passed by value (one stack word)
public:
    bool operator()(const TContextItemPair& a, const TContextItemPair& b) const;
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
                              _Distance __len, _Tp __val, _Compare __comp);   // 0x00593E3E

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp,
                                  _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap<TContextItemPair*, TContextItemPair, CContextSensitivePriorityCompare, int>(
    TContextItemPair*, TContextItemPair*, TContextItemPair*, TContextItemPair, CContextSensitivePriorityCompare, int*);
}