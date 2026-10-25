// _STL::__unguarded_insertion_sort_aux(pair<long,CIntelligentPointer<CThing>>* first, pair<long,CIntelligentPointer<CThing>>* last, pair<long,CIntelligentPointer<CThing>>*, CKeyPairCompareLess<long,CIntelligentPointer<CThing>> comp) -- STLport final unguarded insertion pass over the long-keyed thing map: insert each entry into the sorted run before it
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A32B0, 79 bytes (+ int3), /Gr `__fastcall` (first in ecx -> esi, last in edx
//   -> ebx; the value-type tag pointer and the comparator word on the stack, `ret 8`).
// Identity: STLport 4.6 `__unguarded_insertion_sort_aux` of the CVectorMap<long, CIntelligentPointer<CThing> > sort:
//   the landed __final_insertion_sort front end 0x006A3D30 calls it as `(first + 16, last, (T*)0, comp)` and the callee
//   is the landed __unguarded_linear_insert 0x006A2DC0 over the 12-byte pair<long, CIntelligentPointer<CThing> >
//   (vtable 0x01238C6C). Callers: 0x006A3497, 0x006A3D65. The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::_Destroy" is propagated.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (the `_Tp(*__i)` copy built
//   in the argument area), __unguarded_linear_insert 0x006A2DC0 (out of line; it owns and destroys its by-value val).
// Shape: `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);` -- the pair's
//   user copy constructor builds the by-value argument in its stack slot (`sub esp,0Ch ; mov eax,esp`) after the
//   comparator word is pushed from ebp; the comparator stays declaration-only.
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {                         // 8 bytes: vptr, item
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x006A2DC0

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template <class K, class V> class CKeyPairCompareLess {   // empty, passed by value (one stack word)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const;
};

typedef _STL::pair<long, CIntelligentPointer<CThing> > TThingEntry;
typedef CKeyPairCompareLess<long, CIntelligentPointer<CThing> > TThingEntryLess;
template void __fastcall _STL::__unguarded_insertion_sort_aux<TThingEntry*, TThingEntry, TThingEntryLess>(
    TThingEntry*, TThingEntry*, TThingEntry*, TThingEntryLess);