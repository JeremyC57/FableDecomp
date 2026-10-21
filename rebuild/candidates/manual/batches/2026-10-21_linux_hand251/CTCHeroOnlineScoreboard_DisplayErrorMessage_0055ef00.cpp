// CTCHeroOnlineScoreboard::DisplayErrorMessage(const CWideString& message) -- hides the second connecting message, puts the message in the connecting panel's UI_CONNECTING_TEXT, gives its OK button action 0x112 and moves input focus from the mouse pointer to that button
// Leaf: OnOkClicked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055EF00, 385 bytes (`ret 4`). Manifest label OnOkClicked is wrong: the
// real OnOkClicked is 0x0055EE20 (landed); this one takes one by-reference wide-string message.
// Callees: CVectorMap::LowerBound @ 0x0040F020, CCharString(const char*, long) @ 0x0099EBF0, operator new @ 0x00BFEA1A,
// NUISystem::CAction::CAction @ 0x0042BE50, CCountedPointer<CAction>(CAction*) @ 0x0042AA29, operator delete @ 0x00BFE9BC,
// NUISystem::CManager::GetInstance @ 0x0041E5F2 (x4); component slots 3 (+0x0C FindChild), 48 (+0xC0), 71 (+0x11C AddAction),
// 144 (+0x240 SetText); manager slots 5 (+0x14) and 9 (+0x24); observer slots 3 (+0x0C ObserveEvent) and 6 (+0x18).
// Shape: the OK button is a reference (no null test on its CObserver conversion); the counted pointer's dtor is inline.
// Layout: retail CTCHeroOnlineScoreboard per the landed ctor 0x00562EC0 / ResetScoreboard 0x00562E70: PConnectingMessage
// +0x60, PConnectingMessage1 +0x64 (PDB +0x80/+0x84; the landed OnOkClicked 0x0055EE20 calls +0x60 PScoreboardScreen --
// a naming conflict for review); retail CTCInventory's PMouse_Pointer is +0x214 (PDB +0x260).
struct CTCBase;
struct CWideString;

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};

namespace NUISystem {
struct CAction {                                     // retail 0x10 bytes
    long  ActionType;                                // +0x00
    void* UIActionParams;                            // +0x04
    void* ThingActionParams;                         // +0x08
    bool  Valid;                                     // +0x0C
    CAction();                                       // 0x0042BE50
};
}

struct SCountBlock {
    long m_references;                               // +0
    void (__fastcall* m_pfnDestroy)(void*);          // +4
    void* m_pObject;                                 // +8
};
template <class T> struct CCountedPointer {
    T*           m_pObject;                          // +0
    SCountBlock* m_pCount;                           // +4
    CCountedPointer(T* object);                      // 0x0042AA29
    ~CCountedPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;                     // operator delete @ 0x00BFE9BC
            }
        }
    }
    T* operator->() const { return m_pObject; }
};

namespace NUISystem {
struct CObserver {
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void SetFocus_slot6();                   // slot 6 (+0x18)
};
struct CComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetActive_slot48(bool active);      // slot 48 (+0xC0)
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70();
    virtual void AddAction(const CCountedPointer<CAction>& action);   // slot 71 (+0x11C)
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81();
    virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91();
    virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95(); virtual void v96();
    virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
struct CComponent : CComponentBase, CObserver {
};
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void AddFocus_slot5(CObserver* observer);     // slot 5 (+0x14)
    virtual void m06(); virtual void m07(); virtual void m08();
    virtual void Notify_slot9(CObserver* observer);       // slot 9 (+0x24)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    unsigned char _pad_0x00[0x20];
    unsigned long m_presentBits[9];                  // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CTCInventory {
    unsigned char          _pad_0x000[0x214];
    NUISystem::CComponent* PMouse_Pointer;           // +0x214
};

struct CTCHeroOnlineScoreboard {
    void*                  __vftable;                // +0x00
    CThing*                ParentThing;              // +0x04
    unsigned char          _pad_0x08[0x58];
    NUISystem::CComponent* PConnectingMessage;       // +0x60
    NUISystem::CComponent* PConnectingMessage1;      // +0x64
    void OnOkClicked(const CWideString& message);
};

void CTCHeroOnlineScoreboard::OnOkClicked(const CWideString& message)
{
    CThing* thing = ParentThing;
    if (thing->HasTC(0x11)) {
        CTCInventory* inventory = thing->GetTC<CTCInventory>(0x11);
        PConnectingMessage1->SetActive_slot48(false);
        PConnectingMessage->FindChild(CCharString("UI_CONNECTING_TEXT", -1))->SetText(message, true, true);
        NUISystem::CComponent& okButton = *PConnectingMessage->FindChild(CCharString("PC_OK_GAME_OKBUTTON_SCOREBOARD", -1));
        CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
        action->ActionType = 0x112;
        okButton.AddAction(action);
        NUISystem::CManager::GetInstance()->AddFocus_slot5(inventory->PMouse_Pointer);
        NUISystem::CManager::GetInstance()->AddFocus_slot5(&okButton);
        NUISystem::CManager::GetInstance()->Notify_slot9(inventory->PMouse_Pointer);
        NUISystem::CManager::GetInstance()->Notify_slot9(&okButton);
        inventory->PMouse_Pointer->ObserveEvent(0x19);
        ((NUISystem::CObserver&)okButton).SetFocus_slot6();
        PConnectingMessage->SetActive_slot48(true);
    }
}
