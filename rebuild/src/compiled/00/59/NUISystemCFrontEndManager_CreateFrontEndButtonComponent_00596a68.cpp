// NUISystem::CFrontEndManager::CreateFrontEndButtonComponent(CWideString& text, float y, const CCharString& frameDef, const CCharString& buttonDef, bool flag) -- build a front-end list button: a button (buttonDef or "UI_BUTTON") and a "UI_BUTTON_TEXT" text showing text inside a "UI_TEXT_BUTTON", all in a frame (frameDef or "UI_FRONTEND_BUTTON") whose states 0, 1, 3, 4 and 5 are moved to height y
// Leaf: ?CreateFrontEndButtonComponent@CFrontEndManager@NUISystem@@QAEPAVCComponent@2@AAVCWideString@@MABVCCharString@@1_N@Z
// Retail: Fable.exe @ 0x00596A68, 614 bytes, `ret 14h`; Ego_r.pdb symbol of the same name.
// Body: CreateComponent(const CCharString&) (0x00594F94); CCountedPointer<CComponent> Reset 0x00429C6F / CCountedPointer(T*)
// 0x00429C15 / dtor 0x004291DE; CCountedPointer<CText> conversion through ShareData 0x00429460, dtor 0x0042925C;
// CText::SetValue(const CWideString&, bool, bool) (slot 144, +0x240); CComponent::AddChild(CCountedPointer<CComponent>&)
// (slot 59, +0xec); CComponent::GetStates() (slot 82, +0x148) -> map<unsigned long, CUIState>::operator[] (0x0042D5B1)
// -> Position.Y (+0xc) = y; the empty-name tests are the inline CCharString::operator==("") (null block: strcmp with
// "" -> `repe cmpsb`; else CBasicString<char>::operator== 0x004115A0); CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0.
// Names: Ego_r.pdb (CreateComponent, SetValue, AddChild, GetStates, CUIState::Position).
// Layout: CCountedPointer {Data +0, Info +4}; CCharString data block = CBasicString<char> (as the matched 0x008A86C0).
// Shape: the default-name temporaries and the state keys live in the dead reference-parameter slots.
// Opt: size TU with a frame pointer.
#pragma optimize("s", on)
#pragma optimize("y", off)
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