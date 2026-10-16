#pragma optimize("y", off)
#pragma optimize("s", on)
// IsThingActive(const CScriptThing& thing) -- true while the script thing is alive, has not been killed or knocked out (by anything) and is not unconscious
// Leaf: IsThingActive
// Grade: RELOCATION_MATCH (155 bytes) -- the `&&` chain stored in a `char` local and returned through its implicit conversion gives retail's `cmp byte ptr [ebp-1],0 ; setne al` (the form that matched the twin 0x00CBE245; applied by the landing session)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBE1AA, length 155 bytes, `ret`; `__fastcall` (thing in ecx);
// frame-pointer size TU (`push ebp ; mov ebp,esp`, `push 3 ; pop ebx`): `optimize("y",off)` + `("s",on)`.
// Catalog label "CV_FisticuffsClubScript::IsKnotholeGladeNotAPunchClub" is a wrong propagation: the body is one of
// the global script-thing helpers (FoundMessageIsThingKnockedOutBy 0x00CBE10F .. IsThingAwareOfHero 0x00CBE2E0, which
// calls it) and only calls CScriptThing virtuals on its argument. The real name is not known (descriptive).
// Callees: CScriptThing (vtable 0x01238C8C) slots 75 IsAlive (+0x12c), 18 MsgIsKilledBy (+0x48), 32 MsgIsKnockedOutBy
// (+0x80), 61 IsUnconscious (+0xf4); CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the empty literal
// @ 0x0122D70E, twice), ~CCharString @ 0x0099EAE0.
// Shape: one `&&` chain returning bool; the "" arguments are IMPLICIT CCharString conversion temporaries (retail's
// `lea ecx ; push ecx`), tracked by flag bits in ebx and destroyed after the result byte is stored.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CScriptThing {                              // vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;       // slot 18 (+0x48, 0x004AAAF0)
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31();
    virtual bool MsgIsKnockedOutBy(const CCharString& attacker) const; // slot 32 (+0x80)
    virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual bool IsUnconscious() const;            // slot 61 (+0xf4)
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c)
    long m_data[2];
};
bool __fastcall IsThingActive(const CScriptThing& thing)
{
    char result = thing.IsAlive() && !thing.MsgIsKilledBy("") && !thing.MsgIsKnockedOutBy("") && !thing.IsUnconscious();
    return result;
}