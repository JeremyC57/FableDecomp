#include <cstdio>
#include <cstring>
// Behaviour test for 0042ebb6 CNewFrontendGameComponent::Shutdown: the def reference dropped (destroyed on the last one), the
// component unregistered before the scene release, the game state resumed only when no game is being created, both file
// lists emptied and the screen cleared to opaque black when a display exists.
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CFrontEndDef {
    virtual void v00() {}
    virtual void Destroy() { Log("DefDestroy"); }
    long RefCount;                                 // +0x04
    void ReleaseRef() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    void Clear() { if (Object) { Object->ReleaseRef(); Object = 0; } }
    CDefPointer& operator=(T* p) { Clear(); Object = p; return *this; }
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last) { Log("Erase"); _M_finish = first; return first; }
    void clear() { erase(_M_start, _M_finish); }
};
struct CWideString { void* m_pData; };
struct CDateAndTime { unsigned char _body[0x10]; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CDisplayManager {
public:
    void ClearRenderTarget(const CRGBColour& c, unsigned long flags) { Log(c.R == 0 && c.G == 0 && c.B == 0 && c.A == 0xff && flags == 7 ? "Clear" : "ClearBAD"); }
    void SwapScreens() { Log("Swap"); }
};
class CGameStateOwner_013b8394 {                   // the object held at 0x013B8394
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void ResumeGame_slot16() { Log("S16"); }
    virtual void v17() {}
    virtual void SetPaused_slot18(bool paused) { Log(paused ? "S18BAD" : "S18"); }
    void Restore_00991750() { Log("R1"); }
    void Restore_009918f0() { Log("R2"); }
};
struct CFrontendGlobal_013b838c {
    unsigned char _pad0[0x38];
    bool          Flag38;                          // +0x38
};
class CNewFrontendGameComponent;
void SetActiveFrontend_004336b0(CNewFrontendGameComponent* component) { Log(component ? "SetBAD" : "Set0"); }
CNewFrontendGameComponent* g_pNewFrontendGameComponent;
CFrontendGlobal_013b838c* g_pFrontendGlobal_013b838c;
CGameStateOwner_013b8394* g_pGameState_013b8394;
CDisplayManager* g_pDisplayDevice;
class CNewFrontendGameComponent {
public:
    void*                     __vftable;           // +0x00
    unsigned char             _pad04[0x25];
    bool                      m_bCreateGame;       // +0x29
    unsigned char             _pad2a[0x12];
    CDefPointer<CFrontEndDef> m_pFrontEndDef;      // +0x3c
    unsigned char             _pad40[0x2c];
    vector<CWideString>       FileNames;           // +0x6c
    vector<CDateAndTime>      FileTimes;           // +0x78
    void ReleaseScene_0042dd28() { Log(g_pNewFrontendGameComponent ? "SceneBAD" : "Scene"); }
    void ReleaseXMVCode() { Log("XMV"); }
    void Shutdown();
};

void CNewFrontendGameComponent::Shutdown()
{
    m_pFrontEndDef = 0;
    SetActiveFrontend_004336b0(0);
    g_pNewFrontendGameComponent = 0;
    ReleaseScene_0042dd28();
    g_pFrontendGlobal_013b838c->Flag38 = true;
    if (!m_bCreateGame && g_pGameState_013b8394) {
        g_pGameState_013b8394->ResumeGame_slot16();
        g_pGameState_013b8394->SetPaused_slot18(false);
        g_pGameState_013b8394->Restore_00991750();
        g_pGameState_013b8394->Restore_009918f0();
    }
    ReleaseXMVCode();
    FileNames.clear();
    FileTimes.clear();
    if (g_pDisplayDevice) {
        g_pDisplayDevice->ClearRenderTarget(CRGBColour(0, 0, 0, 0xff), 7);
        g_pDisplayDevice->SwapScreens();
    }
}

int main()
{
    static CNewFrontendGameComponent c;
    CFrontEndDef def; def.RefCount = 1;
    CWideString names[2]; CDateAndTime times[1];
    c.m_pFrontEndDef.Object = &def; c.m_bCreateGame = false;
    c.FileNames._M_start = names; c.FileNames._M_finish = names + 2; c.FileTimes._M_start = times; c.FileTimes._M_finish = times + 1;
    CFrontendGlobal_013b838c global; global.Flag38 = false; g_pFrontendGlobal_013b838c = &global;
    CGameStateOwner_013b8394 state; g_pGameState_013b8394 = &state;
    CDisplayManager display; g_pDisplayDevice = &display;
    g_pNewFrontendGameComponent = &c;
    g_log[0] = 0;
    c.Shutdown();
    bool ok = std::strcmp(g_log, "DefDestroy;Set0;Scene;S16;S18;R1;R2;XMV;Erase;Erase;Clear;Swap;") == 0;
    ok = ok && c.m_pFrontEndDef.Object == 0 && def.RefCount == 0 && g_pNewFrontendGameComponent == 0 && global.Flag38;
    ok = ok && c.FileNames._M_finish == names && c.FileTimes._M_finish == times;
    def.RefCount = 2; c.m_pFrontEndDef.Object = &def; c.m_bCreateGame = true; g_pDisplayDevice = 0;
    g_log[0] = 0;
    c.Shutdown();
    ok = ok && std::strcmp(g_log, "Set0;Scene;XMV;Erase;Erase;") == 0 && def.RefCount == 1 && c.m_pFrontEndDef.Object == 0;
    if (!ok) { std::printf("0042ebb6_TEST FAIL\n"); return 1; }
    std::printf("0042ebb6_TEST PASS\n");
    return 0;
}
