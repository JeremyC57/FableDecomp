// CGlobal_WatchForHeroDeathScript::WatchForHeroDeath() -- the global script's watch loop: while the script thread runs, pump frames while the hero is valid and alive; once not, pump frames and look again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE8FE0, length 153 bytes (padded to 160).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (on `this`), the game interface (+0x40) virtual slots 70
//          (+0x118, the hero script thing by reference) and 7 (+0x1c, Pump), the script thing's slots 75 (+0x12c, IsValid)
//          and 61 (+0xf4, the dead test).
// Shape: `while (!IsActiveThreadTerminating())` around an inner `while (valid && !dead) { Pump(); if (terminating) return; }`
// and a trailing `if (terminating) return; Pump();` -- both loops rotated (the valid test duplicated at the inner loop's
// bottom, the `lea esp,[esp]` alignment pad before its head). The class layout follows the landed sibling
// 0x00EFAFC0 (CScriptBase-derived, the interface pointer at +0x40).
struct CScriptThing_Calls {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual bool IsDead() const;                   // slot 61 (+0xf4)
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
struct CGameScriptInterface_Calls {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void Pump();                           // slot 7 (+0x1c)
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual const CScriptThing_Calls& GetHero() const;   // slot 70 (+0x118)
};
struct CScriptBase_Retail {
    virtual void v00();
    unsigned char _pad[0x3c];                      // +0x04 .. +0x40 (the active thread is at +0x2c)
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CGlobal_WatchForHeroDeathScript : CScriptBase_Retail {
    CGameScriptInterface_Calls* m_pInterface;      // +0x40
    void WatchForHeroDeath();
};

void CGlobal_WatchForHeroDeathScript::WatchForHeroDeath()
{
    while (!IsActiveThreadTerminating()) {
        while (m_pInterface->GetHero().IsValid() && !m_pInterface->GetHero().IsDead()) {
            m_pInterface->Pump();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        m_pInterface->Pump();
    }
}