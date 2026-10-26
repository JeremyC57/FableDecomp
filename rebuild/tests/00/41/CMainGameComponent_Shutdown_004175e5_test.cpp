// Behaviour test for CMainGameComponent::Shutdown @ 0x004175E5: the teardown order and the cleared state.
#include <cstdio>
#include <cstring>

static char g_log[128];
static void logs(const char* s) { std::strcat(g_log, s); }

struct CPolymorphic { virtual ~CPolymorphic() { logs("d"); } };
template <class T> struct scoped_ptr {
    T* px;
    typedef T* (scoped_ptr::*unspecified_bool_type)() const;
    T* get() const { return px; }
    operator unspecified_bool_type() const { return px == 0 ? 0 : &scoped_ptr::get; }
    T* operator->() const { return px; }
    void reset() { T* old = px; px = 0; if (old) delete old; }
};
struct CCountedPointer {
    void* Data; void* Info; const char* tag;
    void Release() { logs(tag); }
    void Clear() { Release(); Data = 0; }
};
struct CNetworkSession : CPolymorphic { void Shutdown() { logs("N"); } };
struct CSoundManager {
    void StopAll() { logs("s"); } void SetPaused(bool p) { logs(p ? "?" : "p"); } void StopMusic() { logs("m"); } void Shutdown() { logs("S"); }
};
static CSoundManager g_sound; static CSoundManager* g_pSoundManager = &g_sound;
struct CLSocketServer { void CloseSocket(void*) { logs("C"); } };
static CLSocketServer g_server; static CLSocketServer* g_pSocketServer = &g_server;
static CCountedPointer g_GlobalCountedPointer = { (void*)1, 0, "g" };
static int g_gameObj; static void* g_pGame = &g_gameObj;
class CInitBaseClass { public: bool init; bool IsInitialised() const { return init; } void Uninitialise_004ae990() { logs("U"); init = false; } };
class CEditWorld { public: void Shutdown() { logs("E"); } };
class CConsole { public: static CConsole* Get() { static CConsole c; return &c; } void Initialise(long w, long h, void*) { if (w == 0x60 && h == 0x29) logs("K"); } };
static void UninitialiseParticles() { logs("P"); }
static void* g_defManager = (void*)1;
class CGameDefinitionManager { public: static void Set(CGameDefinitionManager* m) { g_defManager = m; logs("D"); } };

class CMainGameComponent { public:
    long m_10; CCountedPointer m_14; scoped_ptr<CPolymorphic> m_1c, m_20; scoped_ptr<CNetworkSession> NetworkSession;
    scoped_ptr<CPolymorphic> m_28; CCountedPointer m_2c; CInitBaseClass InputSystem; CCountedPointer ConsoleFont, ConsoleFont2;
    CEditWorld EditWorld; CCountedPointer m_161c8; bool Running; unsigned char Socket[12];
    void UninitEventPackageLoading() { logs("l"); }
    void UninitEventPackageSaving() { logs("v"); }
    void Shutdown();
};

void CMainGameComponent::Shutdown()
{
    Running = false;
    if (g_pSoundManager) { g_pSoundManager->StopAll(); g_pSoundManager->SetPaused(false); g_pSoundManager->StopMusic(); }
    if (NetworkSession) NetworkSession->Shutdown();
    if (g_pSoundManager) g_pSoundManager->Shutdown();
    m_10 = 0;
    g_GlobalCountedPointer.Clear();
    g_pGame = 0;
    g_pSocketServer->CloseSocket(Socket);
    EditWorld.Shutdown();
    if (InputSystem.IsInitialised()) InputSystem.Uninitialise_004ae990();
    UninitEventPackageLoading();
    UninitEventPackageSaving();
    UninitialiseParticles();
    CConsole::Get()->Initialise(0x60, 0x29, ConsoleFont.Data);
    m_14.Clear(); m_1c.reset(); m_20.reset(); NetworkSession.reset(); m_28.reset();
    m_2c.Clear(); ConsoleFont.Clear(); ConsoleFont2.Clear(); m_161c8.Clear();
    CGameDefinitionManager::Set(0);
}

int main()
{
    CMainGameComponent c;
    c.m_10 = 5; c.Running = true; c.InputSystem.init = true;
    CCountedPointer a = { (void*)1, 0, "1" }, b = { (void*)1, 0, "2" }, f = { (void*)1, 0, "3" }, f2 = { (void*)1, 0, "4" }, e = { (void*)1, 0, "5" };
    c.m_14 = a; c.m_2c = b; c.ConsoleFont = f; c.ConsoleFont2 = f2; c.m_161c8 = e;
    c.m_1c.px = new CPolymorphic; c.m_20.px = 0; c.NetworkSession.px = new CNetworkSession; c.m_28.px = new CPolymorphic;
    c.Shutdown();
    int ok = std::strcmp(g_log, "spmNSgCEUlvPK1ddd2345D") == 0 && !c.Running && c.m_10 == 0 && g_pGame == 0 && g_defManager == 0
          && c.m_14.Data == 0 && c.m_161c8.Data == 0 && c.NetworkSession.px == 0 && c.m_1c.px == 0 && !c.InputSystem.init;
    if (!ok) { std::printf("004175e5_TEST FAIL\n"); return 1; }
    std::printf("004175e5_TEST PASS\n");
    return 0;
}