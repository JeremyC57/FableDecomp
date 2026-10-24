// std::remove_copy_if(std::pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>* first, std::pair<...>* last, std::pair<...>* result, CDeadDangerThing pred) -- copy the stealth danger entries whose thing is alive and whose status is set to result; returns the new end
// Flags: /Gr
// Leaf: remove_copy_if
// Retail: Fable.exe @ 0x00748D80, 129 bytes (/O2). Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50,
// SetPItem 0x00A01B90. Caller: CTCStealth::UpdateDangerInfo (0x00747C70, STLport `remove_if` inlined:
// find_if, then this remove_copy_if from the element after the hit).
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return
// result;` over 16-byte pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> (pointer vptr +0,
// item +4; Ego_r.pdb CDangerStatus: EDangerStatus DangerStatus +8, long Timer +0xC). The anonymous-
// namespace predicate CDeadDangerThing removes an entry whose thing is gone, is dead (retail CThing flag
// byte +0x91 bit 0, as in the landed CThingDeadCreature/CThingSwitch models) or whose status is zero --
// GetPItem() is called once per test. The pair's implicit operator= is `SetPItem(GetPItem())` then the
// status words through a second induction pointer pair (+8). /Gr: first=ecx, last=edx (spilled to the
// `push ecx` slot), result + the empty predicate on the stack (`ret 8`).
namespace N00748d80 {
struct CThing {                                           // retail flag byte only
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                               // +0x91 bit 0
};
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
};
namespace NStealth {
enum EDangerStatus { DANGER_STATUS_NONE };
struct CDangerStatus {
    EDangerStatus DangerStatus;                           // +0x0
    long Timer;                                           // +0x4
};
}
template<class K, class V> struct pair { K first; V second; };
typedef pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> Elem;
struct CDeadDangerThing {                                 // _anon_AE0DD725::CDeadDangerThing
    bool operator()(const Elem& e) const
    {
        return e.first.GetPItem() == 0 || e.first.GetPItem()->Dead || e.second.DangerStatus == NStealth::DANGER_STATUS_NONE;
    }
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
template Elem* __fastcall remove_copy_if<Elem*, Elem*, CDeadDangerThing>(Elem*, Elem*, Elem*, CDeadDangerThing);
}