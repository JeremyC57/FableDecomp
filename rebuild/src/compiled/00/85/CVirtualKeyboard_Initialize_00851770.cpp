// CVirtualKeyboard::Initialize() -- finds the UI_NEW_PROFILE_EDIT_BOX component on the entry screen and, if it is an edit box (type 0x25), primes it with the auto-save display name, forbids '.', sets DeleteAllOnKeyPress, registers it with the front-end manager, subscribes it to events 0x21/0x22 and activates it; always returns 0
// Leaf: Initialize
// Retail: Fable.exe (TLC, VC7.1) @ 0x00851770, 234 bytes (padded to 240), plain `ret`.
//   PDB name ?Initialize@CVirtualKeyboard@@QAEJXZ (returns long).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the by-value name, built in its argument slot);
//   CComponent virtual slot 3 (+0xc, find a child by name) and slot 65 (+0x104, component type);
//   CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0 (`__fastcall`), CUserProfileManager::GetAutoSaveDisplayName
//   0x004069E0 (CWideString by value); NUISystem::CEditBox slots 143 SetText 0x00540180, 147 UpdateDisplayText
//   0x00540980, 155 SetForbiddenCharacters 0x00540D40, 150 SetActive 0x005401B0; CWideString(const wchar_t*)
//   0x0099B6B0, ~CWideString 0x0099B510; NUISystem::CManager::GetInstance 0x0041E5F2 and its slot 2 (+8,
//   takes the edit box's CObserver); CObserver slot 3 ObserveEvent(EEvent) 0x0052DA20. String "UI_NEW_PROFILE_EDIT_BOX"
//   0x012759A0, L"." 0x0124C4A8.
// Shape: the by-value CCharString argument needs a user copy ctor (built in the slot: `push ecx ; mov ecx,esp`);
//   `L"."` is an implicit conversion temporary (re-addressed with `lea`); the CObserver base sits at +4, so the
//   AddObserver argument is the null-tested `p ? p + 4 : 0` conversion and ObserveEvent adjusts `this` by +4.
// Layout (engine/CVirtualKeyboard.h): PEntryScreen +0x08, PEntryBox +0x0c; retail CEditBox DeleteAllOnKeyPress +0x17c
//   (Ego_r.pdb +0x1bc - 0x40, as Text +0x164 / CursorPos +0x168 written by SetText).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);       // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
};
class CUserProfileManager { public:
    CWideString GetAutoSaveDisplayName();          // 0x004069E0
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
};
namespace NUISystem {
class CObserver { public:                                 // secondary base, vtable slots of 0x01246B64
    virtual ~CObserver();
    virtual void ProcessEvent(long event);
    virtual bool v002(long);
    virtual void ObserveEvent(long event);         // slot 3 (+0xc) 0x0052DA20
};
class CComponent { public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* GetChildByName(CCharString name);   // slot 3 (+0xc)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013();
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064();
    virtual long GetComponentType();   // slot 65 (+0x104)
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void SetText(const CWideString& text);   // slot 143 (+0x23c)
    virtual void v144(); virtual void v145(); virtual void v146();
    virtual void UpdateDisplayText();   // slot 147 (+0x24c)
    virtual void v148(); virtual void v149();
    virtual void SetActive(bool active);   // slot 150 (+0x258)
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void SetForbiddenCharacters(const CWideString& characters);   // slot 155 (+0x26c)
};
class CEditBox : public CComponent, public CObserver { public:          // primary vtable 0x01246B8C, CObserver at +4
    unsigned char _pad08[0x17c - 0x08];
    bool DeleteAllOnKeyPress;                      // +0x17c
};
class CManager { public:
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual ~CManager();
    virtual void ProcessEvent(long event);
    virtual void AddObserver(CObserver* observer); // slot 2 (+8)
};
}
using namespace NUISystem;
struct CVirtualKeyboard {
    CWideString ProfileName;                       // +0x00
    bool        Canceled;                          // +0x04
    bool        Done;                              // +0x05
    CComponent* PEntryScreen;                      // +0x08
    CEditBox*   PEntryBox;                         // +0x0c
    long Initialize();
};
long CVirtualKeyboard::Initialize()
{
    CComponent* box = PEntryScreen->GetChildByName(CCharString("UI_NEW_PROFILE_EDIT_BOX", -1));
    if (box && box->GetComponentType() == 0x25) {
        PEntryBox = static_cast<CEditBox*>(box);
        PEntryBox->SetText(::CUserProfileManager::Get().GetAutoSaveDisplayName());
        PEntryBox->UpdateDisplayText();
        PEntryBox->SetForbiddenCharacters((const unsigned short*)L".");
        PEntryBox->DeleteAllOnKeyPress = true;
        NUISystem::CManager::GetInstance()->AddObserver(PEntryBox);
        PEntryBox->ObserveEvent(0x21);
        PEntryBox->ObserveEvent(0x22);
        PEntryBox->SetActive(true);
    }
    return 0;
}