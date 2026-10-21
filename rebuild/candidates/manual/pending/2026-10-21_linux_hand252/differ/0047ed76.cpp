// Grade: DIFFER(366v366) -- one x87/push scheduling swap: retail `fsubp st(1) ; push ecx` before the SetPosition float slot, ours `push ecx ; fsubp st(1)`; a named y local fixes the order but lets the second GetUIScale temp reuse the first's frame slot
// CTCInGameMenu::SetToSave(long saveIndex) -- remember the save slot and wait two frames, create the "save in progress" dialog, set its text while between quest screens, show it centred on screen and stamp the time
// Leaf: SetToSave
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047ED76, 366 bytes, `ret 4`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (as the landed siblings 0x0047DA50 / 0x0047D1AC).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("PC_SAVE_IN_PROGRESS_DIALOG"
//   @ 0x01236A00, "PC_SAVE_IN_PROGRESS_TEXT" @ 0x012369E4, "TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC" @ 0x012369C0),
//   NUISystem::CManager::GetInstance @ 0x0041E5F2, CManager::CreateComponent(const CCharString&, bool) @ 0x0041DB1D,
//   vector<CComponent*>::push_back @ 0x00486024, CQuestManager::IsBetweenQuestStartAndFinishScreens @ 0x004AF290 on the
//   global quest manager @ 0x013B89FC, the component's virtual slots 3 FindChild(CCharString by value), 145 (+0x244,
//   text by tag), 43 (+0xAC) and 31 (+0x7C, position), screen width / height @ 0x0041CC14 / 0x0041CC2B,
//   CManager::GetUIScale @ 0x0041CF47 (C2DVector by hidden pointer), GFGetTime @ 0x009E1BC0.
// Data: 0.5f @ 0x0122F59C, 52.0f @ 0x0123678C, 204.0f @ 0x01236884.
// Layout: retail CTCInGameMenu with 12-byte vectors (PDB offsets - 0x20 past the seven vectors): Components +0x58,
//   SaveGameIndex +0xC8, FramesToWaitToSave +0xCC, PSaveInProgress +0xD0, SaveInProgressTime +0xD8.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();                                  // 0x0099EAE0
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};

float GetScreenWidth();                              // 0x0041CC14
float GetScreenHeight();                             // 0x0041CC2B
double __fastcall GFGetTime();                       // 0x009E1BC0

namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name);  // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30();
    virtual void SetPosition(const C2DVector& position, float depth, bool b);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void Show_slot43();                       // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88();
    virtual void v89(); virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93();
    virtual void v94(); virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98();
    virtual void v99(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144();
    virtual void SetTextByTag(const CCharString& tag, bool a, bool b);   // slot 145 (+0x244)
};
struct CManager {
    CComponent* CreateComponent(const CCharString& name, bool b);   // 0x0041DB1D
    C2DVector GetUIScale();                          // 0x0041CF47
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}
using NUISystem::CComponent;
using NUISystem::CManager;

struct CQuestManager {
    bool IsBetweenQuestStartAndFinishScreens();      // 0x004AF290
};
extern CQuestManager* g_pQuestManager;               // 0x013B89FC

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    void push_back(const T& x);                      // 0x00486024
};

struct CTCInGameMenu {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x50];
    CRetailVector<CComponent*> Components;           // +0x58
    unsigned char _pad_0x64[0x64];
    long          SaveGameIndex;                     // +0xC8
    long          FramesToWaitToSave;                // +0xCC
    CComponent*   PSaveInProgress;                   // +0xD0
    unsigned char _pad_0xd4[0x4];
    double        SaveInProgressTime;                // +0xD8

    void SetToSave(long saveIndex);
};

void CTCInGameMenu::SetToSave(long saveIndex)
{
    SaveGameIndex = saveIndex;
    FramesToWaitToSave = 2;
    {
        CCharString name("PC_SAVE_IN_PROGRESS_DIALOG", -1);
        Components.push_back(CManager::GetInstance()->CreateComponent(name, false));
    }
    if (g_pQuestManager->IsBetweenQuestStartAndFinishScreens()) {
        CComponent* text = Components[Components.size() - 1]->FindChild(CCharString("PC_SAVE_IN_PROGRESS_TEXT", -1));
        if (text) {
            CCharString tag("TEXT_GUI_SAVE_HERO_IN_PROGRESS_PC", -1);
            text->SetTextByTag(tag, true, true);
        }
    }
    Components[Components.size() - 1]->Show_slot43();
    PSaveInProgress = Components[Components.size() - 1];
    C2DVector position(GetScreenWidth() * 0.5f - CManager::GetInstance()->GetUIScale().X * 204.0f,
                       GetScreenHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
    PSaveInProgress->SetPosition(position, 0.0f, false);
    SaveInProgressTime = GFGetTime();
}
