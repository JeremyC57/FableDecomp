// NUISystem::CFrontEndScreen::CFrontEndScreen(long id) -- base CChangingStateComponent(id), empty left-click action, no accept button
// Leaf: ??0CFrontEndScreen@NUISystem@@QAE@J@Z
// Retail: Fable.exe @ 0x0054E3D0, 59 bytes, `ret 4`; one caller (0x0041D512); Ego_r.pdb ??0CFrontEndScreen@NUISystem@@QAE@J@Z.
// Body: CChangingStateComponent::CChangingStateComponent(id) (0x0052CC50); the three vtables of
// .?AVCFrontEndScreen@NUISystem@@ (0x012497E4 at +0, 0x012497BC at +4 CObserver, 0x012497B4 at +0x18 CActionParam);
// PLeftClickAction (CCountedPointer: Data +0x160, Info +0x164) = 0, PAcceptButton (+0x168) = 0; m_fLastInputTime is
// left uninitialised.
// Layout: retail offsets are the Ego_r.pdb ones less 0x30 (PDB m_fLastInputTime +0x18c / PLeftClickAction +0x190 /
// PAcceptButton +0x198); CFrontEndScreen::Initialise (0x0054E4B0) writes m_fLastInputTime at +0x15c and PAcceptButton at
// +0x168, so the class is modelled locally (engine/CFrontEndScreen.h has the PDB offsets).
namespace NUISystem {
class CAction;
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
};
class CUIInterface {
public:
    virtual ~CUIInterface();
};
class CObserver {
public:
    virtual ~CObserver();
    unsigned char _body[0x10];                           // +0x08 .. +0x18 of the component
};
class CActionParam {
public:
    virtual ~CActionParam();
};
class CComponent : public CUIInterface, public CObserver, public CActionParam {
public:
    unsigned char _body[0x15c - 0x1c];                   // +0x1c
};
class CChangingStateComponent : public CComponent {
public:
    CChangingStateComponent(long id);                    // 0x0052CC50
};
class CFrontEndScreen : public CChangingStateComponent {
public:
    CFrontEndScreen(long id);
    virtual ~CFrontEndScreen();
    float                     m_fLastInputTime;          // +0x15c
    CCountedPointer<CAction>  PLeftClickAction;          // +0x160
    CComponent*               PAcceptButton;             // +0x168
};
CFrontEndScreen::CFrontEndScreen(long id)
    : CChangingStateComponent(id), PAcceptButton(0)
{
}
}