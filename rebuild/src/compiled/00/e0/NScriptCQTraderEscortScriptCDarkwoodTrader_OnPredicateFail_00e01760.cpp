// NScript::CQ_TraderEscortScript::CDarkwoodTrader::OnPredicateFail() -- when the trader's timer is still running:
// one trader fewer, stop the escort timer, then have the nearest "DarkwoodTrader" comment HERO_KILLED_TRADER (killed by
// the hero), TRADER_DIED (two left) or LAST_TRADER (one left); finally clear the byte at +0x10b of the state at +0x18.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E01760, length 416 bytes (0x195 code + int3 pad).
// Callees: g_pGameScriptInterface (0x0143E8F8) GetTimer slot 90 / SetTimer slot 89; 0x0099EBF0 CCharString(const char*,
// long); CScriptThing slot 18 MsgIsKilledBy; CGameScriptInterface (+4) slot 77 GetNearestWithScriptName (by value);
// 0x00E01900 CQ_TraderEscortScript::MakeTraderComment(const CCharString&, const CScriptThing&, ETraderComment)
// (mangled name from the label TSVs); 0x004AA840 ~CScriptThing; 0x0099EAE0 ~CCharString. Slots: ghidra_out/cgsi_slots_aligned.tsv.
// Layout: descriptive (no PDB class): game +4, CScriptThing +8, parent script +0x14 (timer +0x48, traders left +0x50),
// state pointer +0x18, timer +0x2c. ETraderComment enumerator names unknown (values 0 and 1 used).
// Shape: implicit-literal CCharString arguments; the nested by-value GetNearestWithScriptName temporary is destroyed after
// the call. Matched first try.
// Leaf: OnPredicateFail
class CCharString { public:
    char* m_pString;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CScriptThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;  // slot 18 (+0x48)
    void* m_pImpObject;                                  // +0x04 PImp
    void* m_pImpCount;                                   // +0x08
    ~CScriptThing();                                     // 0x004AA840
};
class CGameScriptInterface { public:
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const; // slot 77 (+0x134)
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void SetTimer(long timer, long time);                 // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;                       // slot 90 (+0x168)
};
extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

namespace NScript {
class CQ_TraderEscortScript { public:
    enum ETraderComment { TRADER_COMMENT_0, TRADER_COMMENT_1 };
    unsigned char _pad00[0x48];
    long m_TraderTimer;                                  // +0x48
    unsigned char _pad4c[0x4];
    long m_TradersLeft;                                  // +0x50
    bool MakeTraderComment(const CCharString& text, const CScriptThing& trader, ETraderComment comment); // 0x00E01900
    struct CDarkwoodTrader;
};
struct STraderState {
    unsigned char _pad00[0x10b];
    bool m_flag10b;                                      // +0x10b
};
struct CQ_TraderEscortScript::CDarkwoodTrader {
    virtual void v00();
    CGameScriptInterface* m_pGame;                       // +0x04
    CScriptThing m_Thing;                                // +0x08
    CQ_TraderEscortScript* m_pScript;                    // +0x14
    STraderState* m_pState;                              // +0x18
    unsigned char _pad1c[0x10];
    long m_Timer;                                        // +0x2c
    void OnPredicateFail();
};

void CQ_TraderEscortScript::CDarkwoodTrader::OnPredicateFail()
{
    if (g_pGameScriptInterface->GetTimer(m_Timer) > 0) {
        --m_pScript->m_TradersLeft;
        g_pGameScriptInterface->SetTimer(m_pScript->m_TraderTimer, 0);
        if (m_Thing.MsgIsKilledBy("SCRIPT_NAME_HERO"))
            m_pScript->MakeTraderComment("HERO_KILLED_TRADER", m_pGame->GetNearestWithScriptName(m_Thing, "DarkwoodTrader"), TRADER_COMMENT_0);
        else if (m_pScript->m_TradersLeft == 2)
            m_pScript->MakeTraderComment("TRADER_DIED", m_pGame->GetNearestWithScriptName(m_Thing, "DarkwoodTrader"), TRADER_COMMENT_1);
        else if (m_pScript->m_TradersLeft == 1)
            m_pScript->MakeTraderComment("LAST_TRADER", m_pGame->GetNearestWithScriptName(m_Thing, "DarkwoodTrader"), TRADER_COMMENT_0);
        m_pState->m_flag10b = false;
    }
}
}