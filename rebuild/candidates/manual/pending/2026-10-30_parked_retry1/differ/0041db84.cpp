// NUISystem::CManager::ShowErrorMessage(long) -- show the controller-disconnected dialog (front-end or in-game variant) with the pad number, then flag the message as shown
// Grade: IMPROVED(276v281) -- retry1: with GetTextBySymbol 0x0041CFB2 as a same-TU static __stdcall (body: main-game text bank, then the front-end one, each skipped when the text equals g_emptyWideString, else CWideString::CreateFromCharString) VC7.1 emits retail's call exactly (`lea edi,[ebp+8] ; call`, callee-popped symbol). Left: VC7.1 packs the three CCharString temporaries into one frame word [ebp-4]; retail gives each its own (-4/-8/-0xc, frame 0xc).
// Leaf: ShowErrorMessage
// Lane gap: retail calls GetTextBySymbol 0x0041CFB2 with a link-time-generated convention (the CWideString result pointer in
//   edi, `mov eax,edi ; ret 4`), which a single-TU VC7.1 compile cannot emit; this source uses the standard hidden pointer.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator!=(const CWideString& other) const;    // 0x0099B0F0
    static CWideString __fastcall CreateFromCharString(const CCharString& text);   // 0x0099B6A0
};
extern const CWideString g_emptyWideString;             // 0x013BCA24
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
template <class T> struct CCountedPointer { T* Object; T* operator->() const { return Object; } };
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;    // +0x14 (Ego_r CMainGameComponent::PTextBank)
};
struct CFrontEndGameComponent {
    unsigned char _pad00[0x60];
    CCountedPointer<NGameText::CDataBank> PTextBank;    // +0x60
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
extern CFrontEndGameComponent* g_pFrontEndGame;         // 0x013B871C

// 0x0041CFB2: a same-TU static (retail's private convention: the result pointer in edi, `mov eax,edi ; ret 4`).
static __declspec(noinline) CWideString __stdcall GetTextBySymbol(const CCharString& symbol)
{
    if (g_pMainGameComponent) {
        CWideString text = g_pMainGameComponent->PTextBank->GetTextBySymbol(symbol);
        if (text != g_emptyWideString)
            return text;
    }
    if (g_pFrontEndGame) {
        CWideString text = g_pFrontEndGame->PTextBank->GetTextBySymbol(symbol);
        if (text != g_emptyWideString)
            return text;
    }
    return CWideString::CreateFromCharString(symbol);
}
bool __fastcall ConstructString(CWideString* text, long number);   // 0x009C7B70
struct CGameJoystickManager {
    static CGameJoystickManager* __fastcall Get();      // 0x0049BB90
    long GetActivePadIndex();                           // 0x0049BB40
};
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* GetChildByName(CCharString name);   // slot 3 (+0xc)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void Show();   // slot 43 (+0xac)
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void SetModal(bool modal);   // slot 48 (+0xc0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void SetText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
};
class CManager {
public:
    unsigned char _pad00[0x9c];
    bool          ErrorMessageShown;                    // +0x9c
    CComponent*   PErrorDialog;                         // +0xa0
    CComponent* CreateComponent(const CCharString& name, bool show);   // 0x0041DB1D
    void ShowErrorMessage(long error);
};

void CManager::ShowErrorMessage(long error)
{
    if (error == 0) {
        CComponent* text;
        if (g_pFrontEndGame) {
            PErrorDialog = CreateComponent("UI_DIALOG_CONTROLLERDISCONNECT", false);
            text = PErrorDialog->GetChildByName("UI_CONTROLLERDISCONNECT_TEXT");
        } else {
            PErrorDialog = CreateComponent("UI_DIALOG_CONTROLLERDISCONNECT_GAME", false);
            text = PErrorDialog->GetChildByName("UI_CONTROLLERDISCONNECT_TEXT_GAME");
        }
        CWideString message = GetTextBySymbol("TEXT_GUI_CONTROLLER_DISCONNECT");
        ConstructString(&message, CGameJoystickManager::Get()->GetActivePadIndex() + 1);
        text->SetText(message, true, true);
        PErrorDialog->Show();
        PErrorDialog->SetModal(true);
    }
    ErrorMessageShown = true;
}
}
