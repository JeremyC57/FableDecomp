// std::remove_copy_if(std::pair<CIntelligentPointer<CThing>,long>* first, std::pair<CIntelligentPointer<CThing>,long>* last, std::pair<CIntelligentPointer<CThing>,long>* result, CIsFirstElementEqualTo pred) -- copy every (thing, value) pair whose thing is not pred's target to result; returns the new end
// Flags: /Gr
// Leaf: remove_copy_if
// Retail: Fable.exe @ 0x0080DF90, 96 bytes incl. int3 padding (/O2). Callees: CBaseIntelligentPointer::
// GetPItem 0x00A01B50, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10. Caller 0x0080E2F0: STLport
// `remove_if` inlined (find_if @ 0x0080E060, then this remove_copy_if from the element after the hit), the
// predicate built by value in the argument area (CBaseIntelligentPointer ctor, vtable 0x01238C6C =
// CIntelligentPointer<CThing>, Set/GetPItem).
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return
// result;` over 12-byte pair<CIntelligentPointer<CThing>, long> (pointer vptr +0, item +4, long +8): the
// pair's implicit operator= is `SetPItem(GetPItem())` plus the long; the anonymous-namespace predicate
// CIsFirstElementEqualTo holds the target pointer by value and compares GetPItem() of the element's first
// with its own (element evaluated first). The callee destroys its by-value predicate on the single exit
// (the pointer's compiler-generated destructor just calls the base one). /Gr: first=ecx, last=edx,
// result + the 8-byte predicate on the stack (`ret 0Ch`).
namespace N0080df90 {
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                      // +0x04
    CBaseIntelligentPointer();                            // 0x00A01B10
    ~CBaseIntelligentPointer();                           // 0x00A01C10
    CThing* GetPItem() const;                             // 0x00A01B50
    void SetPItem(CBaseObject* item);                     // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};
template<class K, class V> struct pair { K first; V second; };
typedef pair<CIntelligentPointer<CThing>, long> Elem;
struct CIsFirstElementEqualTo {                           // _anon_41ACE303::CIsFirstElementEqualTo
    CIntelligentPointer<CThing> m_Target;                 // +0x00
    bool operator()(const Elem& e) const { return e.first.GetPItem() == m_Target.GetPItem(); }
};
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
template Elem* __fastcall remove_copy_if<Elem*, Elem*, CIsFirstElementEqualTo>(Elem*, Elem*, Elem*, CIsFirstElementEqualTo);
}