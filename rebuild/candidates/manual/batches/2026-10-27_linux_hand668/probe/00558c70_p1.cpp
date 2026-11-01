// NUISystem::CNavButton::ChangeState(unsigned long state) -- base state change, then (de)register as a front-end observer
// Leaf: ?ChangeState@CNavButton@NUISystem@@UAEXK@Z
namespace NUISystem {
enum EEvent { EEVENT_19 = 0x19 };
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
            ObserveEvent(EEVENT_19);
        break;
    case 0:
    case 6:
        CManager::GetInstance()->RemoveObserver(this);
        break;
    }
}
}
