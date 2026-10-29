// CFrontEndManager::Update(float dt) -- per-frame front-end update: unpause the file installer two seconds after a pause, pop the virtual-keyboard screen once it finishes, act on a finished profile-name entry (load an existing profile, create a new one, or go to the profile screen), update every screen component, and retire finished fade / message / load-game components
// Leaf: ?Update@CFrontEndManager
// Layout: local view (engine/CFrontEndManager.h declares no methods); members +0x1c..+0xd9 descriptive from the retail accesses.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);                 // 0x0099B720
    ~CWideString();                                        // 0x0099B510
    const unsigned short* GetAsCharArray() const;          // 0x0099AF10
};
extern "C" int __cdecl _wcsicmp(const unsigned short* a, const unsigned short* b);   // 0x00F3FB84
double __fastcall GFGetTime();                             // 0x009E1BC0
class CFileInstaller { public:
    void Unpause();                                        // 0x009C3250
    static CFileInstaller* __fastcall Get();               // 0x00404440 (folded CTBaseSingleton<>::Get)
};
extern void* g_pFileInstallerActive;                       // 0x013CA818
struct SComponentStatus {
    unsigned char _b0[3];
    bool Active;                                           // +3
    SComponentStatus(const SComponentStatus& other);
};
struct CComponentBase {
    virtual void v000();
    virtual void Update(float dt);                    // slot 1 (+0x04)
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual SComponentStatus GetStatus();             // slot 14 (+0x38)
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void SetState(long state);                // slot 48 (+0xc0)
    virtual bool IsFinished();                        // slot 49 (+0xc4)
};
struct CComponentSecondBase { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void Release(); };
struct CFadeComponent : CComponentBase, CComponentSecondBase { };
struct CRefVirtual { virtual void v00(); virtual void Destroy(); };
struct CRefCounted {
    long m_count;
    __forceinline void DecRef() { --m_count; }
    __forceinline long GetRefCount() const { return m_count; }
};
struct CRefBase : CRefVirtual, CRefCounted {
    __forceinline void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CScreenDef : CRefBase {
    unsigned char _pad08[0x194];
    CWideString NewProfileText;                            // +0x19c
    CWideString ProfileText;                               // +0x1a0
};
struct CDefPointer {
    CScreenDef* m_p;
    __forceinline ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CScreenDef* operator->() const { return m_p; }
};
struct CComponentSecondBase;
namespace NUISystem { class CManager { public:
    virtual void v000();
    virtual void v001();
    virtual void RemoveComponent(CComponentSecondBase* component);   // slot 2 (+0x08)
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void SetText(const CWideString& text, bool now);   // slot 40 (+0xa0)
    virtual void v041();
    virtual CDefPointer GetDef();                     // slot 42 (+0xa8)
    static CManager* __fastcall GetInstance();           // 0x0041E5F2
}; }
struct CScreenStack {
    CComponentBase*& Top();                                // 0x0059B039
    void Pop();                                            // 0x0059AEE5
};
struct CScreenNode { unsigned char _b[8]; CScreenNode* Left; CScreenNode* Right; long Key; CComponentBase* Value; };
extern "C" CScreenNode* __cdecl _Rb_increment(CScreenNode* node);   // 0x004292C0
struct CScreenMap {
    CScreenNode* Header;
    unsigned long Count;
    unsigned long _08;
    CComponentBase*& operator[](const long& key);          // 0x0059B5D7
};
// same-TU read-only stand-in for the screen map's operator[] (0x0059B5D7): the lookup reads the key, the result is a node value
#pragma auto_inline(off)
__declspec(noinline) CComponentBase*& CScreenMap::operator[](const long& key)
{
    CScreenNode* y = Header;
    CScreenNode* x = Header->Left;
    while (x) {
        if (!(x->Key < key)) { y = x; x = x->Left; }
        else x = x->Right;
    }
    if (y == Header || key < y->Key)
        y = Header;
    return y->Value;
}
#pragma auto_inline(on)
struct CWideStringVector {
    CWideString* _M_start; CWideString* _M_finish; CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CWideStringVector();                                  // 0x00412130
};
struct CVirtualKeyboard {
    unsigned char _pad[4];
    bool Cancelled;                                        // +4
    bool Finished;                                         // +5
    CWideString GetProfileName();                          // 0x00851890
};
struct CProfileSource { unsigned char _pad[8]; CWideString ProfileName; };
class CUserProfileManager : public CProfileSource { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
struct CFrontEndGame {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Reset();                                  // slot 4 (+0x10)
    unsigned char _pad04[0x25];
    bool Dirty;                                            // +0x29
    void LoadGame(CWideString* name);                      // 0x00594F36
};
#pragma pack(push, 4)
struct CFrontEndManager {
    virtual void v00();
    unsigned char _pad04[0x18];
    CFrontEndGame* Game;                                   // +0x1c
    CScreenStack ScreenStack;                              // +0x20
    unsigned char _pad21[0x33];
    CScreenMap Screens;                                    // +0x54
    CVirtualKeyboard* VirtualKeyboard;                     // +0x60
    bool CanLoad;                                          // +0x64
    unsigned char _pad65[0x33];
    CComponentBase* FadeOwner;                             // +0x98
    CFadeComponent* Fade;                                  // +0x9c
    bool KeyboardActive;                                   // +0xa0
    CComponentBase* MessageBox;                            // +0xa4
    CComponentBase* LoadBox;                               // +0xa8
    CWideString LoadName;                                  // +0xac
    unsigned char _padb0[0x20];
    double InstallerPauseTime;                             // +0xd0
    bool InstallerPaused;                                  // +0xd8
    bool InstallerResumed;                                 // +0xd9
    void LaunchVirtualKeyboard();                          // 0x00596917
    void EnableInput(bool enable);                         // 0x00594FA9
    void GetProfileNames(CWideStringVector* names);        // 0x005955AB
    void LoadProfile(CWideString* name);                   // 0x0059899A
    bool CreateNewProfile(CWideString* name);              // 0x0059697A
    void GotoNextScreen(CComponentBase* screen, bool back);   // 0x00596763
    void Update(float dt);
};
#pragma pack(pop)


#pragma auto_inline(off)
__declspec(noinline) void CFrontEndManager::GetProfileNames(CWideStringVector* names)
{
    CWideString* start = names->_M_start;
    names->_M_finish = start;
}
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
// R&D I-3 sweep: a same-TU auto_inline(off) body for ~vector<CWideString> 0x00412130 (destroys and frees only its own
// storage) makes the profile-name vector non-escaping; the search loop then loads begin straight into edi as in retail.
#pragma auto_inline(off)
CWideStringVector::~CWideStringVector()
{
    for (CWideString* p = _M_start; p != _M_finish; ++p)
        p->~CWideString();
    if (_M_start)
        free(_M_start);
}
#pragma auto_inline(on)
#pragma auto_inline(on)

void CFrontEndManager::Update(float dt)
{
    double now = GFGetTime();
    if (now - InstallerPauseTime >= 2.0 && InstallerPaused && g_pFileInstallerActive) {
        CFileInstaller::Get()->Unpause();
        InstallerPaused = false;
    }
    if (KeyboardActive && ScreenStack.Top()) {
        if (ScreenStack.Top()->IsFinished() || !ScreenStack.Top()->GetStatus().Active) {
            KeyboardActive = false;
            LaunchVirtualKeyboard();
            if (ScreenStack.Top() == Screens[0x13])
                ScreenStack.Pop();
        }
    }
    if (VirtualKeyboard && VirtualKeyboard->Finished) {
        if (!VirtualKeyboard->Cancelled) {
            CWideString current(::CUserProfileManager::Get().ProfileName);
            CWideString name = VirtualKeyboard->GetProfileName();
            CWideStringVector names;
            GetProfileNames(&names);
            CWideString* it = names._M_start;
            for (; it != names._M_finish; ++it)
                if (_wcsicmp(it->GetAsCharArray(), name.GetAsCharArray()) == 0)
                    break;
            if (it != names._M_finish) {
                EnableInput(false);
                LoadProfile(&current);
                GotoNextScreen(Screens[0x13], false);
                NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
                NUISystem::CManager::GetInstance()->SetText(manager->GetDef()->ProfileText, true);
            }
            else {
                bool created = CreateNewProfile(&name);
                EnableInput(false);
                if (created)
                    GotoNextScreen(Screens[0], true);
                else {
                    LoadProfile(&current);
                    GotoNextScreen(Screens[0x1b], false);
                }
            }
        }
        else {
            NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
            NUISystem::CManager::GetInstance()->SetText(manager->GetDef()->NewProfileText, true);
            EnableInput(true);
        }
    }
    for (CScreenNode* node = Screens.Header->Left; node != Screens.Header; node = _Rb_increment(node))
    {
        CComponentBase* screen = node->Value;
        if (screen)
            screen->Update(dt);
    }
    if (FadeOwner && (FadeOwner->IsFinished() || !FadeOwner->GetStatus().Active)) {
        Fade->SetState(5);
        NUISystem::CManager::GetInstance()->RemoveComponent(Fade);
        Fade->Release();
        FadeOwner = 0;
        Fade = 0;
    }
    if (MessageBox && (MessageBox->IsFinished() || !MessageBox->GetStatus().Active) && CanLoad) {
        MessageBox = 0;
        if (InstallerPaused && g_pFileInstallerActive) {
            CFileInstaller::Get()->Unpause();
            InstallerPaused = false;
            InstallerResumed = true;
        }
        CFrontEndGame* game = Game;
        game->Reset();
        game->Dirty = true;
    }
    if (LoadBox && (LoadBox->IsFinished() || !LoadBox->GetStatus().Active)) {
        LoadBox = 0;
        if (InstallerPaused && g_pFileInstallerActive) {
            CFileInstaller::Get()->Unpause();
            InstallerPaused = false;
            InstallerResumed = true;
        }
        Game->LoadGame(&LoadName);
    }
}