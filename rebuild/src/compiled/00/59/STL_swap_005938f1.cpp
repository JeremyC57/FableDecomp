// std::swap(pair<long,CDefPointer<const CContextSensitiveItemDef>>& a, pair<long,CDefPointer<const CContextSensitiveItemDef>>& b) -- exchange two prioritised context-sensitive item entries through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005938F1, 51 bytes (a /Os module: unaligned entry, ebp frame, `leave`), /Gr
//   `__fastcall` (a in ecx -> edi, b in edx -> esi), plain `ret`.
// Identity: the 8-byte {long, CDefPointer} element of the context-sensitive item priority sort (the TU's __adjust_heap
//   0x00593E3E is catalogued `_Adjust_heap<pair<long, CDefPointer<const CContextSensitiveItemDef>>, ...,
//   CContextSensitivePriorityCompare>`; the __pop_heap 0x005946B2 shares this element's helpers). The element copy
//   0x00592DFB copies the long and the def pointer (add-ref of the def's count at +4), the assignment 0x0059308A and the
//   destructor 0x005930F3 release through the def's slot-1 Delete. Callers: the out-of-line iter_swap 0x00593E0B (a bare
//   `jmp`) and the partition step 0x00594672. The catalog's "swap<CTCBase::CRegisteredTCInfo>" is propagated.
// Callees: pair copy ctor 0x00592DFB, pair::operator= 0x0059308A, ~pair 0x005930F3 (all out of line in this module).
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` in a size TU, where the pair's copy constructor,
//   assignment and destructor stay calls. ~CDefPointer is `__forceinline` (its release sits inside the out-of-line ~pair
//   0x005930F3): with a plain inline def-pointer destructor VC7.1 inlines ~pair here and calls ~CDefPointer on +4.
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

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

typedef pair<long, CDefPointer<const CContextSensitiveItemDef> > TContextItemPair;
template void __fastcall swap<TContextItemPair>(TContextItemPair&, TContextItemPair&);
}