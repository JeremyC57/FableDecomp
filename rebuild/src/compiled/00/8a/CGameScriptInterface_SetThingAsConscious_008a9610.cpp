// CGameScriptInterface::SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const -- knock a live creature out (fall-stunned, with an animation) or bring it round (recover from stunned)
// Leaf: ?SetThingAsConscious@CGameScriptInterface@@UBEXABVCScriptThing@@_NABVCCharString@@@Z
// Retail: Fable.exe @ 0x008A9610, 162 bytes, `ret 0Ch`; Ego_r.pdb same symbol.
// Body: if thing.IsAlive() (CScriptThing slot 75, +0x12c) and its thing (slot 11, +0x2c) is non-null, not dead
// (+0x91 bit 0), a creature (ThingTypeFlags +0x6c bit 3) holding interface 0x31 (UsingComponents[1] bit 17, +0x24):
// conscious -> SetCurrentAction(CCombatAction_RecoverFromStunned(creature, 0)) (ctor 0x008AE1B0, vtable 0x0127851C, RTTI
// .?AVCCombatAction_RecoverFromStunned@@); else SetCurrentAction(CCombatAction_FallStunned(creature, 0, anim)) (ctor
// 0x008EDA90, Ego_r.pdb ??0CCombatAction_FallStunned@@QAE@AAVCThingCreatureBase@@PAV1@ABVCCharString@@@Z);
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0; both temporaries die through
// ~CCombatActionBase 0x0062E900 (the derived destructors are implicit).
// Layout: CThing retail (UsingComponents +0x20, ThingTypeFlags +0x6c, Dead +0x91; PDB +0x7c / +0xa1). The actions are
// Ego_r.pdb 300 / 304 bytes; retail's shared temporary slot is 0x118, so the model gives CCombatActionBase 0x114 bytes and
// RecoverFromStunned its 4 extra bytes. CScriptThing is declared locally with its virtuals (engine/CScriptThing.h
// flattens the vptr).
// Shape: /O2 (16-aligned entry).
class CCharString;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                        // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CScriptThing {
public:
    virtual ~CScriptThing();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual CThing* GetPThing() const;                   // slot 11 (+0x2c)
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                        // slot 75 (+0x12c)
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
};
class CThingCreatureBase;
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase(CThingCreatureBase& creature, CThingCreatureBase* other);
    virtual ~CCombatActionBase();                        // 0x0062E900
    unsigned char _pad04[0x114 - 4];
};
class CCombatAction_RecoverFromStunned : public CCombatActionBase {
public:
    CCombatAction_RecoverFromStunned(CThingCreatureBase& creature, CThingCreatureBase* other);   // 0x008AE1B0
    unsigned long _pad114;
};
class CCombatAction_FallStunned : public CCombatActionBase {
public:
    CCombatAction_FallStunned(CThingCreatureBase& creature, CThingCreatureBase* other, const CCharString& anim); // 0x008EDA90
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);    // 0x006644F0
};
class CGameScriptInterface {
public:
    virtual void SetThingAsConscious(const CScriptThing& thing, bool conscious, const CCharString& anim) const;
};

void CGameScriptInterface::SetThingAsConscious(const CScriptThing& thing, bool conscious, const CCharString& anim) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        if (t && !t->IsDead() && (t->ThingTypeFlags & 8) && t->HasTC(0x31))
        {
            CThingCreatureBase* creature = (CThingCreatureBase*)t;
            if (conscious)
                creature->SetCurrentAction(CCombatAction_RecoverFromStunned(*creature, 0));
            else
                creature->SetCurrentAction(CCombatAction_FallStunned(*creature, 0, anim));
        }
    }
}