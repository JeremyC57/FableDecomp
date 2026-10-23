// CTCInventoryWeapons::ConstructWieldEntryPC(bool melee, NUISystem::CViewport* viewport, NUISystem::CComponent* list, long defIndex) -- PC twin of ConstructWieldEntry: the guide text is the melee/ranged PC wield guide plus two separators and the item def's description, and both texts are also registered with the optional list
// Grade: DIFFER(638v638) -- same single residue as 0x005C323A: retail loads the entry component into esi after the viewport call
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C3444, 638 bytes, `ret 14h`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// FableWin.pdb: ?ConstructWieldEntryPC@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@PAVCComponent@4@J@Z
// Callees as 0x005C323A, plus CWideString copy @ 0x0099B720 (from the empty string 0x013BCA24) / operator= @ 0x0099B7D0 /
// operator+= @ 0x0099B8D0 (twice with the static string 0x013B8BA8), the item-def description @ 0x005D9630 (`__fastcall`, hidden
// result in ecx), and the list's slot 124 (+0x1F0).
// Shape: see 0x005C323A (implicit const char* conversions, the guide bank / description component read early, a named entry
// component pointer); tried a reference local, a split declaration and a same-TU counted-pointer ctor body for the esi load.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CWideString {
    void* m_pStringData;
    CWideString(const CWideString& other);                        // 0x0099B720
    ~CWideString();                                               // 0x0099B510
    CWideString& operator=(const CWideString& other);             // 0x0099B7D0
    const CWideString& operator+=(const CWideString& other);      // 0x0099B8D0
};
extern const CWideString g_EmptyWideString;                       // 0x013BCA24
extern const CWideString g_NewLineWideString;                     // 0x013B8BA8 (a static separator string; name descriptive)
CWideString __fastcall GetItemDefDescription(long defIndex);      // 0x005D9630
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);              // 0x0099EBF0
    CCharString(const CCharString& other);                        // 0x0099EC30
    ~CCharString();                                               // 0x0099EAE0
};
struct CDataBank {                                                // NGameText::CDataBank
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                                       // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;                  // 0x013B86A0
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                                   // 0x00429C15 (CComponent) / 0x0042AA29 (CAction)
    CCountedPointer(const CCountedPointer& other);                // 0x00428928
    ~CCountedPointer();                                           // 0x004291DE (CComponent) / 0x00429160 (CAction)
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CAction {                                                  // retail 0x10 bytes
    long  ActionType;                                             // +0x00
    void* UIActionParams;                                         // +0x04
    void* ThingActionParams;                                      // +0x08
    bool  Valid;                                                  // +0x0C
    CAction();                                                    // 0x0042BE50
};
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void AddChild(const CCountedPointer<CComponent>& child);        // slot 59 (+0xEC)
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void AddSelectedAction(const CCountedPointer<CAction>& action); // slot 66 (+0x108)
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void AddListedChild(CComponent* child);                         // slot 124 (+0x1F0)
    virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void SetViewportChild(CComponent* child);                       // slot 142 (+0x238)
    virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);          // slot 144 (+0x240)
};
struct CViewport : CComponent {
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    CComponent* CreateMenuEntryComponent(const CWideString& text, float a, float b, const CCharString& definition,
                                         float c, CCharString icon, float d, CCharString button);   // 0x00428494
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();            // 0x0041E5F2
};
}

struct CTCInventoryWeapons {
    CCountedPointer<NUISystem::CComponent> ConstructWieldEntryPC(bool melee, NUISystem::CViewport* viewport, NUISystem::CComponent* list, long defIndex);
};

CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructWieldEntryPC(bool melee, NUISystem::CViewport* viewport, NUISystem::CComponent* list, long defIndex)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CWideString text = bank->GetTextBySymbol("TEXT_GUI_MENU_WIELD");
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f,
        "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("PC_DESCRIPTION", false));
    CDataBank* guideBank = g_pMainGameComponent->TextBank;
    NUISystem::CComponent* descriptionComponent = description.m_pObject;
    CCharString guide(melee ? "TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_WIELD_PC" : "TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_WIELD_PC");
    CWideString guideText(g_EmptyWideString);
    guideText = guideBank->GetTextBySymbol(guide);
    guideText += g_NewLineWideString;
    guideText += g_NewLineWideString;
    guideText += GetItemDefDescription(defIndex);
    descriptionComponent->SetText(guideText, true, true);
    NUISystem::CComponent* entryComponent = entry.m_pObject;
    viewport->SetViewportChild(description.m_pObject);
    entryComponent->AddChild(description);
    if (list)
        list->AddListedChild(description.m_pObject);
    CCountedPointer<NUISystem::CComponent> name(manager->CreateComponent("PC_SUBTITLE", false));
    name->SetText(text, true, true);
    entryComponent->AddChild(name);
    if (list)
        list->AddListedChild(name.m_pObject);
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = 1;
    entryComponent->AddSelectedAction(action);
    return entry;
}
