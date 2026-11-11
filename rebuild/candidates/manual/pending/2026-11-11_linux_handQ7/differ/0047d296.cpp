// CTCInGameMenu::PCSetQuitToFrontEnd() -- set the PC yes/no dialog up as the "quit to the front end" question: yes/no
// Grade: DIFFER(482v482) -- the push ecx/fsubp swap is FIXED by `(GetScreenWidth() * 0.5f) - ...` (PAREN tuple, as the matched twins 0x0047E651/0x0047E9AE/0x0047F30F); left: in the 3rd observer call (RemoveObserver(g_pPCMousePointer), 0x0047D3A1) retail loads the manager vtable before the global (ours global first). Next: c2prio on that call; the global-first order holds for the other three calls.
// Leaf: ?PCSetQuitToFrontEnd@CTCInGameMenu@@
// callbacks, shown and centred on the screen, then made the active dialog with the mouse pointer: both registered with
// the UI manager, and both buttons observed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047D296, 482 bytes, plain `ret` (frame pointer, `leave`).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
float GetScreenWidth();                              // 0x0041CC14
float GetScreenHeight();                             // 0x0041CC2B

namespace NUISystem {
struct CObserver {                                   // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void ObserveAllEvents();                   // slot 6 (+0x18)
};
typedef void (*TCallback)();
struct CComponentBase {                              // the primary vtable
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); 
    virtual void SetPosition(const C2DVector& position, float depth, bool flag);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void SetActive(bool active);      // slot 48 (+0xC0)
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); 
    virtual void SetYesCallback(TCallback callback); // slot 142 (+0x238)
    virtual void SetNoCallback(TCallback callback);  // slot 143 (+0x23C)
};
struct CComponent : CComponentBase, CObserver {
};
struct CYesNo : CComponent {
};
struct CManager {                                    // NUISystem::CManager (CObservable vtable part)
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void RemoveObserver(CObserver* observer);   // slot 5 (+0x14)
    virtual void m06(); virtual void m07(); virtual void m08();
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);   // slot 9 (+0x24)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
    C2DVector GetUIScale();                          // 0x0041CF47
};
}

extern NUISystem::CComponent* g_pPCMousePointer;  // 0x013B87A4
extern NUISystem::CYesNo*     g_pPCYesNoDialog;   // 0x013B87A8

struct CThing;
struct CTCInGameMenu {
    void*                  __vftable;                // +0x00
    CThing*                ParentThing;              // +0x04 (CTCBase)
    unsigned char          _pad_0x08[0xE4];
    NUISystem::CComponent* PMousePointer;            // +0xEC (PDB +0x110)
    NUISystem::CYesNo*     PYesNoDialog;             // +0xF0
    static void QuitToFrontEndYes();                 // 0x0047D289
    static void YesNoDialogNo();                     // 0x0047D0D5
    void PCSetQuitToFrontEnd();
};

void CTCInGameMenu::PCSetQuitToFrontEnd()
{
    PYesNoDialog->SetYesCallback(QuitToFrontEndYes);
    PYesNoDialog->SetActive(true);
    C2DVector position((GetScreenWidth() * 0.5f) - NUISystem::CManager::GetInstance()->GetUIScale().X * 104.0f,
                       GetScreenHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PYesNoDialog->SetPosition(position, 0.0f, false);
    PYesNoDialog->SetNoCallback(YesNoDialogNo);
    g_pPCYesNoDialog = PYesNoDialog;
    g_pPCMousePointer = PMousePointer;
    NUISystem::CManager::GetInstance()->RemoveObserver(g_pPCYesNoDialog);
    NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCYesNoDialog);
    NUISystem::CManager::GetInstance()->RemoveObserver(g_pPCMousePointer);
    NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pPCMousePointer);
    g_pPCMousePointer->ObserveEvent(0x19);
    g_pPCYesNoDialog->ObserveAllEvents();
    NUISystem::CComponent* yesButton = PYesNoDialog->FindChild(CCharString("PC_YESNO_GAME_YESBUTTON", -1));
    if (yesButton) {
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(yesButton);
        yesButton->ObserveEvent(0x19);
    }
    NUISystem::CComponent* noButton = PYesNoDialog->FindChild(CCharString("PC_YESNO_GAME_NOBUTTON", -1));
    if (noButton) {
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(noButton);
        noButton->ObserveEvent(0x19);
    }
}
