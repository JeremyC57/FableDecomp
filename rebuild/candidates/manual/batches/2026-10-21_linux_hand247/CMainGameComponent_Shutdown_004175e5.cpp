// CMainGameComponent::Shutdown() -- tear the game component down: stop the sound system, shut the network session, drop the global game/server hooks, uninitialise the event-package machinery and console, then release every owned subsystem pointer and clear the definition manager
// Leaf: Shutdown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004175E5, length 354 bytes (unaligned TU), tail `jmp`.
// Callees (several through ILT thunks): the sound manager @ 0x013B8394 (virtual slots 16 +0x40 and 18 +0x48, then 0x00991750 and
//   0x009918F0), the network session's (+0x24, a scoped_ptr with the safe-bool idiom: `neg ; sbb ; test edx,offset get` against
//   scoped_ptr::get 0x00418F47) virtual slot 10 (+0x28), the global counted pointer @ 0x013B8790 (release 0x0041915A), the server
//   @ 0x013B8390 CloseSocket 0x009BF5A0, 0x0049BA20 on +0x16178, CInitBaseClass IsInitialised 0x0099A330 / Uninitialise 0x004AE990 on
//   +0x13AB8, UninitEventPackageLoading 0x004160A6 / UninitEventPackageSaving 0x00416122, the global Uninitialise 0x00AEBEA0,
//   CConsole::Get 0x00414C90 + Initialise 0x009ED190, the counted-pointer releases 0x00403750 / 0x004190B6 / 0x004057A0 / 0x00419108,
//   the scoped_ptr resets (virtual scalar deleting dtors), CGameDefinitionManager::Set(0) 0x0044C71F (`__fastcall`, the tail jmp).
// Globals: game @ 0x013B86A0 (cleared).
// Layout: local model of the retail component (no engine/ header): owned pointers at +0x10..+0x2c, the session +0x24, the
//   event-package and console counted pointers +0x16144/+0x1614c/+0x161c8, the running flag +0x161e0.
// Shape: each counted pointer is cleared by its out-of-line release followed by `Data = 0` (an inline Clear()); the scoped_ptrs
//   reset with `old = px; px = 0; delete old`; the 1 for the deleting dtors lives in ebp (`xor ebp,ebp ; inc ebp`).
// Pragma: #pragma optimize("s", on) -- a size-optimised TU (unaligned entry; constants materialised in registers).
#pragma optimize("s", on)
class CPolymorphic {
public:
    virtual ~CPolymorphic();
};

template <class T> struct scoped_ptr {
    T* px;
    typedef T* (scoped_ptr::*unspecified_bool_type)() const;
    T* get() const { return px; }
    operator unspecified_bool_type() const { return px == 0 ? 0 : &scoped_ptr::get; }
    T* operator->() const { return px; }
    void reset()
    {
        T* old = px;
        px = 0;
        if (old)
            delete old;
    }
};

#define COUNTED_POINTER(Name, ReleaseAddr)                                   \
    struct Name {                                                            \
        void* Data;                                                          \
        void* Info;                                                          \
        void Release();                                                      \
        __forceinline void Clear() { Release(); Data = 0; }                  \
    };
COUNTED_POINTER(CCountedPointer403750, 0x00403750)
COUNTED_POINTER(CCountedPointer4190b6, 0x004190B6)
COUNTED_POINTER(CCountedPointer4057a0, 0x004057A0)
COUNTED_POINTER(CCountedPointer419108, 0x00419108)
COUNTED_POINTER(CCountedPointer41915a, 0x0041915A)
#undef COUNTED_POINTER

class CNetworkSession : public CPolymorphic {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Shutdown();                         // slot 10 (+0x28)
};

class CSoundManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void StopAll();                          // slot 16 (+0x40)
    virtual void v17();
    virtual void SetPaused(bool paused);             // slot 18 (+0x48)
    void StopMusic();                                // 0x00991750
    void Shutdown();                                 // 0x009918F0
};
extern CSoundManager* g_pSoundManager;               // 0x013B8394

class CLSocketServer {
public:
    void CloseSocket(void* socket);                  // 0x009BF5A0
};
extern CLSocketServer* g_pSocketServer;              // 0x013B8390
extern CCountedPointer41915a g_GlobalCountedPointer; // 0x013B8790
extern void* g_pGame;                                // 0x013B86A0

class CInitBaseClass {
public:
    bool IsInitialised() const;                      // 0x0099A330
    void Uninitialise();                             // 0x004AE990
};
class CEditWorld {
public:
    void Shutdown();                                 // 0x0049BA20
};
class CConsole {
public:
    static CConsole* Get();                          // 0x00414C90
    void Initialise(long width, long height, void* font);   // 0x009ED190
};
void UninitialiseParticles();                        // 0x00AEBEA0
class CGameDefinitionManager {
public:
    static void __fastcall Set(CGameDefinitionManager* manager);   // 0x0044C71F
};

class CMainGameComponent {
public:
    virtual void v00();
    unsigned char          Socket[0x0c];             // +0x04
    long                   m_10;                     // +0x10
    CCountedPointer403750  m_14;                     // +0x14
    scoped_ptr<CPolymorphic> m_1c;                   // +0x1c
    scoped_ptr<CPolymorphic> m_20;                   // +0x20
    scoped_ptr<CNetworkSession> NetworkSession;      // +0x24
    scoped_ptr<CPolymorphic> m_28;                   // +0x28
    CCountedPointer4190b6  m_2c;                     // +0x2c
    unsigned char          _pad34[0x13ab8 - 0x34];
    CInitBaseClass         InputSystem;              // +0x13ab8
    unsigned char          _pad13ab9[0x16144 - 0x13ab9];
    CCountedPointer4057a0  ConsoleFont;              // +0x16144
    CCountedPointer4057a0  ConsoleFont2;             // +0x1614c
    unsigned char          _pad16154[0x16178 - 0x16154];
    CEditWorld             EditWorld;                // +0x16178
    unsigned char          _pad16179[0x161c8 - 0x16179];
    CCountedPointer419108  m_161c8;                  // +0x161c8
    unsigned char          _pad161d0[0x161e0 - 0x161d0];
    bool                   Running;                  // +0x161e0

    void UninitEventPackageLoading();                // 0x004160A6
    void UninitEventPackageSaving();                 // 0x00416122
    void Shutdown();
};

void CMainGameComponent::Shutdown()
{
    Running = false;
    if (g_pSoundManager) {
        g_pSoundManager->StopAll();
        g_pSoundManager->SetPaused(false);
        g_pSoundManager->StopMusic();
    }
    if (NetworkSession)
        NetworkSession->Shutdown();
    if (g_pSoundManager)
        g_pSoundManager->Shutdown();
    m_10 = 0;
    g_GlobalCountedPointer.Clear();
    g_pGame = 0;
    g_pSocketServer->CloseSocket(Socket);
    EditWorld.Shutdown();
    if (InputSystem.IsInitialised())
        InputSystem.Uninitialise();
    UninitEventPackageLoading();
    UninitEventPackageSaving();
    UninitialiseParticles();
    CConsole::Get()->Initialise(0x60, 0x29, ConsoleFont.Data);
    m_14.Clear();
    m_1c.reset();
    m_20.reset();
    NetworkSession.reset();
    m_28.reset();
    m_2c.Clear();
    ConsoleFont.Clear();
    ConsoleFont2.Clear();
    m_161c8.Clear();
    CGameDefinitionManager::Set(0);
}
