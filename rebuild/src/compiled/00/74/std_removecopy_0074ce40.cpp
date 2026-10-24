// std::remove_copy(CIntelligentPointer<CThingCreatureBase>* first, CIntelligentPointer<CThingCreatureBase>* last, CIntelligentPointer<CThingCreatureBase>* result, const CIntelligentPointer<CThingCreatureBase>& val) -- copy every intelligent thing pointer that does not point at val's thing to result; returns the new end
// Flags: /Gr
// Leaf: remove_copy
// Retail: Fable.exe @ 0x0074CE40, 81 bytes (/O2), byte twin of the landed-model 0x00737AD0. Callees:
// CBaseIntelligentPointer::GetPItem 0x00A01B50, SetPItem 0x00A01B90. Caller: CTCCreatureGenerator's
// RemoveTriggerer (0x0074A1E0): `CreatureTriggerers.erase(remove(begin, end, CIntelligentPointer<
// CThingCreatureBase>(thing)), end)` -- the temporary carries vtable 0x0123E7D8, find @ 0x0074CEE0, then this
// remove_copy from the element after the hit; Ego_r.pdb CreatureTriggerers is
// vector<CIntelligentPointer<CThingCreatureBase> >.
// Shape: STLport `for (; first != last; ++first) if (!(*first == val)) { *result = *first; ++result; }
// return result;` over 8-byte CIntelligentPointer elements (vptr +0, item +4): operator== compares the two
// GetPItem() results (the element's first), operator= is `SetPItem(other.GetPItem())`. /Gr: first=ecx,
// last=edx, result and the value reference on the stack (`ret 8`).
// Label correction: the propagated `_Remove_if<pair<CIntelligentPointer<CThing>,long>*, CIsFirstElementEqualTo>`
// is the 12-byte remove_copy_if twin (0x0080DF90); this body has an 8-byte stride and a value reference.
namespace N0074ce40 {
class CThing;
class CThingCreatureBase;
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
typedef CIntelligentPointer<CThingCreatureBase> CPtr;
template CPtr* __fastcall remove_copy<CPtr*, CPtr*, CPtr>(CPtr*, CPtr*, CPtr*, const CPtr&);
}