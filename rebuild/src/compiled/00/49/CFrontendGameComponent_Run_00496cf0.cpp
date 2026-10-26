// CFrontendGameComponent::Run(CGameComponent*& nextComponent) -- pump the front end until it quits, then hand over to the new front end or to a main game built from the chosen world and save (setting the hero's joystick), or shut down with no successor
// Leaf: Run
// Retail: Fable.exe (TLC, VC7.1) @ 0x00496CF0, 411 bytes, `ret 4`.
// Callees: the engine system accessor 0x009A4EC0 and its Update 0x009A6460, CFrontendGameComponent::Input 0x004963A0 /
//   Output 0x00494900 / shutdown 0x00495780, operator new 0x00BFEA1A, CNewFrontendGameComponent(CGame&, const Init&) 0x0042EA8F
//   and CMainGameComponent(CGame&, const Init&) 0x00418DCA (incremental-link thunks), CWideString() 0x0099AED0 / operator=
//   0x0099B7D0 / dtor 0x0099B510, CCharString() 0x0099E4B0 / dtor 0x0099EAE0, CGameJoystickManager::Get 0x0049BB90 (static
//   fastcall) / SetMainPlayerJoystickDeviceNumber 0x0049BB30; the new component's virtual slot 1. Global 0x013B7D58.
// Layout: PDB CFrontendGameComponent with retail offsets (Game +0xc, Quit +8, Running +9, CreateGame +0x10, joystick +0x14,
//   CreateReleaseFrontEnd +0x19, SaveToLoad +0x1e2c, WorldToLoad +0x1e40; the network arrays are smaller in retail).
class CWideString { public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
};
class CCharString { public:
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
};
class CGame;
struct CEngineSystem { long Update(); };                 // 0x009A6460
CEngineSystem* GetEngineSystem();                        // 0x009A4EC0
class CGameJoystickManager { public:
    static CGameJoystickManager& __fastcall Get();       // 0x0049BB90
    void SetMainPlayerJoystickDeviceNumber(long device); // 0x0049BB30
};
class CGameComponent { public:
    virtual void v00();
    virtual void Init();                                 // slot 1
};
class CNewFrontendGameComponentInit { public: long Nothing; CNewFrontendGameComponentInit() : Nothing(0) { } };
class CNewFrontendGameComponent : public CGameComponent { public:
    unsigned char Body[0x144];
    CNewFrontendGameComponent(CGame& game, const CNewFrontendGameComponentInit& init);   // 0x0042EA8F
};
class CMainGameComponentInit { public:
    CWideString InitialWorldName;
    CWideString InitialWorldHolySiteName;
    CCharString InitialQuestName;
    CWideString SaveGameName;
};
class CMainGameComponent : public CGameComponent { public:
    unsigned char Body[0x161e4];
    CMainGameComponent(CGame& game, const CMainGameComponentInit& init);   // 0x00418DCA
};
struct CFrontendGameComponent;
extern CFrontendGameComponent* g_pLastFrontend;          // 0x013B7D58 (descriptive)
struct CFrontendGameComponent {
    void*         __vftable;                             // +0x00
    long          m04;
    bool          Quit;                                  // +0x08
    bool          Running;                               // +0x09
    CGame*        Game;                                  // +0x0c
    bool          CreateGame;                            // +0x10
    long          CreateGameJoystickDeviceNumber;        // +0x14
    bool          CreateEditor;                          // +0x18
    bool          CreateReleaseFrontEnd;                 // +0x19
    unsigned char Body[0x1e2c - 0x1c];
    CWideString   SaveToLoad;                            // +0x1e2c
    unsigned char Body2[0x10];
    CWideString   WorldToLoad;                           // +0x1e40
    void Input();                                        // 0x004963A0
    void Output();                                       // 0x00494900
    void Shutdown();                                     // 0x00495780
    bool Run(CGameComponent*& nextComponent);
};

bool CFrontendGameComponent::Run(CGameComponent*& nextComponent)
{
    Running = true;
    while (!Quit) {
        long state = GetEngineSystem()->Update();
        if (state == 2)
            Quit = true;
        else if (state == 1) {
            Input();
            Output();
        }
    }
    Running = false;
    if (CreateReleaseFrontEnd) {
        Shutdown();
        CNewFrontendGameComponentInit init;
        CGameComponent* component = new CNewFrontendGameComponent(*Game, init);
        component->Init();
        nextComponent = component;
        g_pLastFrontend = this;
        return true;
    }
    if (CreateGame) {
        Shutdown();
        CMainGameComponentInit init;
        init.InitialWorldName = WorldToLoad;
        init.SaveGameName = SaveToLoad;
        CGameJoystickManager::Get().SetMainPlayerJoystickDeviceNumber(CreateGameJoystickDeviceNumber);
        CGameComponent* component = new CMainGameComponent(*Game, init);
        component->Init();
        nextComponent = component;
        g_pLastFrontend = this;
        return true;
    }
    Shutdown();
    g_pLastFrontend = this;
    return false;
}