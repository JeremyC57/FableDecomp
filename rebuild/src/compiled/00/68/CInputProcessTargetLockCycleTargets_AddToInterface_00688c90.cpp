// CInputProcessTargetLockCycleTargets::AddToInterface(CInputProcessManager& manager) -- reset the click detector (pressure 0, no pulse, pulses-only from the global setting), take the click threshold from the controlled creature's hero-combat def (0.75 without a live creature), then register through the base class
// Grade: RELOCATION_MATCH -- the def local is a separate holder type whose destructor releases with a direct `if (--m_count == 0) Destroy();` (retail `dec [edi+4] ; jne`) while the getter's CDefPointer temporary keeps the ReleaseTemporary long form; the holder's assignment from CDefPointer is CDefPointer's own operator= (add-ref, old object known null)
// Leaf: AddToInterface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00688C90, 169 bytes, `ret 4`.
// Callees: CPlayer::PeekPControlledCreature @ 0x00487DD0, the thing's hero-combat def getter @ 0x00663340 (CDefPointer by
//   hidden pointer; the label "CEventKilledCreature" is propagated -- landed callers 0x00789C20 / 0x006D6EE0 name it),
//   the def's slot 1 (Destroy) on release, CInputProcessGameBase::AddToInterface @ 0x00687A70.
// Globals: the pulses-only setting byte @ 0x013BA8B0.
// Layout: Ego_r.pdb CInputProcessTargetLockCycleTargets: LastButtonPressure +0x28, ButtonPressureThresholdForClick +0x2c,
//   ClickAcceptancePulseTimer +0x30, AllowPulsesOnly +0x34 over CInputProcessGameBase (MyPlayer +0x18); the hero-combat
//   def's click-pressure float sits at retail +0xe4 (Ego_r.pdb field not resolved: the retail def's maps are narrower).
// Shape: the def pointer is a function-scope local assigned from the getter's temporary (CDefPointer::operator= with the
//   null local folds to `if (o) { def = o; ++count; }`), the temporary's null-tested dtor releases through the def's
//   inline ReleaseTemporary() and clears; the local is released after the base call, whose bool rides in bl.
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CDefObject : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
class CHeroCombatDef : public CDefObject { public:
    unsigned char _pad08[0xe4 - 0x08];
    float         ClickPressureThreshold;          // +0xe4
};
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _bits91 : 7;
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const;   // 0x00663340
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    const CThingPlayerCreature* PeekPControlledCreature() const;               // 0x00487DD0
};
extern bool g_targetLockAllowPulsesOnly;           // 0x013BA8B0
class CInputProcessManager;
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    virtual bool AddToInterface(CInputProcessManager& manager);   // 0x00687A70
};
class CInputProcessTargetLockCycleTargets : public CInputProcessGameBase {
public:
    float LastButtonPressure;                      // +0x28
    float ButtonPressureThresholdForClick;         // +0x2c
    long  ClickAcceptancePulseTimer;               // +0x30
    bool  AllowPulsesOnly;                         // +0x34
    virtual bool AddToInterface(CInputProcessManager& manager);
};

struct CHeroCombatDefHolder {
    CHeroCombatDef* Object;
    CHeroCombatDefHolder() : Object(0) { }
    ~CHeroCombatDefHolder() { if (Object) { if (--Object->m_count == 0) Object->Destroy(); } }
    CHeroCombatDefHolder& operator=(const CDefPointer<CHeroCombatDef>& o)
    {
        if (Object != o.Object) {
            CHeroCombatDef* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    CHeroCombatDef* operator->() const { return Object; }
};
bool CInputProcessTargetLockCycleTargets::AddToInterface(CInputProcessManager& manager)
{
    LastButtonPressure = 0.0f;
    ClickAcceptancePulseTimer = -1;
    AllowPulsesOnly = g_targetLockAllowPulsesOnly;
    CHeroCombatDefHolder def;
    const CThing* creature = MyPlayer->PeekPControlledCreature();
    if (creature && !creature->Dead) {
        def = creature->GetHeroCombatDef();
        ButtonPressureThresholdForClick = def->ClickPressureThreshold;
    } else {
        ButtonPressureThresholdForClick = 0.75f;
    }
    bool added = CInputProcessGameBase::AddToInterface(manager);
    return added;
}