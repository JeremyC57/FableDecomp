// std::__uninitialized_fill_n(CIntelligentPointer<CThingCreatureBase >* first, unsigned int n, const CIntelligentPointer<CThingCreatureBase >& x, const __false_type&) -- copy-construct n copies of x into raw storage at first, return the end of the constructed range
// Grade: DIFFER(62v62) -- register choice only: retail keeps n in ebx and x in edi (x loaded first, 'mov edi,[esp+10h] ; mov ebx,edx'); VC7.1 here assigns x to ebx and n to edi, every other byte equal
// Leaf: ??$__uninitialized_fill_n@PAV?$CIntelligentPointer@VCThingCreatureBase@@@@IV1@@std@@YIPAV?$CIntelligentPointer@VCThingCreatureBase@@@@PAV1@IABV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074CD10, 62 bytes (+ int3 padding), `__fastcall` `ret 8`.
// Identity (proven): STLport 4.6 `__uninitialized_fill_n<_ForwardIter, _Size, _Tp>(first, n, x, const __false_type&)` for
//   _ForwardIter = CIntelligentPointer<CThingCreatureBase >*, _Size = unsigned int (`test edx,edx ; jbe`), _Tp = CIntelligentPointer<CThingCreatureBase >; evidence: the copy-constructed elements store vtable 0x0123E7D8, RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@ (CIntelligentPointer<CThingCreatureBase>); callers 0x0074D600, 0x00799CB0, 0x00799CD0 (the vector<CIntelligentPointer<CThingCreatureBase> > growth / fill paths).
// Label: the manifest's "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is propagated across byte-identical bodies (family J073: five fills that differ only in
//   the stored vtable; retail has no identical-code folding, so each address is one instantiation).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (agent952 H001 spellings).
// Shape: /Gr template (first in ecx, n in edx, x and the tag reference on the stack). Placement new null-tests each destination;
//   CIntelligentPointer's user copy constructor is inline: the base ctor, the vtable store, then SetPItem(x.GetPItem()). The
//   placement new sits directly in the loop (as in the landed fill 0x006A3180 over CThingGraphicArgPasser, same register
//   assignment, and its __forceinline copy constructor): with a plain inline copy constructor, directly or through
//   _Construct, VC7.1 swaps the n / x registers (ebx / edi).
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new ((void*)&*__cur) _Tp(__x);
    return __cur;
}

template CIntelligentPointer<CThingCreatureBase >* __fastcall __uninitialized_fill_n<CIntelligentPointer<CThingCreatureBase >*, unsigned int, CIntelligentPointer<CThingCreatureBase > >(
    CIntelligentPointer<CThingCreatureBase >*, unsigned int, const CIntelligentPointer<CThingCreatureBase >&, const __false_type&);

}
