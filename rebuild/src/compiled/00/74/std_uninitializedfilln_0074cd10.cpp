// std::__uninitialized_fill_n(CIntelligentPointer<CThingCreatureBase>* first, unsigned int n, const CIntelligentPointer<CThingCreatureBase>& x, const __false_type&) -- copy-construct n intelligent pointers to x's item into raw storage; returns the end
// Flags: /Gr
// Leaf: ??$__uninitialized_fill_n@PAV?$CIntelligentPointer@VCThingCreatureBase@@@@IV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074CD10, 62 bytes, __fastcall (first ecx, n edx, &x and the tag on the stack), `ret 8`.
// Identity (proven): each placement copy stores vtable 0x0123E7D8, RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@ (CIntelligentPointer<CThingCreatureBase>);
//   callers the K108 _M_insert_overflow 0x0074D600 and CThingSearchTools::GetPFirstThing 0x00799C40.
// Callees: CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (on x), SetPItem 0x00A01B90.
// Shape (family K008, representative 0x004AA190): STLport 4.6 `__uninitialized_fill_n(..., const __false_type&)` loop with the
//   null-tested _Construct and the __forceinline intelligent-pointer copy constructor; the class's user-declared virtual
//   destructor (the landed 0x00739550 model) gives retail's register assignment (x in edi, n in ebx); STLport's three-parameter template, as the binding spells it.
inline void* operator new(unsigned int, void* p) { return p; }
class CBaseObject;
class CThingCreatureBase;
class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { CBaseObject* item = other.GetPItem(); SetPItem(item); }
    virtual ~CIntelligentPointer() { }                      // the class's own vtable (as the landed 0x00739550 row)
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CIntelligentPointer<CThingCreatureBase>* __fastcall __uninitialized_fill_n<CIntelligentPointer<CThingCreatureBase>*, unsigned int, CIntelligentPointer<CThingCreatureBase> >(
    CIntelligentPointer<CThingCreatureBase>*, unsigned int, const CIntelligentPointer<CThingCreatureBase>&, const __false_type&);
}