// CTCInventoryWeapons::ConstructWieldEntry(bool melee, NUISystem::CViewport* viewport) -- builds the "wield" menu entry: a menu entry component labelled TEXT_GUI_MENU_WIELD, a one-off description text (melee / ranged wield guide) shown in the viewport, a name text, and a type-1 select action; returns the entry by value
// Grade: DIFFER(522v522) -- one scheduling difference: retail loads the entry component into esi AFTER the viewport call (`mov esi,[ebp-30h]`), ours before it
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C323A, 522 bytes, `ret 0Ch`. Frame-pointer size TU: optimize("y",off) + ("s",on).
// FableWin.pdb: ?ConstructWieldEntry@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NPAVCViewport@NUISystem@@@Z
// Callees: NGameText::CDataBank::GetTextBySymbol @ 0x009C95E0 (g_pMainGameComponent 0x013B86A0 +0x14), CFrontEndManager::GetInstance
// @ 0x0041E5F2, CManager::CreateMenuEntryComponent(const CWideString&, float, float, const CCharString&, float, CCharString, float,
// CCharString) @ 0x00428494, CManager::CreateComponent @ 0x0041DB1D, CCountedPointer<CComponent>(T*) @ 0x00429C15 / copy @ 0x00428928 /
// dtor @ 0x004291DE, CAction @ 0x0042BE50 + CCountedPointer<CAction> @ 0x0042AA29 / dtor @ 0x00429160, operator new @ 0x00BFEA1A,
// component slots 59 AddChild, 66 (+0x108), 142 (+0x238), 144 SetText; CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, ~CWideString @ 0x0099B510.
// Shape: the CCharString definition / symbol arguments are `const char*` converted implicitly (the temporary is built before the
// pushes and passed by `lea`); the by-value CCharString parameters are built in their argument slots. The guide's bank and the
// description component are read into locals before the guide string is built; the entry component is a named pointer local
// (it moves the entry to the far frame slot, -0x30, as in retail).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CWideString {
    void* m_pStringData;
    ~CWideString();                                               // 0x0099B510
};
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
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
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
    CCountedPointer<NUISystem::CComponent> ConstructWieldEntry(bool melee, NUISystem::CViewport* viewport);
};

CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructWieldEntry(bool melee, NUISystem::CViewport* viewport)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CWideString text = bank->GetTextBySymbol("TEXT_GUI_MENU_WIELD");
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(text, 0.0f, 1.0f,
        "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    CDataBank* guideBank = g_pMainGameComponent->TextBank;
    NUISystem::CComponent* descriptionComponent = description.m_pObject;
    CCharString guide(melee ? "TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_WIELD" : "TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_WIELD");
    descriptionComponent->SetText(guideBank->GetTextBySymbol(guide), true, true);
    NUISystem::CComponent* entryComponent = entry.m_pObject;
    viewport->SetViewportChild(description.m_pObject);
    entryComponent->AddChild(description);
    CCountedPointer<NUISystem::CComponent> name(manager->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW", false));
    name->SetText(text, true, true);
    entryComponent->AddChild(name);
    CCountedPointer<NUISystem::CAction> action(new NUISystem::CAction);
    action->ActionType = 1;
    entryComponent->AddSelectedAction(action);
    return entry;
}
