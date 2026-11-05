// CTCWife::InitiateDivorce() -- unless a hero-information screen is up, shows the def's "divorce occurred" subtitle, flags WaitingForDivorce, joins the global update list and pauses the game for the current player (SendPauseEvent(true) inlined)
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pPlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CEF20, 192 bytes. __fastcall, `ret`, /GS cookie 0x0139C8A8.
// Callees: CPlayerGui::GetPHeroInfoScreensManager() const 0x00437CD0 (manifest "CWorld::DrawGetEnvironment", propagated);
//          CSubtitleRenderer::SetText(CFontBank*, const CCharString&, unsigned long, float, bool, bool) 0x00492ED1 on the
//          renderer embedded at CPlayerGui+0x18; CTCBase::AddToGlobalTCs() 0x00686ED0; CTCBase::PeekWorld() const
//          0x00686A80; CWorld::PeekMyPlayer() 0x0049D9D0 (player number byte +0x28); CMemoryBuffer::Copy 0x009F1760;
//          CMainGameComponent::AddGameEvent(const CGameEvent&) 0x0041649C; __security_check_cookie 0x00BFE9F9.
//          Globals: (&CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get()) 0x013B8790, CMainGameComponent::Get() 0x013B86A0 (font bank at +0x1614c).
// Shape: the screens manager's +0x34 flag byte (descriptive: a screen is active) is the only guard. SetText takes the
//        font bank, the def's TextGuiWifeDivorceOccurred (CHeroMarriageDef +0xa4, formed with `add eax,0A4h`), colour 0x22,
//        time 0.0f, true, true. The pause event is the landed CTCWife::SendPauseEvent(bool) 0x006CEBF0 body inlined with
//        a constant true: CGameEvent(9) initialised in its frame slot (Type 9, Player -1, EndPos 0, Valid, !Replacement),
//        the player number stored over Player, the normalised flag byte in its own slot handed to Copy(1, &flag). The
//        40-byte event buffer is what makes the TU /GS.
// Layout: CTCWife ParentThing +0x04, PHeroMarriageDef +0x0c, WaitingForDivorce +0x29 (engine/CTCWife.h).
class CCharString;
class CFontBank;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBool(bool value) { value = value ? true : false; Copy(1, &value); }
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x1614c];
    CFontBank* m_pFontBank;                        // +0x1614c
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
class CSubtitleRenderer { public:                         // embedded at CPlayerGui+0x18
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
namespace NHeroInformationScreens {
class CManager { public:
    unsigned char _pad00[0x34];
    unsigned char m_screenActive;                  // +0x34 (descriptive: a hero-information screen is up)
};
}
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer m_subtitleRenderer;          // +0x18
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
class CPlayer { public:
    unsigned char _pad_0x00[0x28];
    signed char   m_playerNumber;                  // +0x28
};
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:              // 0x0049D9D0
};
class CHeroMarriageDef { public:
    unsigned char _pad00[0xa4];
    unsigned char TextGuiWifeDivorceOccurred[4];   // +0xa4 CCharString
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
public:
    void AddToGlobalTCs();   // 0x00686ED0
};

class CTCWife : public CTCBase { public:
    void*             ParentThing;                 // +0x04
    unsigned char     _pad_0x08[4];
    CHeroMarriageDef* PHeroMarriageDef;            // +0x0c CDefPointer<CHeroMarriageDef const>::Object
    unsigned char     _pad_0x10[0x29 - 0x10];
    bool              WaitingForDivorce;           // +0x29
    void InitiateDivorce();
};
void CTCWife::InitiateDivorce()
{
    if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetPHeroInfoScreensManager()->m_screenActive)
        return;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().m_subtitleRenderer.SetText(CMainGameComponent::Get()->m_pFontBank,
        *(const CCharString*)PHeroMarriageDef->TextGuiWifeDivorceOccurred, 0x22, 0.0f, true, true);
    WaitingForDivorce = true;
    AddToGlobalTCs();
    CGameEvent gameEvent(9);                       // SendPauseEvent(true) written out: the constant flag folds to one store
    gameEvent.Player = ((CPlayer*)((CWorld*)&(PeekWorld()))->PeekMyPlayer())->m_playerNumber;
    gameEvent.AddBool(true);
    CMainGameComponent::Get()->AddGameEvent(gameEvent);
}