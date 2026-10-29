// CTCActivationReceptorBase::FrameUpdate() -- with triggers: an activated receptor deactivates once its set time has elapsed; otherwise combine the triggers by the logic type (1 AND, 2 OR, 3 NOR, 4 NAND) and activate on true
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F7870, 192 bytes incl. the four-entry jump table 0x007F7920 (plain `ret`). Slot 9 of the
// CTCActivationReceptor* vtables (CTCActivationReceptorCreatureGenerator 0x012413EC and four more: the inherited base method).
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static), STLport __find_if over CThing** with unary_negate<CIsTriggerActivated>
// @ 0x007F7EF0 and with CIsTriggerActivated @ 0x007F7C10 (both `__fastcall`-shaped: first/last in ecx/edx, the empty predicate by
// value and the random_access_iterator_tag by const reference on the stack), this's virtual Activate (slot 28, +0x70) and
// Deactivate (slot 29, +0x74).
// Layout: retail (the landed constructor 0x007F7710): Activated +0x0c, Triggers +0x10 (12-byte vector), LogicType +0x1c,
// DeactivateAfterSetTime +0x20, FramesAfterActivationToDeactivate +0x24, FrameAtLastActivation +0x28. engine/
// CTCActivationReceptorBase.h carries the PDB's 16-byte vector (LogicType +0x20 ..), so a local model.
// Shape: `case 1: case 4:` / `case 2: case 3:` arms each end in `if (LogicType == 4/3) result = !result;` -- VC7.1 tail-merges the
// two `jne ; test ; sete` endings; the default leaves straight to the epilogue. The empty test is an inline `empty()` (a direct
// `first == last` keeps both in ecx/edx and the calls reuse them); each search is STLport's inline `find_if(first, last, pred)`
// wrapper (`__forceinline`: the second overload is otherwise emitted out of line) passing its by-value predicate and an inline
// iterator-category tag to the out-of-line __find_if, which gives retail's order (last/first loaded, tag pushed, then the
// predicate's uninitialised dword copied) and the shared tag/predicate frame slots. Empty classes with user constructors: a
// value-initialised `T()` of a ctor-less empty class adds a zero byte store.
class CThing;

class CWorld { public:
    static long __fastcall GetFrame();                         // 0x0049D870
};

class CIsTriggerActivated { public: CIsTriggerActivated() { } };
struct CNotTriggerActivated { CIsTriggerActivated m_pred; CNotTriggerActivated() { } };   // std::unary_negate<CIsTriggerActivated>
struct CRandomAccessIteratorTag { CRandomAccessIteratorTag() { } };                           // std::random_access_iterator_tag

CThing** __fastcall FindIfNotActivated(CThing** first, CThing** last, CNotTriggerActivated pred,
                                       const CRandomAccessIteratorTag& tag);   // 0x007F7EF0
CThing** __fastcall FindIfActivated(CThing** first, CThing** last, CIsTriggerActivated pred,
                                    const CRandomAccessIteratorTag& tag);      // 0x007F7C10

inline CRandomAccessIteratorTag IteratorCategory(CThing** const&) { return CRandomAccessIteratorTag(); }
__forceinline CThing** find_if(CThing** first, CThing** last, CNotTriggerActivated pred)
{
    return FindIfNotActivated(first, last, pred, IteratorCategory(first));
}
__forceinline CThing** find_if(CThing** first, CThing** last, CIsTriggerActivated pred)
{
    return FindIfActivated(first, last, pred, IteratorCategory(first));
}

struct CThingPtrVector {                                 // retail std::vector<CThing*> (12 bytes)
    CThing** m_first;
    CThing** m_last;
    CThing** m_end;
    bool empty() const { return m_first == m_last; }
};

class CTCActivationReceptorBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void Activate();                             // slot 28 (+0x70)
    virtual void Deactivate();                           // slot 29 (+0x74)
    CThing*         ParentThing;                         // +0x04
    bool            ToBeRemoved;                         // +0x08
    bool            IsInGlobalUpdateTCs;                 // +0x09
    unsigned char   _pad0a[2];
    bool            Activated;                           // +0x0c
    CThingPtrVector Triggers;                            // +0x10
    long            LogicType;                           // +0x1c ELogicType
    bool            DeactivateAfterSetTime;              // +0x20
    unsigned long   FramesAfterActivationToDeactivate;   // +0x24
    unsigned long   FrameAtLastActivation;               // +0x28
    void FrameUpdate();
};

void CTCActivationReceptorBase::FrameUpdate()
{
    if (Triggers.empty())
        return;
    if (Activated) {
        if (DeactivateAfterSetTime && CWorld::GetFrame() - FrameAtLastActivation == FramesAfterActivationToDeactivate)
            Deactivate();
        return;
    }
    bool activate = false;
    switch (LogicType) {
    case 1:
    case 4:
        if (find_if(Triggers.m_first, Triggers.m_last, CNotTriggerActivated()) == Triggers.m_last)
            activate = true;
        if (LogicType == 4)
            activate = !activate;
        break;
    case 2:
    case 3:
        if (find_if(Triggers.m_first, Triggers.m_last, CIsTriggerActivated()) != Triggers.m_last)
            activate = true;
        if (LogicType == 3)
            activate = !activate;
        break;
    }
    if (activate)
        Activate();
}