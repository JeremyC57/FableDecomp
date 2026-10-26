// CTCProjectileTrap::TriggerTrap() -- the trap fires: restart the shot timer from the def's seconds-between-shots, count the shot, launch a SHOT_ARROW_SINGLE shot from the trap thing and reset the transition parameters
// Leaf: TriggerTrap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00787CA5, 170 bytes (plain `ret`; PDB ?TriggerTrap@CTCProjectileTrap@@MAEXXZ). The trap TU is
//   size-optimised with a frame pointer (`push ebp ; mov ebp,esp ... leave`): `optimize("y", off)` + `("s", on)`.
// Callees: __ftol2 @ 0x00BFEA70, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CTCBase::GetDefinitionManager()
//   const @ 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const @ 0x009AD410, CreateShot(...)
//   @ 0x007C0AD0 (`__fastcall`, as the landed CCreatureAction_TrollProjectileAttack::PerformAction 0x008C6430: accuracy 100, the
//   empty ability out pointer), CTCTrapBase::SetTransitionParams(float, float) @ 0x00787C35 (PDB IAEXMM; manifest "ConstructString"
//   is propagated). Globals: g_framesPerSecond @ 0x01375550; 100.0f @ 0x01230A08.
// Layout: Ego_r.pdb CTCTrapBase with 12-byte retail vectors (PTrapDef +0x54, TrapType +0x5c) and CTCProjectileTrap ShotTimer +0x60,
//   NumShotsThisRound +0x68; CTrapDef SecondsBetweenShots +0x60 (PDB +0x64: def bodies sit 4 bytes low).
// Shape: the name string and the ability pointer each live in their own block (one shared [ebp-4] frame word); in the size TU
//   the ability pointer's release (`dec [eax] ; cmp [eax],0`) inlines only with `__forceinline` on its destructor and helpers.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C3DVector;
class CRightHandedSet;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
struct CRefCounted {                                   // second base of the def: the count at +4
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                            // slot 1
};
class CMeleeCombatAbilityDef : public CDefVirtual, public CRefCounted { public:
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    __forceinline ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
class CThing;
CThing* __fastcall CreateShot(long defIndex, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                              CDefPointer<CMeleeCombatAbilityDef>* ability, C3DVector* position, CRightHandedSet* frame);   // 0x007C0AD0
extern long g_framesPerSecond;                         // 0x01375550
class CTrapDef { public:
    unsigned char _pad00[0x60];
    float SecondsBetweenShots;                         // +0x60 (PDB +0x64)
};
class CTCTrapBase { public:
    virtual void v00();
    CThing*       ParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad08[0x54 - 0x08];
    const CTrapDef* PTrapDef;                          // +0x54 CDefPointer (PDB +0x60)
    bool          Active;                              // +0x58
    bool          Triggered;                           // +0x59
    bool          LastTriggerOnProximity;              // +0x5a
    long          TrapType;                            // +0x5c ETrapType
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    void SetTransitionParams(float a, float b);        // 0x00787C35
};
struct CTCProjectileTrap : CTCTrapBase {
    long ShotTimer;                                    // +0x60
    long IdleTimer;                                    // +0x64
    long NumShotsThisRound;                            // +0x68
    virtual void TriggerTrap();
};

void CTCProjectileTrap::TriggerTrap()
{
    ShotTimer = (long)(g_framesPerSecond * PTrapDef->SecondsBetweenShots);
    ++NumShotsThisRound;
    long shotIndex;
    {
        CCharString name("SHOT_ARROW_SINGLE", -1);
        shotIndex = GetDefinitionManager().GetDefGlobalIndexFromName(name);
    }
    {
        CDefPointer<CMeleeCombatAbilityDef> ability;
        CreateShot(shotIndex, ParentThing, 0, 0, 100.0f, 1.0f, 1.0f, &ability, 0, 0);
    }
    SetTransitionParams(0.0f, 0.0f);
}