// NUISystem::CNavButton::ChangeState(unsigned long state) -- run the base state change; states 1 and 5: register with the front-end manager as an observer and, unless this is its screen's accept button, observe event 0x19; states 0 and 6: unregister
// Leaf: ?ChangeState@CNavButton@NUISystem@@UAEXK@Z
// Layout: engine/CObserver.h and engine/CManager.h are generated flat records (pack(1), vptr as a data member, no virtuals),
//   so NUISystem::CObserver / CManager are modelled locally with their virtual slots (CObserver 0x14 bytes as the header).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00558C70, 151 bytes = 0x82 of code + the 3-entry jump table 0x00558CF4 + the 7-byte index
//   table 0x00558D00 {0,1,2,2,2,1,0}; `ret 4`. (The "SPLIT" note on the row is the function's own switch tables; the next
//   function starts after the 9 filler bytes at 0x00558D10.)
// Identity: slot 48 of NUISystem::CNavButton's vtable 0x0124B04C (rtti-vtables.tsv; sole reference 0x0124B10C), the slot the
//   UI family uses for ChangeState (landed CSlider 0x0054A990, CSpellContainerList 0x00544350); first call is the qualified base
//   ?ChangeState@CChangingStateComponent@NUISystem@@UAEXK@Z 0x0052CF40 with the same argument. Ego_r.pdb / FableWin.pdb
//   ?ChangeState@CNavButton@NUISystem@@UAEXK@Z (donor order ctor, copy ctor, dtor, IsAcceptButton, ChangeState).
// Callees: ?GetInstance@CManager@NUISystem@@SIPAV12@XZ 0x0041E5F2 (static __fastcall), manager slot 2 AddObserver / slot 5
//   RemoveObserver (names as the landed CSlider::ChangeState), ?IsAcceptButton@CNavButton@NUISystem@@QBE_NXZ 0x00558C10 (the PDB
//   spelling is const; the landed row spells it QAE), CObserver slot 3 ObserveEvent(EEvent) (Ego_r ?ObserveEvent@CObserver@
//   NUISystem@@UAEXW4EEvent@2@@Z) as a tail jump through the CObserver subobject (+4).
// Bases (Fable.exe RTTI): CNavButton : CClickable : CHoverable : CChangingStateComponent : CComponent, with CUIInterface @0,
//   CObserver @4, CActionParam @0x18; `(CObserver*)this` is the null-tested +4 conversion (retail's `test esi,esi ; lea edi,[esi+4]`).
// Jump table verified by hand: index 0 (states 0, 6) -> 0x00558CC7 RemoveObserver arm, index 1 (states 1, 5) -> 0x00558C91
//   AddObserver arm, index 2 (2, 3, 4) and default -> 0x00558CEF return; the object's $L labels sit at the same offsets.
// Enumerators: EEvent 0x19 and the state numbers have no proven names (numbered literals, as the landed CSlider).
namespace NUISystem {
enum EEvent { };                                // NUISystem::EEvent (Ego_r ObserveEvent signature); enumerator names unproven
class CUIInterface { public:
    virtual void u00(); virtual void u01(); virtual void u02(); virtual void u03(); virtual void u04(); virtual void u05();
    virtual void u06(); virtual void u07(); virtual void u08(); virtual void u09(); virtual void u10(); virtual void u11();
    virtual void u12(); virtual void u13(); virtual void u14(); virtual void u15(); virtual void u16(); virtual void u17();
    virtual void u18(); virtual void u19(); virtual void u20(); virtual void u21(); virtual void u22(); virtual void u23();
    virtual void u24(); virtual void u25(); virtual void u26(); virtual void u27(); virtual void u28(); virtual void u29();
    virtual void u30(); virtual void u31(); virtual void u32(); virtual void u33(); virtual void u34(); virtual void u35();
    virtual void u36(); virtual void u37(); virtual void u38(); virtual void u39(); virtual void u40(); virtual void u41();
    virtual void u42(); virtual void u43(); virtual void u44(); virtual void u45(); virtual void u46(); virtual void u47();
    virtual void ChangeState(unsigned long state);   // slot 48
};
class CObserver { public:
    virtual void o00();
    virtual void o01();
    virtual bool IsObservingEvent(EEvent eventType);   // slot 2 (+0x08)
    virtual void ObserveEvent(EEvent eventType);       // slot 3 (+0x0c)
    unsigned char EventsToObserve[0xc];         // +0x04 set<EEvent>
    bool PreventObservation;                    // +0x10
};
class CActionParam { public:
    virtual void p00();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
};
class CChangingStateComponent : public CComponent { public:
    virtual void ChangeState(unsigned long state);   // 0x0052CF40
};
class CHoverable : public CChangingStateComponent { public:
};
class CClickable : public CHoverable { public:
};
class CManager { public:
    virtual void v000(); virtual void v001();
    virtual void AddObserver(CObserver* observer);       // slot 2 (+0x08)
    virtual void v003(); virtual void v004();
    virtual void RemoveObserver(CObserver* observer);    // slot 5 (+0x14)
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
};
class CNavButton : public CClickable { public:
    bool IsAcceptButton() const;                         // 0x00558C10
    virtual void ChangeState(unsigned long state);
};
void CNavButton::ChangeState(unsigned long state)
{
    CChangingStateComponent::ChangeState(state);
    switch (state) {
    case 1:
    case 5:
        CManager::GetInstance()->AddObserver(this);
        if (!IsAcceptButton())
            ObserveEvent((EEvent)0x19);
        break;
    case 0:
    case 6:
        CManager::GetInstance()->RemoveObserver(this);
        break;
    }
}
}