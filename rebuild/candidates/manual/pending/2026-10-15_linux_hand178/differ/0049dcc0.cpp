// CWorld::ResetToFrontEnd() -- leave the game: build the front-end game component and hand it to the main component as the next one to run, then quit the main component
// Grade: DIFFER(142v142) -- register allocation only: second arm's init address in edx (retail ecx) and the vtable loads in eax/edx (retail edx/eax)
// Retail: Fable.exe @ 0x0049DCC0, 142 bytes. PDB ?ResetToFrontEnd@CWorld@@QAEXXZ
// Callees: 0x00991620 on the global 0x013B8394 (a bool setter: stores +0x26 and calls its slot 2 when it changes;
// the manifest label "CTCDParticleEmitter::EditFrameUpdate" is a byte-twin propagation), operator new 0x00BFEA1A,
// CFrontendGameComponent(CGame&, const CFrontendGameComponentInit&) 0x00496070 (0x1E60 bytes),
// CNewFrontendGameComponent(CGame&, const CNewFrontendGameComponentInit&) 0x0042EA8F (0x148 bytes),
// CGameComponent slot 1 Init / slot 4 SetQuit (rtti-vtables.tsv). Globals: the CGame object 0x013B83D0 and the
// legacy-front-end switch 0x013B8642 (as named in the landed CGame::Play 0x00412F90).
// CWorld: +0x08 Component (CMainGameComponent*, engine/CWorld.h); CMainGameComponent +0x161C4 NextComponentToRun
// (PDB +0x161CC: the retail class is 8 bytes smaller ahead of it).
// Shape: one 4-byte init record ({Nowt}/{Nothing} = 0) shared by both arms and zeroed before the branch; a failed
// allocation leaves a null component that is still called (no null test in retail).
struct CGame;
extern CGame g_Game;                         // 0x013B83D0
extern bool  g_UseLegacyFrontend;            // 0x013B8642

struct CGameState {
    void SetFlag(bool value);                // 0x00991620
};
extern CGameState* g_pGameState;             // 0x013B8394

struct CGameComponent {
    virtual void OnPreDeviceReset();         // slot 0
    virtual void Init();                     // slot 1 (+0x04)
    virtual bool Run(CGameComponent** next); // slot 2
    virtual void ChangeTextureColourDepth(long depth);   // slot 3
    virtual void SetQuit();                  // slot 4 (+0x10)
};

struct CFrontendGameComponentInit { long Nowt; };
struct CNewFrontendGameComponentInit { long Nothing; };

struct CFrontendGameComponent : CGameComponent {   // 0x1E60 bytes
    unsigned char _body[0x1e5c];
    CFrontendGameComponent(CGame& game, const CFrontendGameComponentInit& init);       // 0x00496070
};
struct CNewFrontendGameComponent : CGameComponent {   // 0x148 bytes
    unsigned char _body[0x144];
    CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit& init); // 0x0042EA8F
};

struct CMainGameComponent : CGameComponent {
    unsigned char   _pad[0x161c0];
    CGameComponent* NextComponentToRun;      // +0x161C4 (PDB +0x161CC)
};

struct CWorld {
    void*               __vftable;           // +0x00
    unsigned char       _base_0x04[0x4];     // +0x04
    CMainGameComponent* Component;           // +0x08
    void ResetToFrontEnd();
};

void CWorld::ResetToFrontEnd()
{
    if (g_pGameState)
        g_pGameState->SetFlag(false);
    CGameComponent* frontEnd;
    if (g_UseLegacyFrontend)
    {
        CFrontendGameComponentInit init;
        init.Nowt = 0;
        frontEnd = new CFrontendGameComponent(g_Game, init);
    }
    else
    {
        CNewFrontendGameComponentInit init;
        init.Nothing = 0;
        frontEnd = new CNewFrontendGameComponent(g_Game, init);
    }
    frontEnd->Init();
    CMainGameComponent* component = Component;
    component->NextComponentToRun = frontEnd;
    component->SetQuit();
}
