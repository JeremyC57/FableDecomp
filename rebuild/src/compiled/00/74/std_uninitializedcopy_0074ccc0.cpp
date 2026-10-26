// std::__uninitialized_copy(CIntelligentPointer<CThingCreatureBase >* first, CIntelligentPointer<CThingCreatureBase >* last, CIntelligentPointer<CThingCreatureBase >* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end of the constructed range
// Leaf: ??$__uninitialized_copy@PAV?$CIntelligentPointer@VCThingCreatureBase@@@@PAV1@@std@@YIPAV?$CIntelligentPointer@VCThingCreatureBase@@@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074CCC0, 66 bytes (+int3 pad to 80), `__fastcall` `ret 8`.
// Identity (proven): STLport 4.6 `__uninitialized_copy<_InputIter, _ForwardIter>(first, last, result, const __false_type&)` for
//   _InputIter = CIntelligentPointer<CThingCreatureBase >*, _ForwardIter = CIntelligentPointer<CThingCreatureBase >*. Element: the vtable the body stores, 0x0123E7D8, is RTTI
//   .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@. Source constness: its only caller 0x0074D600 is vector<CIntelligentPointer<CThingCreatureBase> >::_M_insert_overflow, which copies _M_start..pos (`mov ecx,[edi]`) through it: a non-const pointer.
//   Family H001 (7 byte-identical bodies; retail has no identical-code folding, so each address is exactly one instantiation).
//   The manifest label "_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int> > ...>" is a propagated Dinkumware spelling.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Shape: /Gr template (first in ecx, last in edx, result and the tag reference on the stack). Placement new null-tests each
//   destination; CIntelligentPointer's user copy constructor is inline: the base ctor, the vtable store, then
//   SetPItem(other.GetPItem()).
class CBaseObject;
class CThingCreatureBase;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    void* PData;                                        // +4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CIntelligentPointer<CThingCreatureBase >* __fastcall __uninitialized_copy<CIntelligentPointer<CThingCreatureBase >*, CIntelligentPointer<CThingCreatureBase >*>(
    CIntelligentPointer<CThingCreatureBase >*, CIntelligentPointer<CThingCreatureBase >*, CIntelligentPointer<CThingCreatureBase >*, const __false_type&);

}