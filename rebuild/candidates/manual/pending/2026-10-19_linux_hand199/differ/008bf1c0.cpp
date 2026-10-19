// CCreatureAction_UnblockableMidAttack::Initialise() -- play STRIKE_MEDIUM_FRONT_UNBLOCKABLE, run the combat-action Initialise, then turn the creature to face its live target (unless it is targeting itself)
// Grade: DIFFER(178v178) -- scheduling only: retail loads the position interface's vtable (ebp) right after the second creature GetPItem, ours after the target's GetPosition (ebx/ebp swap follows)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF1C0, length 192 bytes (plain `ret`, virtual).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName(const
// CCharString&) 0x008574D0, CCombatActionBase::Initialise 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CThing::GetPosition 0x004C73D0, __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to)
// 0x00661540 (float, the XY heading from `from` to `to`), the creature's position interface (+0x60) virtual slot 66 (+0x108,
// the XY-angle setter; class not proven).
// Layout: PCreature +8, PTarget +0xA8.
// Shape: the facing call is one expression: the interface and its vtable are fetched first, the creature's GetPItem for the
// angle's `from` is evaluated before the target's position, and the float result goes out through `push ecx ; fstp [esp]`.
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00661540

struct CThingPositionInterface {                // the thing's placement interface at +0x60
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void SetAngleXY(float angle);       // slot 66 (+0x108)
};

struct CThing {
    unsigned char _pad0[0x60];
    CThingPositionInterface* m_pPositionInterface;   // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};

inline float AngleBetweenThings(const CThing* from, const CThing* to)
{
    return AngleBetweenPositions(from->GetPosition(), to->GetPosition());
}

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_UnblockableMidAttack : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_UnblockableMidAttack::Initialise()
{
    {
        CCharString name("STRIKE_MEDIUM_FRONT_UNBLOCKABLE", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1)) {
        if (PTarget.GetPItem() != PCreature.GetPItem())
            PCreature.GetPItem()->m_pPositionInterface->SetAngleXY(AngleBetweenThings(PCreature, PTarget));
    }
}
