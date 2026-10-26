// CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, interface/owner pair, vtable 0x012CBEB4, two arrays of five empty strings and two registered timers
// Leaf: ??0CQ_EndGameFocalSitesScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2A820, length 125 bytes, `ret 8`.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110, CCharString::CCharString() @ 0x0099E4B0 (2 x 5 times, two inline
// counted loops), CGameScriptInterface::RegisterTimer (slot 87, +0x15c) through the interface global @ 0x0143E8F8.
// Layout: Ego_r.pdb CQ_EndGameFocalSitesScript - 0x14: CurrentFocalSiteRegion +0x48, TeleportHSPs +0x4c,
// RegionDialogueNames +0x60, TeleportPosition +0x74, JackTimer +0x88, WorldTimer +0x8c.
// Shape: the landed quest-script constructor family (0x00E68F70 novtable script-object base; CTimer registers itself
// in its inline constructor); the second loop head is aligned (`jmp ; lea ecx,[ecx]`).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString();                                   // 0x0099E4B0
};

class CGameScriptInterface { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                    // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8

class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};

class CScriptBase { public:
    virtual void v00();
    unsigned char _body[0x3c];                       // +0x04..+0x40
    CScriptBase();                                   // 0x00CB8110
};

struct __declspec(novtable) CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};

namespace NScript {
class CQ_EndGameFocalSitesScript : public CScriptObject {
public:  // vtable 0x012CBEB4
    long        CurrentFocalSiteRegion;              // +0x48
    CCharString TeleportHSPs[5];                     // +0x4c
    CCharString RegionDialogueNames[5];              // +0x60
    C3DVector   TeleportPosition;                    // +0x74
    bool        MissionFailed;                       // +0x80
    bool        JackLeaves;                          // +0x81
    bool        EndCutScenePlayed;                   // +0x82
    bool        QuestStartScreenShown;               // +0x83
    long        counter;                             // +0x84
    CTimer      JackTimer;                           // +0x88
    CTimer      WorldTimer;                          // +0x8c
    bool        JackReached;                         // +0x90
    virtual void v00();
    CQ_EndGameFocalSitesScript(void* owner, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CQ_EndGameFocalSitesScript;

NScript::CQ_EndGameFocalSitesScript::CQ_EndGameFocalSitesScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}