// CGame::Play() -- create the first game component (the main game when starting straight into it, else the legacy or new front end), start it, and run components until one reports no successor; when only compiling front-end definitions, just build and discard it
// Leaf: Play
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00412F90, length 394 bytes (`ret`).
// Callees: CompileDefs @ 0x0042F70B, operator new @ 0x00BFEA1A, CWideString ctor/dtor/operator= @ 0x0099AED0 / 0x0099B510 /
//   0x0099B7D0, CCharString ctor/dtor @ 0x0099E4B0 / 0x0099EAE0, CMainGameComponent ctor @ 0x00418DCA,
//   CFrontendGameComponent ctor @ 0x00496070, CNewFrontendGameComponent ctor @ 0x0042EA8F; the component's virtual slots 0
//   (deleting destructor), 1 (start) and 2 (run one component, out: the next component).
// Globals: g_bCompileFrontendDefinitions 0x013B8648, g_bStartMainGame 0x013B8605, g_bUseLegacyFrontend 0x013B8642,
//   the main-game start path 0x013B7D5C (a CWideString), the retired component 0x013B7D58.
// Layout: Ego_r.pdb CGame CurrentGameComponent +0x08, Quit +0x20C; CMainGameComponentInit {InitialWorldName,
//   InitialWorldHolySiteName, InitialQuestName, SaveGameName}; the front-end init blocks are one zeroed long.
// Shape: each front-end arm calls Start() itself (VC7.1 tail-merges the two calls into the shared join; a single Start()
//   after the inner if/else leaves arm 2 forming its init address in edx); the component pointer doubles as Run's out parameter (so it lives in the frame from the start); `new` with the null-tested constructor call; the init block's zero store is shared by both front-end arms.
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
struct CMainGameComponentInit {
    CWideString InitialWorldName;
    CWideString InitialWorldHolySiteName;
    CCharString InitialQuestName;
    CWideString SaveGameName;
};
struct CFrontendGameComponentInit { long Nowt; };
struct CNewFrontendGameComponentInit { long Nothing; };
struct CGame;
struct CGameComponent {
    virtual ~CGameComponent();                          // slot 0 (deleting destructor)
    virtual void Start();                               // slot 1
    virtual bool Run(CGameComponent*& next);            // slot 2
};
struct CMainGameComponent : CGameComponent {
    CMainGameComponent(CGame* game, const CMainGameComponentInit& init);   // 0x00418DCA
    unsigned char _body[0x161e4];
};
struct CFrontendGameComponent : CGameComponent {
    CFrontendGameComponent(CGame* game, const CFrontendGameComponentInit& init);   // 0x00496070
    unsigned char _body[0x1e5c];
};
struct CNewFrontendGameComponent : CGameComponent {
    CNewFrontendGameComponent(CGame* game, const CNewFrontendGameComponentInit& init);   // 0x0042EA8F
    unsigned char _body[0x144];
};
void __fastcall CompileDefs();                          // 0x0042F70B
extern bool g_bCompileFrontendDefinitions;              // 0x013B8648
extern bool g_bStartMainGame;                           // 0x013B8605
extern bool g_bUseLegacyFrontend;                       // 0x013B8642
extern CWideString g_MainGameStartupPath;               // 0x013B7D5C
extern CGameComponent* g_pRetiredGameComponent;         // 0x013B7D58

struct CGame {
    void*           __vftable;                          // +0x00
    unsigned char   _pad04[4];
    CGameComponent* CurrentGameComponent;               // +0x08
    unsigned char   ParameterBuffer[0x200];             // +0x0C
    bool            Quit;                               // +0x20C
    void Play();
};

void CGame::Play()
{
    CGameComponent* component = 0;
    if (g_bCompileFrontendDefinitions)
        CompileDefs();
    if (g_bStartMainGame) {
        CMainGameComponentInit init;
        init.SaveGameName = g_MainGameStartupPath;
        component = new CMainGameComponent(this, init);
        component->Start();
    } else {
        if (g_bUseLegacyFrontend) {
            CFrontendGameComponentInit init;
            init.Nowt = 0;
            component = new CFrontendGameComponent(this, init);
            component->Start();
        } else {
            CNewFrontendGameComponentInit init;
            init.Nothing = 0;
            component = new CNewFrontendGameComponent(this, init);
            component->Start();
        }
    }
    if (g_bCompileFrontendDefinitions) {
        delete component;
        return;
    }
    if (component)
        CurrentGameComponent = component;
    while (!Quit) {
        bool running = CurrentGameComponent->Run(component);
        if (g_pRetiredGameComponent) {
            delete g_pRetiredGameComponent;
            g_pRetiredGameComponent = 0;
        }
        if (!running)
            Quit = true;
        else
            CurrentGameComponent = component;
    }
}