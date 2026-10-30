// CQ_WhiteBalverineWWScript::MonitorBalverine() -- show the white balverine's health bar until it is killed, then set the Knothole Glade objective, give the hero its head trophy and re-enable the Witchwood creature generators
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E18EB0, length 492 bytes (padded to 496).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CCharString(const char*, long) @ 0x0099EBF0,
//   ~CCharString @ 0x0099EAE0; CScriptThing::MsgIsKilledBy (slot 18, +0x48) on the balverine; through the script's
//   interface (+0x40): NewScriptFrame (7), GiveHeroObject (121, +0x1e4), SetQuestCardObjective (296, +0x4a0),
//   DisplayQuestInfo (321, +0x504), AddQuestInfoBarHealth (325, +0x514), Pause (376, +0x5e0),
//   SetCreatureGeneratorsEnabled (558, +0x8b8), StopOverrideMusic (697, +0xae4).
// Layout (retail = Ego_r PDB -0x14): WhiteBalverineAlive +0x4a, WhiteBalverine (CScriptThing) +0x50.
// Shape: WAIT_UNTIL(WhiteBalverine.MsgIsKilledBy("")) as `while (!x)` (neg/sbb/inc into bl); every literal is passed
//   straight to a `const CCharString&` parameter; the health-bar colour is a CRGBColour(255,255,255,255) temporary.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CRGBColour { public:                                     // B,G,R,A bytes (D3DCOLOR order)
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CScriptThing { public:                                   // vtable 0x01238C8C, 12 bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    void* m_pThing;
    long m_uid;
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_WhiteBalverineWWScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool MissionFailed;                                 // +0x48
    bool MissionSucceeded;                              // +0x49
    bool WhiteBalverineAlive;                           // +0x4a
    long WhiteBalverineHealth;                          // +0x4c
    CScriptThing WhiteBalverine;                        // +0x50
    void MonitorBalverine();
};
} // namespace NScript
using NScript::CQ_WhiteBalverineWWScript;

void NScript::CQ_WhiteBalverineWWScript::MonitorBalverine()
{
    Game->AddQuestInfoBarHealth(WhiteBalverine, CRGBColour(255, 255, 255, 255), "HUD_QUEST_ICON_WHITE_BALVERINE", 1.0f);
    Game->DisplayQuestInfo(true);
    while (!WhiteBalverine.MsgIsKilledBy("")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_08",
                                "KnotholeGlade", "KnotholeGlade");
    WhiteBalverineAlive = false;
    Game->Pause(0.5f);
    Game->StopOverrideMusic(false);
    Game->GiveHeroObject("OBJECT_TROPHY_BALVERINE_FM_HEAD_01", -1, false);
    Game->SetCreatureGeneratorsEnabled("Witchwood4", true);
}