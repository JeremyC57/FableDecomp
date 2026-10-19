// Grade: DIFFER(ours v retail) -- 177v169: only the final release of the def local differs: retail `dec dword ptr [edi+4] ; jne` (direct count), ours the long ReleaseTemporary form `mov ecx,[edi+4] ; dec ecx ; mov eax,ecx ; test ; mov [edi+4],ecx`. The getter temporary needs the ReleaseTemporary dtor (long form) while the local needs the direct form; with one CDefPointer dtor written the direct way (tried) the local matches and the temporary does not. Next: find the second release path (a different pointer type for the local, or an explicit release).
// CInputProcessTargetLockCycleTargets::AddToInterface(CInputProcessManager& manager) -- reset the click detector (pressure 0, no pulse, pulses-only from the global setting), take the click threshold from the controlled creature's hero-combat def (0.75 without a live creature), then register through the base class
// Leaf: AddToInterface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00688C90, 169 bytes, `ret 4`.
// Callees: CPlayer::GetControlledCreature @ 0x00487DD0, the thing's hero-combat def getter @ 0x00663340 (CDefPointer by
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
template <class T> struct CDefPointer {
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
struct CHeroCombatDef : CDefObject {
    unsigned char _pad08[0xe4 - 0x08];
    float         ClickPressureThreshold;          // +0xe4
};
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _bits91 : 7;
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const;   // 0x00663340
};
class CPlayer {
public:
    CThing* GetControlledCreature();               // 0x00487DD0
};
extern bool g_targetLockAllowPulsesOnly;           // 0x013BA8B0
class CInputProcessManager;
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
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

bool CInputProcessTargetLockCycleTargets::AddToInterface(CInputProcessManager& manager)
{
    LastButtonPressure = 0.0f;
    ClickAcceptancePulseTimer = -1;
    AllowPulsesOnly = g_targetLockAllowPulsesOnly;
    CDefPointer<CHeroCombatDef> def;
    CThing* creature = MyPlayer->GetControlledCreature();
    if (creature && !creature->Dead) {
        def = creature->GetHeroCombatDef();
        ButtonPressureThresholdForClick = def->ClickPressureThreshold;
    } else {
        ButtonPressureThresholdForClick = 0.75f;
    }
    bool added = CInputProcessGameBase::AddToInterface(manager);
    return added;
}
