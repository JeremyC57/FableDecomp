// CCrateTeamMember::IsThingCarryingCrate(const CScriptThing& thing) -- whether the thing carries one of the raid's live crates; every step re-checks IsOutOfAction (an out-of-action member answers false)
// Leaf: IsThingCarryingCrate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCED10, 210 bytes (224 with padding), `ret 4`.
// Callees: CCrateTeamMember::IsOutOfAction 0x00F35B30 (landed caller 0x00DCEC70; the manifest label is propagated),
//   the crate's CScriptThing virtual slots 75 (+0x12c, alive) and 63 (+0xfc, carried by), the thing's slot 1 (+4).
// Layout: Ego_r.pdb CCrateTeamMember (landed 0x00DCEC70): ParentClass +0x14; the parent script's crate vector of 12-byte
//   CScriptThings at +0x54.
// Shape: the result goes through an inline Return(value) = `if (IsOutOfAction()) return false; return value;` (the
//   false case keeps the call and drops its result; the true case is `neg ; sbb ; inc`); size() is recomputed per test.
struct CScriptThing {                       // 12 bytes: vptr + counted pointer
    virtual void v00();
    virtual void* GetThing() const;             // slot 1 (+0x04)
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
    virtual bool IsBeingCarriedBy(void* carrier) const;   // slot 63 (+0xfc)
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
    virtual bool IsAlive() const;               // slot 75 (+0x12c)
    void* PImp_Data;
    void* PImp_Info;
};

template<class T> struct CRetailVector {
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) const { return first[n]; }
};

struct CQ_OrchardFarmRaidScript {
    unsigned char _pad[0x54];
    CRetailVector<CScriptThing> Crates;     // +0x54
};

struct CCrateTeamMember {
    virtual void v00();
    unsigned char _pad04[0x10];
    CQ_OrchardFarmRaidScript* ParentClass;  // +0x14
    bool IsOutOfAction();                   // 0x00F35B30
    bool Return(bool value)
    {
        if (IsOutOfAction())
            return false;
        return value;
    }
    bool IsThingCarryingCrate(const CScriptThing& thing);
};

bool CCrateTeamMember::IsThingCarryingCrate(const CScriptThing& thing)
{
    for (unsigned long i = 0; i < ParentClass->Crates.size(); ++i) {
        if (IsOutOfAction())
            return false;
        if (ParentClass->Crates[i].IsAlive()) {
            if (ParentClass->Crates[i].IsBeingCarriedBy(thing.GetThing()))
                return Return(true);
        }
    }
    return Return(false);
}