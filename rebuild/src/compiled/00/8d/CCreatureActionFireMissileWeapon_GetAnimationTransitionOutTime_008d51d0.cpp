// CCreatureAction_FireMissileWeapon::GetAnimationTransitionOutTime() const -- 3 when the creature's virtual slot 77 says so, else the base value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D51D0, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), the creature's virtual slot 77 (+0x134; the
// AI-creature vtable names it only as vfunc_77 -- modelled descriptively as a bool predicate),
// CCreatureActionBase::GetAnimationTransitionOutTime @ 0x006924C0 (the base implementation, reached as a tail jump).
// Shape: a value-returning forwarder on the false path (`jmp` recovers byte-exact, unlike void forwarders). `UBEJXZ`:
// const, returns long.
struct CThingCreatureBase_Calls {           // engine/CThingCreatureBase.h exists but is not used: only slot 77 is modelled
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual bool IsQuickFiring() const;     // slot 77 (+0x134), name descriptive
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase_Calls* m_pItem;      // +0x04
    CThingCreatureBase_Calls* GetPItem() const;   // 0x00A01B50
};

struct CCreatureActionBase {                // engine/CCreatureActionBase.h exists but is not used: only PCreature and the base call are modelled (retail 0xa8 bytes, PDB 0xb4)
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    long GetAnimationTransitionOutTime() const;   // 0x006924C0 (base, non-virtual call here)
};

struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {
    long GetAnimationTransitionOutTime() const;
};

long CCreatureAction_FireMissileWeapon::GetAnimationTransitionOutTime() const
{
    if (PCreature.GetPItem()->IsQuickFiring())
        return 3;
    return CCreatureActionBase::GetAnimationTransitionOutTime();
}