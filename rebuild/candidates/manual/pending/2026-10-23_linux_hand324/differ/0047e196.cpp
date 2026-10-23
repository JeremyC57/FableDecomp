// CTCInGameMenu::SaveGame(long index) -- manual save into slot `index`: pick the file name (the slot's existing name, or a new "Manual - " name from the profile manager), refuse a new slot when the disk is full, otherwise resolve the path and save the world state; wait out the save-in-progress time, drop the in-progress dialog, post message 0x3E on success, then show PC_DIALOG_GAME_SAVED_GAME (confirm or failed text, OK button action 0xE7 with a long parameter 4), register it and the mouse pointer for input and centre it on the screen
// Grade: DIFFER(1211v1211) -- shape, callees and size as retail; residue is frame-slot sharing only: the first SetText key temporary gets its own slot (-10h) instead of sharing the CreateComponent name slot (-8), which pushes the dialog copy, the message origin, the action pointer and the dialog position 8 bytes down (tried: named/condition-declared text pointers, blocks around the dialog part or the rest, conversion-temp push_back)
// Leaf: ?SaveGame@CTCInGameMenu
//
// Retail 0x0047E196, 1211 bytes (`ret 4`; frame pointer, `leave`). Size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: CWideString() 0x0099AED0, CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510, CWideString::operator!=
//   (const wchar_t*) 0x0099B150, operator+(const wchar_t*, const CWideString&) 0x0099BFF0 (__fastcall, hidden result in ecx),
//   CDateAndTime bool query 0x009E9C60, CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, the profile manager's bool query
//   0x00407030, GetManualSaveFileName 0x00406610, GetPathNameFromFileName 0x00408120, the slot-name setter 0x0040BC50,
//   CWorld::IsDiskSpaceForSaveGame 0x0049D4B0 / SaveGameStateManual 0x004A0780 (world at engine +0x24, 0x013B86A0), GFGetTime
//   0x009E1BC0, vector<CComponent*>::erase 0x00485468 / push_back 0x00486024, CTCBase::GetWorld 0x00686A70,
//   CMessageEventManager::AddMessage 0x00687540, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CManager::GetInstance
//   0x0041E5F2 / CreateComponent 0x0041DB1D / GetUIScale 0x0041CF47, CQuestManager query 0x004AF290 (0x013B89FC), CAction()
//   0x0042BE50, CCountedPointer<CAction> 0x0042AA29 / ~ 0x00429160, param-list push_back 0x0042B646, screen width / height
//   0x0041CC14 / 0x0041CC2B; the dialog callback 0x0047DB73.
// Layout: retail CTCInGameMenu (12-byte containers, landed ctor 0x0047DF8A): FileNames +0x28, FileTimes +0x34 (0x1C each),
//   Components +0x58, m_pOKDialog +0xac, PSaveInProgress +0xd0, SaveInProgressTime +0xd8, PMousePointer +0xec.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const CWideString& other);
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator!=(const wchar_t* text) const;         // 0x0099B150
};
CWideString __fastcall operator+(const wchar_t* text, const CWideString& s);   // 0x0099BFF0
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CDateAndTime {
    unsigned char _body[0x1c];
    bool IsSet() const;                                 // 0x009E9C60
};
double __fastcall GFGetTime();                          // 0x009E1BC0
float GetScreenWidth();                                 // 0x0041CC14
float GetScreenHeight();                                // 0x0041CC2B
extern long g_framesPerSecond;                          // 0x01375550

namespace NUISystem {
struct CActionParam {
    virtual void v00();
    long Type;                                          // +0x04
    CActionParam(long type) : Type(type) {}
};
struct CActionParamLong : CActionParam {                // vtable 0x012366F4
    long Value;                                         // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
};
struct CActionParamList {
    void* _M_node;
    void push_back(CActionParam* const& value);         // 0x0042B646
};
}
template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x0042AA29
    ~CCountedPointer();                                 // 0x00429160
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CAction {
    long             ActionType;
    CActionParamList UIActionParams;
    void*            ThingActionParams;
    bool             Valid;
    CAction();                                          // 0x0042BE50
};
struct CObserver {
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);          // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void SetFocus_slot6();                      // slot 6 (+0x18)
};
typedef void (*CDialogCallback)();
struct CComponentBase {
    virtual ~CComponentBase();                          // slot 0
    virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30();
    virtual void SetPosition(const C2DVector& position, float z, long flags);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void Reset_slot43();                        // slot 43 (+0xAC)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetActive_slot48(bool active);         // slot 48 (+0xC0)
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68();
    virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73();
    virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79();
    virtual void AddAction(const CCountedPointer<CAction>& action);   // slot 80 (+0x140)
    virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85();
    virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89(); virtual void v90();
    virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94(); virtual void v95();
    virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141();
    virtual void SetYesCallback(CDialogCallback callback);   // slot 142 (+0x238)
    virtual void SetNoCallback(CDialogCallback callback);    // slot 143 (+0x23C)
    virtual void v144();
    virtual void SetText(const CCharString& text, bool a, bool b);   // slot 145 (+0x244)
};
struct CComponent : CComponentBase, CObserver {};
struct CYesNo : CComponent {};
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void AddFocus_slot5(CObserver* observer);   // slot 5 (+0x14)
    virtual void m06(); virtual void m07(); virtual void m08();
    virtual void Notify_slot9(CObserver* observer);     // slot 9 (+0x24)
    CComponent* CreateComponent(const CCharString& name, bool a);   // 0x0041DB1D
    C2DVector GetUIScale();                             // 0x0041CF47
    static CManager* __fastcall GetInstance();          // 0x0041E5F2
};
}
void SavedGameDialogCallback();                         // 0x0047DB73

struct CUserProfileManager {
    bool IsQuery_00407030();                            // 0x00407030
    CWideString GetManualSaveFileName(long index);      // 0x00406610
    CWideString GetPathNameFromFileName(const CWideString& name, bool full);   // 0x00408120
    void SetSlotName_0040bc50(long slot, const CWideString& name);   // 0x0040BC50
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
};
struct CWorld {
    bool IsDiskSpaceForSaveGame();                      // 0x0049D4B0
    void SaveGameStateManual(const CWideString& name);  // 0x004A0780
};
struct CMainGameComponent {
    unsigned char _pad00[0x24];
    CWorld*       World;                                // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CQuestManager {
    bool IsBetweenQuestStartAndFinishScreens();         // 0x004AF290
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
struct CThing;
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                     float value1, float value2, long data);   // 0x00687540
};
struct CGameWorld {
    unsigned char         _pad00[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};

template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned long i) { return *(begin() + i); }
    T* end() { return _M_finish; }
};
struct CComponentVector : CRetailVector<NUISystem::CComponent*> {
    NUISystem::CComponent** erase(NUISystem::CComponent** it);        // 0x00485468
    void push_back(NUISystem::CComponent* const& value);              // 0x00486024
};

struct CTCBase {
    virtual void v00();
    struct CThing* ParentThing;                         // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CGameWorld* GetWorld();                             // 0x00686A70
};
class CTCInGameMenu : public CTCBase {
public:
    unsigned char                 _pad0c[0x28 - 0x0c];
    CRetailVector<CWideString>    FileNames;            // +0x28
    CRetailVector<CDateAndTime>   FileTimes;            // +0x34
    unsigned char                 _pad40[0x58 - 0x40];
    CComponentVector              Components;           // +0x58
    unsigned char                 _pad64[0xac - 0x64];
    NUISystem::CYesNo*            m_pOKDialog;          // +0xac
    unsigned char                 _padb0[0xd0 - 0xb0];
    NUISystem::CComponent*        PSaveInProgress;      // +0xd0
    unsigned char                 _padd4[0xd8 - 0xd4];
    double                        SaveInProgressTime;   // +0xd8
    unsigned char                 _pade0[0xec - 0xe0];
    NUISystem::CComponent*        PMousePointer;        // +0xec
    void SaveGame(long index);
};

using namespace NUISystem;

void CTCInGameMenu::SaveGame(long index)
{
    CWideString name;
    if (FileTimes[index].IsSet())
        name = FileNames[index];
    else if (CUserProfileManager::Get().IsQuery_00407030())
        name = CUserProfileManager::Get().GetManualSaveFileName(FileNames.size());
    else {
        name = CUserProfileManager::Get().GetManualSaveFileName(index);
        name = L"Manual - " + name;
    }
    bool failed = false;
    if ((unsigned long)index < FileNames.size() && !g_pMainGameComponent->World->IsDiskSpaceForSaveGame())
        failed = true;
    else {
        CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(name, true);
        if (path != L"") {
            if (!CUserProfileManager::Get().IsQuery_00407030()) {
                if (index <= 0)
                    return;
                CUserProfileManager::Get().SetSlotName_0040bc50(index - 1, name);
            }
            g_pMainGameComponent->World->SaveGameStateManual(name);
        }
    }
    while (GFGetTime() - SaveInProgressTime < 3.0) {
    }
    for (CComponent** it = Components.begin(); it != Components.end(); ++it) {
        if (*it == PSaveInProgress) {
            delete PSaveInProgress;
            Components.erase(it);
            break;
        }
    }
    if (!failed) {
        C3DVector origin(0.0f, 0.0f, 0.0f);
        CGameWorld* world = GetWorld();
        world->MessageEventManager->AddMessage(0x3e, g_framesPerSecond, origin, 0, 0, 0.0f, 0.0f, 0);
    }
    {
        CYesNo* dialog = (CYesNo*)CManager::GetInstance()->CreateComponent("PC_DIALOG_GAME_SAVED_GAME", false);
        CComponent* component = dialog;
        if (g_pQuestManager->IsBetweenQuestStartAndFinishScreens()) {
            CComponent* text = dialog->FindChild("PC_GAME_SAVED_GAME_TEXT");
            if (text)
                text->SetText("TEXT_GUI_CONFIRM_HERO_SAVED", true, true);
        }
        if (failed) {
            CComponent* text = dialog->FindChild("PC_GAME_SAVED_GAME_TEXT");
            if (text)
                text->SetText("TEXT_GUI_SAVE_FAILED", true, true);
        }
        dialog->Reset_slot43();
        Components.push_back(component);
        m_pOKDialog = dialog;
        dialog->SetYesCallback(SavedGameDialogCallback);
        m_pOKDialog->SetNoCallback(SavedGameDialogCallback);
        m_pOKDialog->SetActive_slot48(true);
    }
    CComponent* ok = m_pOKDialog->FindChild("PC_OK_GAME_OKBUTTON");
    if (ok) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = 0xe7;
        action->UIActionParams.push_back(new CActionParamLong(4));
        ok->AddAction(action);
        CManager::GetInstance()->Notify_slot9(ok);
        ok->ObserveEvent(0x19);
    }
    CManager::GetInstance()->AddFocus_slot5(m_pOKDialog);
    CManager::GetInstance()->Notify_slot9(m_pOKDialog);
    CManager::GetInstance()->AddFocus_slot5(PMousePointer);
    CManager::GetInstance()->Notify_slot9(PMousePointer);
    PMousePointer->ObserveEvent(0x19);
    m_pOKDialog->SetFocus_slot6();
    m_pOKDialog->SetPosition(C2DVector(GetScreenWidth() * 0.5f - CManager::GetInstance()->GetUIScale().X * 204.0f,
                                       GetScreenHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f),
                             0.0f, 0);
}
