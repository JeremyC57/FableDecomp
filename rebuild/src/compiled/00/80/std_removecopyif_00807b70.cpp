// std::remove_copy_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint* result, CTargetPointIsNotEnemyOfThing pred) -- copy the target points whose thing is null, flagged dead or an enemy of pred.Thing to result; returns the new end
// Flags: /Gr
// Leaf: remove_copy_if
// Retail: Fable.exe @ 0x00807B70, 167 bytes (/O2). Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50,
// SetPItem 0x00A01B90, CTCEnemy::AreThingsEnemies 0x0076D2F0 (`__fastcall`, landed label).
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return
// result;` over the 28-byte NPlayerTargeting::CTargetPoint (Ego_r.pdb: PThing CIntelligentPointer<CThing> +0,
// Index +8, TargetingPosition C3DVector +0xC, FrameRejected +0x18). The anonymous-namespace predicate holds
// `CThing* Thing` and is true for a live (retail CThing flag byte +0x91 bit 0 clear) non-null thing that is
// not an enemy of Thing; GetPItem() is called once per use. CTargetPoint's implicit operator= is
// SetPItem(GetPItem()), Index, C3DVector's implicit operator= (engine C3DVector has a user copy ctor and no
// operator=: the pointer-register block copy), then FrameRejected -- induction pointers at +0xC for both sides. /Gr: first=ecx, last=edx (spilled to the
// `push ecx` slot), result + the 4-byte predicate on the stack (`ret 8`).
// Label correction: catalog `_Remove_if<CTargetPoint*,CTargetPointIsNotEnemyOfThing>` is the remove_copy_if
// half (the caller inlines find_if and calls this from the element after the hit).
namespace N00807b70 {
struct CThing {                                           // retail flag byte only
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                               // +0x91 bit 0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                      // +0x04
    CThing* GetPItem() const;                             // 0x00A01B50
    void SetPItem(CThing* item);                          // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor, implicit operator=
};
struct CTCEnemy {
    static bool AreThingsEnemies(CThing* a, CThing* b);   // 0x0076D2F0
};
namespace NPlayerTargeting {
struct CTargetPoint {                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;
struct CTargetPointIsNotEnemyOfThing {                    // _anon_58ED6966::CTargetPointIsNotEnemyOfThing
    CThing* Thing;                                        // +0x0
    bool operator()(const CTargetPoint& tp) const
    {
        return tp.PThing.GetPItem() != 0 && !tp.PThing.GetPItem()->Dead
            && !CTCEnemy::AreThingsEnemies(Thing, tp.PThing.GetPItem());
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
template CTargetPoint* __fastcall remove_copy_if<CTargetPoint*, CTargetPoint*, CTargetPointIsNotEnemyOfThing>(CTargetPoint*, CTargetPoint*, CTargetPoint*, CTargetPointIsNotEnemyOfThing);
}