// std::remove_copy(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing>* result, const CIntelligentPointer<const CThing>& val) -- copy every intelligent thing pointer that does not point at val's thing to result; returns the new end
// Flags: /Gr
// Leaf: remove_copy
// Retail: Fable.exe @ 0x00737AD0, 81 bytes (/O2). Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50,
// SetPItem 0x00A01B90. Callers: STLport `remove` inlined into CTCSolidToThing::FrameUpdate 0x00770E80 and
// NThingShot::CTCShotDefinitionBase::Update 0x007C21A0 (both landed: `erase(remove(v.begin(), v.end(),
// CIntelligentPointer<const CThing>(0)), v.end())` -- find @ 0x007379D0, then this remove_copy from the
// element after the hit), and 0x007341E3.
// Shape: STLport `for (; first != last; ++first) if (!(*first == val)) { *result = *first; ++result; }
// return result;` over 8-byte CIntelligentPointer elements (vptr +0, item +4): operator== compares the two
// GetPItem() results (the element's first), operator= is `SetPItem(other.GetPItem())`. /Gr: first=ecx,
// last=edx, result and the value reference on the stack (`ret 8`).
// Label correction: the propagated `_Remove_if<pair<CIntelligentPointer<CThing>,long>*, CIsFirstElementEqualTo>`
// is the 12-byte remove_copy_if twin (0x0080DF90); this body has an 8-byte stride and a value reference.
namespace N00737ad0 {
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    const CThing* m_pItem;                                // +0x04
    const CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(const CThing* item);                    // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
    bool operator==(const CIntelligentPointer& other) const { return GetPItem() == other.GetPItem(); }
};
template <class _InputIter, class _OutputIter, class _Tp>
_OutputIter __fastcall remove_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        if (!(*__first == __val)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
typedef CIntelligentPointer<const CThing> CPtr;
template CPtr* __fastcall remove_copy<CPtr*, CPtr*, CPtr>(CPtr*, CPtr*, CPtr*, const CPtr&);
}