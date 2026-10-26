// CTCLook::IsAbleToTurnViaAnimation() -- turning via animation is allowed when the def allows it (combat or normal flag by the creature's combat state), no current action forbids it, turning animations exist, turning via animation is not disabled and the thing can use turning animations
// Grade: RELOCATION_MATCH -- IS_ALIVE as a template taking `T* const&` bound straight to the PParentCreature member: retail then loads it into eax for the null/dead tests and copies it to ecx for the call (`mov ecx,eax`, twice); a by-value pointer parameter loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752480, length 137 bytes.
// Callees: CThingCreatureBase combat-state query @ 0x00661E80 (bits 2/3 of the byte at +0x15C; catalog "CText::GetAlignment" is a
// byte twin), CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, the current action's (+0x100) virtual slot 50 (+0xC8),
// IsAbleToUseTurningAnimations(CThing*) @ 0x0085FCD0 (__fastcall).
// Layout (retail, local model): ParentThing +0x04, PParentCreature +0x10, PDef +0x14 (CLookDef AllowTurningViaAnimation +0x40,
// CombatAllowTurningViaAnimation +0x4C), TurningViaAnimationDisable +0x41, TurningAnims vector begin/end +0x8C/+0x90 (PDB offsets);
// the creature's dead bit is CThing +0x91 bit 0.
// Shape: the def flag is a conditional read into al; IS_ALIVE inlined twice; nested ifs with one shared `xor al,al` exit.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                         // +0x91 bit 0: dead
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49();
    virtual bool PreventsTurningViaAnimation();      // slot 50 (+0xc8)
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad92[0x100 - 0x92];
    CCreatureActionBase* PCurrentAction;             // +0x100
    bool IsInCombatMode() const;                     // 0x00661E80
    bool IsPerformingAnyAction() const;              // 0x006622A0
};
template<class T> inline bool IS_ALIVE(T* const& p)
{
    return p != 0 && !(p->m_flags91 & 1);
}
bool __fastcall IsAbleToUseTurningAnimations(CThing* thing);   // 0x0085FCD0
class CLookDef { public:
    unsigned char _pad00[0x40];
    bool  AllowTurningViaAnimation;                  // +0x40
    unsigned char _pad41[0x4c - 0x41];
    bool  CombatAllowTurningViaAnimation;            // +0x4c
};
template<class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CAnimationEntry;
struct CTurningAnim { float Angle; const CAnimationEntry* Anim; };
class CTCLook { public:
    virtual void v00();
    CThing* ParentThing;                             // +0x04 (CTCBase)
    unsigned char _base08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;             // +0x10
    CDefPointer<const CLookDef> PDef;                // +0x14
    unsigned char _pad18[0x41 - 0x18];
    bool  TurningViaAnimationDisable;                // +0x41
    unsigned char _pad42[0x8c - 0x42];
    CTurningAnim* TurningAnimsStart;                 // +0x8c TurningAnims (retail 12-byte vector)
    CTurningAnim* TurningAnimsFinish;                // +0x90
    CTurningAnim* TurningAnimsEnd;                   // +0x94
    bool IsAbleToTurnViaAnimation();
};

bool CTCLook::IsAbleToTurnViaAnimation()
{
    bool allowed;
    if (IS_ALIVE(PParentCreature) && PParentCreature->IsInCombatMode())
        allowed = PDef->CombatAllowTurningViaAnimation;
    else
        allowed = PDef->AllowTurningViaAnimation;
    if (allowed) {
        if (!(IS_ALIVE(PParentCreature) && PParentCreature->IsPerformingAnyAction() && PParentCreature->PCurrentAction->PreventsTurningViaAnimation())) {
            if (TurningAnimsStart != TurningAnimsFinish && !TurningViaAnimationDisable) {
                if (IsAbleToUseTurningAnimations(ParentThing))
                    return true;
            }
        }
    }
    return false;
}