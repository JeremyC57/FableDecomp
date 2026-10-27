// std::__uninitialized_copy(CDefPointer<const CHeroAbilityDef>* first, CDefPointer<const CHeroAbilityDef>* last, CDefPointer<const CHeroAbilityDef>* result, const __false_type&) -- copy-construct add-ref'd def pointers into raw storage, return the end
// Leaf: ??$__uninitialized_copy@PAV?$CDefPointer@$$CBVCHeroAbilityDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCHeroAbilityDef@@@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F79F7, 38 bytes, `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (proven): its only caller is the landed vector<CDefPointer<const CHeroAbilityDef> >::_M_insert_overflow 0x005FA10A, which copies its own mutable [start, position) / [position, finish) ranges (the const sibling for operator= is 0x005F7B30).
// Shape (family M151, representative 0x005B1479): STLport `for (; first != last; ++first, ++cur) _Construct(&*cur, *first);`
//   -- the placement-new null test, then CDefPointer's copy (store, null test, `inc [obj+4]`); the element model is the landed
//   uninitialized_fill_n<CDefPointer<const CThingBaseDef>*> 0x007688B0's. Size TU (no loop alignment).
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* p) { return p; }
class CDefPointeeBase {                                     // counted def object: vptr, reference count at +4
public:
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_refCount;                                        // +0x04
};
class CHeroAbilityDef;
template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                                // +0x00 (Ego_r.pdb)
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}
template CDefPointer<const CHeroAbilityDef>* __fastcall std::__uninitialized_copy(CDefPointer<const CHeroAbilityDef>*, CDefPointer<const CHeroAbilityDef>*, CDefPointer<const CHeroAbilityDef>*, const std::__false_type&);