// Grade: DIFFER(62v62) -- register tie: retail keeps x in edi and n in ebx, ours swaps them (x in ebx, n in edi); same instructions otherwise
// _STL::__uninitialized_fill_n(CIntelligentPointer<CThingAICreature>* first, unsigned int n, const CIntelligentPointer<CThingAICreature>& x, const __false_type&) -- STLport fill-construct n AI-creature intelligent pointers from x
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AF940, 62 bytes (+2 int3), `ret 8` (/Gr: first in ecx, n in edx, x and the
//   __false_type tag reference on the stack).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::GetPItem 0x00A01B50, ::SetPItem 0x00A01B90;
//   CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0.
// Identity: the catalog's `_Fill_n<unsigned long*,unsigned int,unsigned long>` is a propagated Dinkumware name; the
//   8-byte stride, the placement null test, the unsigned count (`jbe`) and `ret 8` are STLport 4.x
//   `__uninitialized_fill_n` (the landed 0x006A3180 shape) over the vector<CIntelligentPointer<CThingAICreature> > of
//   this TU (callers 0x007AF9BA / 0x007AFDB0 / 0x007AFED4 store vtable 0x0123E7E0 in their inline copies).
// Element: CIntelligentPointer<T> copy constructor = base ctor, vtable store, other.GetPItem(), SetPItem.
// Shape: /Gr TU; the loop head is 16-byte aligned (the `nop`).
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
class CThingAICreature;
class CBaseObject;
class CBaseIntelligentPointer {                         // 8 bytes: vtable + item
public:
    void* PData;                                        // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7E0 for CIntelligentPointer<CThingAICreature>
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
namespace _STL {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new ((void*)&*__cur) _Tp(__x);
    return __cur;
}
template CIntelligentPointer<CThingAICreature>* __fastcall __uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*, unsigned int, CIntelligentPointer<CThingAICreature> >(
    CIntelligentPointer<CThingAICreature>*, unsigned int, const CIntelligentPointer<CThingAICreature>&, const __false_type&);
}
