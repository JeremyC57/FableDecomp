// NUISystem::CFrontEndScreen::Initialise() -- base initialise, stamp the last-input time, find the UI_ACCEPT button and hand it to the child walk
// Leaf: ?Initialise@CFrontEndScreen@NUISystem@@UAEXXZ
// Retail: Fable.exe @ 0x0054E4B0, 64 bytes, `ret`; referenced only from the CFrontEndScreen vtable (0x01249890); Ego_r.pdb
// ?Initialise@CFrontEndScreen@NUISystem@@UAEXXZ.
// Body: CChangingStateComponent::Initialise() (0x0052C730); m_fLastInputTime (+0x15c) = Time (+0x30); PAcceptButton (+0x168)
// = FindChild(CCharString("UI_ACCEPT")) (vtable +0x0c; Ego_r.pdb ?FindChild@CComponent@NUISystem@@UAEPAV12@VCCharString@@@Z,
// the string built in its by-value argument slot, ctor 0x0099EBF0); if found, tail-call 0x0054E350 (this in ecx, the button
// in edx: a __fastcall helper that recurses through the child list -- its name is not recovered).
// Layout: retail CComponent Time +0x30 (Ego_r.pdb +0x38; the CObserver set and the States map are 4 bytes smaller each in
// STLport); CFrontEndScreen members as at 0x0054E3D0 (PDB less 0x30).
namespace NUISystem {
class CAction;
struct CCPPointerInfo;
}
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();
};
namespace NUISystem {
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CComponent;
class CUIInterface {
public:
    virtual ~CUIInterface();
    virtual void _slot01();
    virtual void _slot02();
    virtual CComponent* FindChild(CCharString name);     // slot 3 (+0x0c)
};
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _body[0x10];
};
class CActionParam {
public:
    virtual ~CActionParam();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _body1c[0x30 - 0x1c];                  // +0x1c
    float         Time;                                  // +0x30
    unsigned char _body34[0x15c - 0x34];                 // +0x34
};
class CChangingStateComponent : public CComponent {
public:
    virtual void Initialise();                           // 0x0052C730 for this class
};
class CFrontEndScreen : public CChangingStateComponent {
public:
    float                     m_fLastInputTime;          // +0x15c
    CCountedPointer<CAction>  PLeftClickAction;          // +0x160
    CComponent*               PAcceptButton;             // +0x168
    virtual void Initialise();
};
void __fastcall SetUpChildrenForAcceptButton(CComponent* component, CComponent* accept_button);   // 0x0054E350 (placeholder name)
void CFrontEndScreen::Initialise()
{
    CChangingStateComponent::Initialise();
    m_fLastInputTime = Time;
    PAcceptButton = FindChild(CCharString("UI_ACCEPT"));
    if (PAcceptButton)
        SetUpChildrenForAcceptButton(this, PAcceptButton);
}
}