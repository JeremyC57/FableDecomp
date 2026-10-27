// std::__uninitialized_fill_n(CIntelligentPointer<CThing>* first, unsigned int n, const CIntelligentPointer<CThing>& x, const __false_type&) -- copy-construct n intelligent pointers to x's item into raw storage; returns the end
// Flags: /Gr
// Leaf: ??$__uninitialized_fill_n@PAV?$CIntelligentPointer@VCThing@@@@IV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AA190, 62 bytes, __fastcall (first ecx, n edx, &x and the tag on the stack), `ret 8`.
// Identity (proven): each placement copy stores vtable 0x01238C6C, RTTI .?AV?$CIntelligentPointer@VCThing@@@@ (CIntelligentPointer<CThing>);
//   callers the K108 vector<CIntelligentPointer<CThing> >::_M_insert_overflow 0x004AC940 and 0x0065574C.
// Callees: CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (on x), SetPItem 0x00A01B90.
// Shape (family K008, representative 0x004AA190): STLport 4.6 `__uninitialized_fill_n(..., const __false_type&)` loop with the
//   null-tested _Construct and the __forceinline intelligent-pointer copy constructor; the class's user-declared virtual
//   destructor (the landed 0x00739550 model) gives retail's register assignment (x in edi, n in ebx); STLport's three-parameter template, as the binding spells it.
inline void* operator new(unsigned int, void* p) { return p; }
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
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
template CIntelligentPointer<CThing>* __fastcall __uninitialized_fill_n<CIntelligentPointer<CThing>*, unsigned int, CIntelligentPointer<CThing> >(
    CIntelligentPointer<CThing>*, unsigned int, const CIntelligentPointer<CThing>&, const __false_type&);
}