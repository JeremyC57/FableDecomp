// CTCTrophy::StartDialogBox(CWideString text, unsigned long flags, CWideString button1, CWideString button2, CWideString button3) -- unless a hero-information screen is up, shows the trophy dialog through the player GUI's subtitle renderer (three-button or two-button form), then waits for confirmation and registers the trophy for global updates once
// Leaf: StartDialogBox
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DD7E0, 257 bytes (`ret 14h`; Ego_r.pdb
// `?StartDialogBox@CTCTrophy@@AAEXVCWideString@@K000@Z`, the four strings by value, destroyed by the callee).
// Callees: CPlayerGui::GetPHeroInfoScreensManager() 0x00437CD0; CWideString::operator!=(const wchar_t*) const 0x0099B150
//          (L"" 0x0122D70C); CWideString(const wchar_t*) 0x0099B6B0; CSubtitleRenderer::SetText(CFontBank*, five const
//          CWideString&, unsigned long, float, bool, bool) 0x00492973 and the two-button overload 0x00492F69;
//          ~CWideString 0x0099B510; CTCBase::AddToGlobalTCs() 0x00686ED0.
// Globals: (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790 (subtitle renderer at +0x18), CMainGameComponent::Get() 0x013B86A0 (font +0x1614C).
// Layout (retail, lists 4 bytes): CTCTrophy WaitingForConfirmation +0x3c (PDB +0x4c), IsGlobalTC +0x40 (PDB +0x50);
//          NHeroInformationScreens::CManager +0x34 an active-screen flag.
// Shape: the empty name is an implicit L"" conversion temporary built before the pushes (its literal push hoisted
//        above the branch, one frame word shared by both arms); the early return still destroys the by-value strings.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t
class CWideString { public:
    void* m_data;
    CWideString(const wchar_t* text);           // 0x0099B6B0
    ~CWideString();                             // 0x0099B510
    bool operator!=(const wchar_t* text) const; // 0x0099B150
};
class CFontBank;
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, const CWideString& button3, unsigned long flags, float time, bool b1,
                 bool b2);                      // 0x00492973
    void SetText(CFontBank* font, const CWideString& text, const CWideString& name, const CWideString& button1,
                 const CWideString& button2, unsigned long flags, float time, bool b1, bool b2);   // 0x00492F69
};
namespace NHeroInformationScreens {
class CManager { public:
    unsigned char _pad_0x00[0x34];
    bool          ScreenActive;                 // +0x34
};
}
class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char     _pad_0x00[0x18];
    CSubtitleRenderer SubtitleRenderer;         // +0x18
public:
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad_0x00[0x1614c];
    CFontBank*    Font;                         // +0x1614C
};

class CTCBase { public:
    virtual void v00();
    void*         ParentThing;                  // +0x04
    bool          ToBeRemoved;                  // +0x08
    bool          IsInGlobalUpdateTCs;          // +0x09
    unsigned char _pad0a[2];
public:
    void AddToGlobalTCs();   // 0x00686ED0
};
class CTCTrophy : public CTCBase { public:
    unsigned char _pad_0x0c[0x30];
    bool          WaitingForConfirmation;       // +0x3c
    bool          WaitingForScore;              // +0x3d
    bool          WaitingForTerminate;          // +0x3e
    bool          WaitingForNextInstructionsPage;   // +0x3f
    bool          IsGlobalTC;                   // +0x40
    void StartDialogBox(CWideString text, unsigned long flags, CWideString button1, CWideString button2, CWideString button3);
};

void CTCTrophy::StartDialogBox(CWideString text, unsigned long flags, CWideString button1, CWideString button2, CWideString button3)
{
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->ScreenActive)
        return;
    if (button3 != L"")
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.SetText(CMainGameComponent::Get()->Font, text, L"", button1, button2, button3, flags, 0.0f, true, true);
    else
        CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().SubtitleRenderer.SetText(CMainGameComponent::Get()->Font, text, L"", button1, button2, flags, 0.0f, true, true);
    WaitingForConfirmation = true;
    if (!IsGlobalTC) {
        AddToGlobalTCs();
        IsGlobalTC = true;
    }
}