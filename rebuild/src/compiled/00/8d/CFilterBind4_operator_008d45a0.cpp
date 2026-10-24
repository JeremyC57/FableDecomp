// CFilter_Bind4::operator()(CEventHitBy* const& event) const -- hit-event filter conjunction: hit by the scripted thing, with a live weapon of the wanted def, created in (After, BeforeOrOn]
// Leaf: ??$?RPAVCEventHitBy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D45A0, length 95 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb instantiates `CFilter_Bind4<CIsHitEventHitByThingWithScriptName, ?A0x70a0c557::
//   CEventFilter_HitEventWithWeapon, CIsEventCreatedBeforeOrOn, CIsEventCreatedAfter>::operator()<CEventHitBy*>(
//   CEventHitBy* const&) const` (with GetEvent<CEventHitBy, ...> of CTCEntityEvents); the second filter is inferred from
//   its data member (Ego_r.pdb CEventFilter_HitEventWithWeapon { long WeaponDefIndex }) and its neighbour 0x008D4550 (the
//   NoWeapon instance: weapon absent or flagged). No direct callers (an unreferenced COMDAT copy; the GetEvent bodies
//   inline it). The catalog label "HitEvent_ParticleMorphFilter_Predicate" is wrong.
// Callees: NEntityEvents::CIsHitEventHitByThingWithScriptName::operator()(const CEventHitBase*) const @ 0x00811100,
//   NEntityEvents::CEventHitBase::GetPWeaponUsed() const @ 0x00810FD0.
// Layout (Ego_r.pdb): CFilter_Bind4 { const F1* F1; const F2* F2; const F3* F3; const F4* F4 } (16 bytes);
//   CEventBase::WorldFrame +4; the weapon thing's 16-bit def index +0x8C and flag bit 0 of +0x91.
// Shape: `return F1(e) && F2(e) && F3(e) && F4(e);` (eax 1/0 exits) with the three filters after the first inlined: the
//   second filter's pointer is fetched into ebx before the GetPWeaponUsed call, the event's WorldFrame loaded once for
//   both frame tests (`jg` / `jle`).
class CThing {
public:
    unsigned char  _pad00[0x8C];
    unsigned short m_defIndex;                          // +0x8C
    unsigned char  _pad8E[3];
    unsigned char  m_flags91;                           // +0x91 (bit 0: the thing is flagged dead)
    bool IsFlaggedDead() const { return (m_flags91 & 1) != 0; }
};

class CCharString {
public:
    void* PStringData;
};

namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
    long WorldFrame;                                    // +0x04
};

class CEventHitBase : public CEventBase {
public:
    CThing* GetPWeaponUsed() const;                     // 0x00810FD0
};

class CEventHitBy : public CEventHitBase {
};

class CIsHitEventHitByThingWithScriptName {
public:
    CCharString ScriptName;                             // +0x00
    bool operator()(const CEventHitBase* event) const;  // 0x00811100
};

class CIsEventCreatedBeforeOrOn {
public:
    long WorldFrame;                                    // +0x00
    bool operator()(const CEventBase* event) const { return event->WorldFrame <= WorldFrame; }
};

class CIsEventCreatedAfter {
public:
    long WorldFrame;                                    // +0x00
    bool operator()(const CEventBase* event) const { return event->WorldFrame > WorldFrame; }
};
}

namespace {
class CEventFilter_HitEventWithWeapon {
public:
    long WeaponDefIndex;                                // +0x00
    bool operator()(const NEntityEvents::CEventHitBase* event) const
    {
        CThing* weapon = event->GetPWeaponUsed();
        if (weapon && !weapon->IsFlaggedDead() && weapon->m_defIndex == WeaponDefIndex)
            return true;
        return false;
    }
};
}

template <class T1, class T2, class T3, class T4>
class CFilter_Bind4 {
public:
    const T1* F1;                                       // +0x00
    const T2* F2;                                       // +0x04
    const T3* F3;                                       // +0x08
    const T4* F4;                                       // +0x0C
    template <class T>
    bool operator()(const T& t) const
    {
        return (*F1)(t) && (*F2)(t) && (*F3)(t) && (*F4)(t);
    }
};

template bool CFilter_Bind4<NEntityEvents::CIsHitEventHitByThingWithScriptName, CEventFilter_HitEventWithWeapon,
                            NEntityEvents::CIsEventCreatedBeforeOrOn, NEntityEvents::CIsEventCreatedAfter>::
    operator()<NEntityEvents::CEventHitBy*>(NEntityEvents::CEventHitBy* const&) const;