// std::__uninitialized_fill_n(CIntelligentPointer<const CThing>* first, unsigned int n, const CIntelligentPointer<const CThing>& x, const __false_type&) -- copy-construct n intelligent pointers to x's item into raw storage; returns the end
// Flags: /Gr
// Leaf: ??$__uninitialized_fill_n@V?$CIntelligentPointer@$$CBVCThing@@@@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x007377A0, 62 bytes, __fastcall (first ecx, n edx, &x and the tag on the stack), `ret 8`.
// Identity (proven): each placement copy stores vtable 0x01237F1C, RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@ (CIntelligentPointer<const CThing>);
//   callers the landed vector<CIntelligentPointer<const CThing> >::_M_insert_overflow 0x00739550.
// Callees: CBaseIntelligentPointer() 0x00A01B10, CBaseIntelligentPointer::GetPItem 0x00A01B50 (on x), SetPItem 0x00A01B90.
// Shape (family K008, representative 0x004AA190): STLport 4.6 `__uninitialized_fill_n(..., const __false_type&)` loop with the
//   null-tested _Construct and the __forceinline intelligent-pointer copy constructor; the class's user-declared virtual
//   destructor (the landed 0x00739550 model) gives retail's register assignment (x in edi, n in ebx); the one-parameter template the landed 0x00739550 row (and the binding) spells.
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C for const CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { CBaseObject* item = other.GetPItem(); SetPItem(item); }
    virtual ~CIntelligentPointer() { }                      // the class's own vtable (as the landed 0x00739550 row)
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class T> T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
template CIntelligentPointer<const CThing>* __fastcall __uninitialized_fill_n<CIntelligentPointer<const CThing> >(
    CIntelligentPointer<const CThing>*, unsigned int, const CIntelligentPointer<const CThing>&, const __false_type&);
}