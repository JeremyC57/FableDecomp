// CCombatActionBase::UpdateActionMoveToPos() -- targets the creature's current position, or the action's next position when the animation moves the creature, uses action movement, or a final position was forced
// Leaf: UpdateActionMoveToPos
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856D30, 155 bytes of code (protected virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice through one `lea edi,[esi+8]`),
//          CThingPhysical::GetPos @ 0x004C73D0 (const C3DVector&), virtual GetAnimationIndex (slot 7, +0x1c),
//          the creature's anim-movement length for an anim id @ 0x006627A0 (float: |movement| * k, 0.0 for anim 0;
//          manifest "CTCAnimationComplex::GetAnimDuration" is propagated from 0x006627F0), CCreatureActionBase::
//          IsUsingActionMovement @ 0x00692A50, virtual GetNextPos (slot 70, +0x118, C3DVector by hidden pointer),
//          CCreatureActionBase::SetMoveToPos(const C3DVector&) @ 0x006929E0.
// Global: the float 0.0001f @ 0x0129BA3C.
// Shape: the position is copied into a 12-byte frame local; the creature for the movement query is a named local
//          fetched BEFORE the slot-7 call (`mov edi,eax`); `> 0.0001f` is `test ah,41h ; je` into the shared
//          GetNextPos assignment; the 24-byte frame holds the local and GetNextPos' result temporary.
// Members (retail layout, Ego_r.pdb names): PreferredFinalPosSet +0xdd (PDB +0xf1), so `this` is modelled locally.
class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CThing { public:
    const C3DVector& GetPos() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing { public:
public:
    float GetAnimMovementDistance(long) const;   // 0x006627A0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual long GetAnimationIndex();           // slot 7 (+0x1c): 0x008565B0
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25();
    virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69();
    virtual C3DVector GetNextPos();             // slot 70 (+0x118): 0x008575C0
    unsigned char _pad04[4];                    // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;          // +0x08
    unsigned char _pad10[0xdd - 0x10];          // +0x10
    bool PreferredFinalPosSet;                  // +0xdd
    bool IsUsingActionMovement();               // 0x00692A50 (CCreatureActionBase)
    void SetMoveToPos(const C3DVector& pos);    // 0x006929E0 (CCreatureActionBase)
    void UpdateActionMoveToPos();
};

void CCombatActionBase::UpdateActionMoveToPos()
{
    C3DVector pos = PCreature.GetPItem()->GetPos();
    CThingCreatureBase* creature = PCreature.GetPItem();
    if (creature->GetAnimMovementDistance(GetAnimationIndex()) > 0.0001f || IsUsingActionMovement() || PreferredFinalPosSet)
        pos = GetNextPos();
    SetMoveToPos(pos);
}