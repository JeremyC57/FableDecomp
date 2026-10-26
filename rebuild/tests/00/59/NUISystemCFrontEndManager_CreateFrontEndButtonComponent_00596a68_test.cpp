#include <cstdio>
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CWideString {
public:
    void* PStringData;
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(T* data);                           // 0x00429C15 for CComponent
    template <class U> __forceinline CCountedPointer(const CCountedPointer<U>& other) : Data(0), Info(0)
    {
        ShareData((T*)other.Data, other.Info);
    }
    ~CCountedPointer();                                 // 0x004291DE (CComponent) / 0x0042925C (CText)
    void Reset(T* data);                                // 0x00429C6F for CComponent
    void ShareData(T* data, CCPPointerInfo* info);      // 0x00429460 for CText
    T* operator->() const { return Data; }
};
namespace std {
template <class K, class V> class map {
public:
    void* _body[3];
    V& operator[](const K& key);                        // 0x0042D5B1 for <unsigned long, CUIState>
};
}
namespace NUISystem {
struct C2DVector { float X, Y; };
class CUIState { public:
    unsigned long StateChangeFlag;                      // +0x00
    long          StateChangeType;                      // +0x04
    C2DVector     Position;                             // +0x08
    C2DVector     Zoom;                                 // +0x10
    unsigned long Colour;                               // +0x18
    float         UpdateTime;                           // +0x1c
    bool          LinearChange;                         // +0x20
};
class CComponent;
typedef std::map<unsigned long, CUIState> CUIStateMap;
class CComponent {
public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004();
    virtual void _v005(); virtual void _v006(); virtual void _v007(); virtual void _v008(); virtual void _v009();
    virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014();
    virtual void _v015(); virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019();
    virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023(); virtual void _v024();
    virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029();
    virtual void _v030(); virtual void _v031(); virtual void _v032(); virtual void _v033(); virtual void _v034();
    virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044();
    virtual void _v045(); virtual void _v046(); virtual void _v047(); virtual void _v048(); virtual void _v049();
    virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054();
    virtual void _v055(); virtual void _v056(); virtual void _v057(); virtual void _v058();
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 59 (+0xec)
    virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063(); virtual void _v064();
    virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069();
    virtual void _v070(); virtual void _v071(); virtual void _v072(); virtual void _v073(); virtual void _v074();
    virtual void _v075(); virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079();
    virtual void _v080(); virtual void _v081();
    virtual CUIStateMap& GetStates();                   // slot 82 (+0x148)
};
class CText : public CComponent {
public:
    virtual void _v083(); virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087();
    virtual void _v088(); virtual void _v089(); virtual void _v090(); virtual void _v091(); virtual void _v092();
    virtual void _v093(); virtual void _v094(); virtual void _v095(); virtual void _v096(); virtual void _v097();
    virtual void _v098(); virtual void _v099(); virtual void _v100(); virtual void _v101(); virtual void _v102();
    virtual void _v103(); virtual void _v104(); virtual void _v105(); virtual void _v106(); virtual void _v107();
    virtual void _v108(); virtual void _v109(); virtual void _v110(); virtual void _v111(); virtual void _v112();
    virtual void _v113(); virtual void _v114(); virtual void _v115(); virtual void _v116(); virtual void _v117();
    virtual void _v118(); virtual void _v119(); virtual void _v120(); virtual void _v121(); virtual void _v122();
    virtual void _v123(); virtual void _v124(); virtual void _v125(); virtual void _v126(); virtual void _v127();
    virtual void _v128(); virtual void _v129(); virtual void _v130(); virtual void _v131(); virtual void _v132();
    virtual void _v133(); virtual void _v134(); virtual void _v135(); virtual void _v136(); virtual void _v137();
    virtual void _v138(); virtual void _v139(); virtual void _v140(); virtual void _v141(); virtual void _v142();
    virtual void _v143();
    virtual void SetValue(const CWideString& text, bool flag, bool update);   // slot 144 (+0x240)
};
class CFrontEndManager {
public:
    CComponent* CreateComponent(const CCharString& definition);   // 0x00594F94
    CComponent* CreateFrontEndButtonComponent(CWideString& text, float y, const CCharString& frameDef,
                                              const CCharString& buttonDef, bool flag);
};
CComponent* CFrontEndManager::CreateFrontEndButtonComponent(CWideString& text, float y, const CCharString& frameDef,
                                                            const CCharString& buttonDef, bool flag)
{
    CCountedPointer<CComponent> button;
    if (buttonDef == "")
        button.Reset(CreateComponent("UI_BUTTON"));
    else
        button.Reset(CreateComponent(buttonDef));
    CCountedPointer<CComponent> textComponent(CreateComponent("UI_BUTTON_TEXT"));
    CCountedPointer<CText> textPointer(textComponent);
    textPointer->SetValue(text, flag, true);
    CCountedPointer<CComponent> textButton(CreateComponent("UI_TEXT_BUTTON"));
    textButton->AddChild(textComponent);
    textButton->AddChild(button);
    CComponent* frame;
    if (frameDef == "")
        frame = CreateComponent("UI_FRONTEND_BUTTON");
    else
        frame = CreateComponent(frameDef);
    frame->AddChild(textButton);
    frame->GetStates()[0].Position.Y = y;
    frame->GetStates()[1].Position.Y = y;
    frame->GetStates()[3].Position.Y = y;
    frame->GetStates()[4].Position.Y = y;
    frame->GetStates()[5].Position.Y = y;
    return frame;
}
}

#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
void NUISystem::CComponent::_v000() {}
void NUISystem::CComponent::_v001() {}
void NUISystem::CComponent::_v002() {}
void NUISystem::CComponent::_v003() {}
void NUISystem::CComponent::_v004() {}
void NUISystem::CComponent::_v005() {}
void NUISystem::CComponent::_v006() {}
void NUISystem::CComponent::_v007() {}
void NUISystem::CComponent::_v008() {}
void NUISystem::CComponent::_v009() {}
void NUISystem::CComponent::_v010() {}
void NUISystem::CComponent::_v011() {}
void NUISystem::CComponent::_v012() {}
void NUISystem::CComponent::_v013() {}
void NUISystem::CComponent::_v014() {}
void NUISystem::CComponent::_v015() {}
void NUISystem::CComponent::_v016() {}
void NUISystem::CComponent::_v017() {}
void NUISystem::CComponent::_v018() {}
void NUISystem::CComponent::_v019() {}
void NUISystem::CComponent::_v020() {}
void NUISystem::CComponent::_v021() {}
void NUISystem::CComponent::_v022() {}
void NUISystem::CComponent::_v023() {}
void NUISystem::CComponent::_v024() {}
void NUISystem::CComponent::_v025() {}
void NUISystem::CComponent::_v026() {}
void NUISystem::CComponent::_v027() {}
void NUISystem::CComponent::_v028() {}
void NUISystem::CComponent::_v029() {}
void NUISystem::CComponent::_v030() {}
void NUISystem::CComponent::_v031() {}
void NUISystem::CComponent::_v032() {}
void NUISystem::CComponent::_v033() {}
void NUISystem::CComponent::_v034() {}
void NUISystem::CComponent::_v035() {}
void NUISystem::CComponent::_v036() {}
void NUISystem::CComponent::_v037() {}
void NUISystem::CComponent::_v038() {}
void NUISystem::CComponent::_v039() {}
void NUISystem::CComponent::_v040() {}
void NUISystem::CComponent::_v041() {}
void NUISystem::CComponent::_v042() {}
void NUISystem::CComponent::_v043() {}
void NUISystem::CComponent::_v044() {}
void NUISystem::CComponent::_v045() {}
void NUISystem::CComponent::_v046() {}
void NUISystem::CComponent::_v047() {}
void NUISystem::CComponent::_v048() {}
void NUISystem::CComponent::_v049() {}
void NUISystem::CComponent::_v050() {}
void NUISystem::CComponent::_v051() {}
void NUISystem::CComponent::_v052() {}
void NUISystem::CComponent::_v053() {}
void NUISystem::CComponent::_v054() {}
void NUISystem::CComponent::_v055() {}
void NUISystem::CComponent::_v056() {}
void NUISystem::CComponent::_v057() {}
void NUISystem::CComponent::_v058() {}
void NUISystem::CComponent::_v060() {}
void NUISystem::CComponent::_v061() {}
void NUISystem::CComponent::_v062() {}
void NUISystem::CComponent::_v063() {}
void NUISystem::CComponent::_v064() {}
void NUISystem::CComponent::_v065() {}
void NUISystem::CComponent::_v066() {}
void NUISystem::CComponent::_v067() {}
void NUISystem::CComponent::_v068() {}
void NUISystem::CComponent::_v069() {}
void NUISystem::CComponent::_v070() {}
void NUISystem::CComponent::_v071() {}
void NUISystem::CComponent::_v072() {}
void NUISystem::CComponent::_v073() {}
void NUISystem::CComponent::_v074() {}
void NUISystem::CComponent::_v075() {}
void NUISystem::CComponent::_v076() {}
void NUISystem::CComponent::_v077() {}
void NUISystem::CComponent::_v078() {}
void NUISystem::CComponent::_v079() {}
void NUISystem::CComponent::_v080() {}
void NUISystem::CComponent::_v081() {}
void NUISystem::CText::_v083() {}
void NUISystem::CText::_v084() {}
void NUISystem::CText::_v085() {}
void NUISystem::CText::_v086() {}
void NUISystem::CText::_v087() {}
void NUISystem::CText::_v088() {}
void NUISystem::CText::_v089() {}
void NUISystem::CText::_v090() {}
void NUISystem::CText::_v091() {}
void NUISystem::CText::_v092() {}
void NUISystem::CText::_v093() {}
void NUISystem::CText::_v094() {}
void NUISystem::CText::_v095() {}
void NUISystem::CText::_v096() {}
void NUISystem::CText::_v097() {}
void NUISystem::CText::_v098() {}
void NUISystem::CText::_v099() {}
void NUISystem::CText::_v100() {}
void NUISystem::CText::_v101() {}
void NUISystem::CText::_v102() {}
void NUISystem::CText::_v103() {}
void NUISystem::CText::_v104() {}
void NUISystem::CText::_v105() {}
void NUISystem::CText::_v106() {}
void NUISystem::CText::_v107() {}
void NUISystem::CText::_v108() {}
void NUISystem::CText::_v109() {}
void NUISystem::CText::_v110() {}
void NUISystem::CText::_v111() {}
void NUISystem::CText::_v112() {}
void NUISystem::CText::_v113() {}
void NUISystem::CText::_v114() {}
void NUISystem::CText::_v115() {}
void NUISystem::CText::_v116() {}
void NUISystem::CText::_v117() {}
void NUISystem::CText::_v118() {}
void NUISystem::CText::_v119() {}
void NUISystem::CText::_v120() {}
void NUISystem::CText::_v121() {}
void NUISystem::CText::_v122() {}
void NUISystem::CText::_v123() {}
void NUISystem::CText::_v124() {}
void NUISystem::CText::_v125() {}
void NUISystem::CText::_v126() {}
void NUISystem::CText::_v127() {}
void NUISystem::CText::_v128() {}
void NUISystem::CText::_v129() {}
void NUISystem::CText::_v130() {}
void NUISystem::CText::_v131() {}
void NUISystem::CText::_v132() {}
void NUISystem::CText::_v133() {}
void NUISystem::CText::_v134() {}
void NUISystem::CText::_v135() {}
void NUISystem::CText::_v136() {}
void NUISystem::CText::_v137() {}
void NUISystem::CText::_v138() {}
void NUISystem::CText::_v139() {}
void NUISystem::CText::_v140() {}
void NUISystem::CText::_v141() {}
void NUISystem::CText::_v142() {}
void NUISystem::CText::_v143() {}
using namespace NUISystem;
static int g_baseCalls;
void CComponent::AddChild(CCountedPointer<CComponent>&) { ++g_baseCalls; }
CUIStateMap& CComponent::GetStates() { ++g_baseCalls; static CUIStateMap m; return m; }
void CText::SetValue(const CWideString&, bool, bool) { ++g_baseCalls; }
static int g_strBase;
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
static int g_strLive;
CCharString::CCharString(const char* text, long length)
{
    check(length == -1, "length -1");
    PStringData = new CCharStringData; PStringData->Data.PData = (char*)text; ++g_strLive;
}
CCharString::~CCharString() { delete PStringData; --g_strLive; }
static CUIState g_states[8][8]; static CUIStateMap g_maps[8];
struct TText : CText {
    int id;
    void AddChild(CCountedPointer<CComponent>& child);
    CUIStateMap& GetStates() { return g_maps[id]; }
    void SetValue(const CWideString& text, bool flag, bool update);
};
static TText g_objs[8]; static int g_made; static const char* g_defs[8];
CComponent* CFrontEndManager::CreateComponent(const CCharString& def)
{
    g_defs[g_made] = def.PStringData->Data.PData; g_objs[g_made].id = g_made; return &g_objs[g_made++];
}
static int g_add[8][2]; static int g_adds;
void TText::AddChild(CCountedPointer<CComponent>& child) { g_add[g_adds][0] = id; g_add[g_adds][1] = static_cast<TText*>(child.Data)->id; ++g_adds; }
static int g_setId; static const CWideString* g_setText; static bool g_setFlag, g_setUpdate;
void TText::SetValue(const CWideString& text, bool flag, bool update) { g_setId = id; g_setText = &text; g_setFlag = flag; g_setUpdate = update; }
template <> CUIState& std::map<unsigned long, CUIState>::operator[](const unsigned long& key) { return g_states[this - g_maps][key]; }
static int g_cpLive;
template <> CCountedPointer<CComponent>::CCountedPointer(CComponent* data) : Data(data), Info(0) { ++g_cpLive; }
template <> CCountedPointer<CComponent>::~CCountedPointer() { --g_cpLive; }
template <> CCountedPointer<CText>::~CCountedPointer() { --g_cpLive; }
template <> void CCountedPointer<CComponent>::Reset(CComponent* data) { Data = data; ++g_cpLive; }
template <> void CCountedPointer<CText>::ShareData(CText* data, CCPPointerInfo* info) { Data = data; Info = info; ++g_cpLive; }
static void run(const CCharString& frameDef, const CCharString& buttonDef, const char* frameName, const char* buttonName, bool flag)
{
    static CFrontEndManager fem; CWideString text; text.PStringData = 0;
    g_made = 0; g_adds = 0; memset(g_states, 0, sizeof g_states);
    CComponent* frame = fem.CreateFrontEndButtonComponent(text, 2.5f, frameDef, buttonDef, flag);
    check(g_made == 4, "four components");
    check(strcmp(g_defs[0], buttonName) == 0 && strcmp(g_defs[1], "UI_BUTTON_TEXT") == 0 && strcmp(g_defs[2], "UI_TEXT_BUTTON") == 0
          && strcmp(g_defs[3], frameName) == 0, "definitions in order");
    check(frame == &g_objs[3], "frame returned");
    check(g_setId == 1 && g_setText == &text && g_setFlag == flag && g_setUpdate, "text set");
    check(g_adds == 3 && g_add[0][0] == 2 && g_add[0][1] == 1 && g_add[1][0] == 2 && g_add[1][1] == 0 && g_add[2][0] == 3 && g_add[2][1] == 2, "children");
    static const int keys[5] = { 0, 1, 3, 4, 5 };
    for (int i = 0; i < 5; ++i) check(g_states[3][keys[i]].Position.Y == 2.5f, "state y");
    check(g_states[3][2].Position.Y == 0.0f && g_states[3][6].Position.Y == 0.0f, "other states untouched");
    check(g_strLive == g_strBase && g_cpLive == 0 && g_baseCalls == 0, "temporaries released");
}
int main()
{
    CCharString nullName("x"); delete nullName.PStringData; nullName.PStringData = 0;
    CCharString emptyName(""); CCharString myFrame("MY_FRAME"); CCharString myButton("MY_BUTTON");
    g_strBase = g_strLive;
    run(nullName, nullName, "UI_FRONTEND_BUTTON", "UI_BUTTON", true);
    run(myFrame, myButton, "MY_FRAME", "MY_BUTTON", false);
    run(emptyName, myButton, "UI_FRONTEND_BUTTON", "MY_BUTTON", true);
    nullName.PStringData = new CCharStringData;
    if (!g_fail) std::printf("00596a68_TEST PASS\n");
    return g_fail;
}