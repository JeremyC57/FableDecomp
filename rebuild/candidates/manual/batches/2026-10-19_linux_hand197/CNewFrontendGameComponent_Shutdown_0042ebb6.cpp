// CNewFrontendGameComponent::Shutdown() -- tear the front end down: drop the front-end def, unregister the component, release the scene, hand the game state back unless a game is being created, unload the XMV code, empty the save-file lists and clear the screen to opaque black
// Leaf: Shutdown
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042EBB6, 198 bytes (`ret`). Size-optimised TU with a frame pointer: `#pragma optimize("y",
//   off)` + `#pragma optimize("s", on)` (as the landed CNewFrontendGameComponent rows of this module).
// Callees: the __fastcall registration forwarder 0x004336B0 (pushes ecx and calls 0x00433D3A on the object at 0x013B8760; called
//   with 0), the component's scene release 0x0042DD28 and ReleaseXMVCode 0x0042DBD8, the game state's (0x013B8394) virtual slots
//   16 (+0x40) and 18 (+0x48, with 0) and its calls 0x00991750 / 0x009918F0, vector<CWideString>::erase(first, last) 0x00412450 and
//   vector<CDateAndTime>::erase 0x00411B00 (out of line in /Os), CDisplayManager::ClearRenderTarget(const CRGBColour&, unsigned
//   long) 0x009BE420 and SwapScreens 0x009BEEB0 on g_pDisplayDevice (0x013B8390); globals 0x013B871C (the live component) and
//   0x013B838C (an object whose +0x38 flag is raised).
// Layout = Ego_r.pdb CNewFrontendGameComponent: m_bCreateGame +0x29, m_pFrontEndDef +0x3c (CDefPointer: count at +4, slot 1
//   destroys), FileNames +0x6c, FileTimes +0x78 (STLport 12-byte vectors; PDB +0x7c).
// Shape: `m_pFrontEndDef = NULL` is CDefPointer::operator=(T*) = Clear() (release, null) then the store; clear() is
//   erase(begin(), end()) pushing last then first; the colour is a CRGBColour local built R, G, B, A.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CFrontEndDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    long RefCount;                                 // +0x04
    __forceinline void ReleaseRef() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    __forceinline void Clear() { if (Object) { Object->ReleaseRef(); Object = 0; } }
    __forceinline CDefPointer& operator=(T* p) { Clear(); Object = p; return *this; }
};
template <class T> struct vector {                 // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last);                   // out of line
    __forceinline void clear() { erase(_M_start, _M_finish); }
};
struct CWideString { void* m_pData; };
struct CDateAndTime { unsigned char _body[0x10]; };
struct CRGBColour {
    unsigned char B, G, R, A;
    __forceinline CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CDisplayManager {
public:
    void ClearRenderTarget(const CRGBColour& colour, unsigned long flags);   // 0x009BE420
    void SwapScreens();                            // 0x009BEEB0
};
class CGameStateOwner_013b8394 {                   // the object held at 0x013B8394
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void ResumeGame_slot16();              // slot 16 (+0x40)
    virtual void v17();
    virtual void SetPaused_slot18(bool paused);    // slot 18 (+0x48)
    void Restore_00991750();                       // 0x00991750
    void Restore_009918f0();                       // 0x009918F0
};
struct CFrontendGlobal_013b838c {
    unsigned char _pad0[0x38];
    bool          Flag38;                          // +0x38
};
class CNewFrontendGameComponent;
void __fastcall SetActiveFrontend_004336b0(CNewFrontendGameComponent* component);   // 0x004336B0
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C
extern CFrontendGlobal_013b838c* g_pFrontendGlobal_013b838c;    // 0x013B838C
extern CGameStateOwner_013b8394* g_pGameState_013b8394;          // 0x013B8394
extern CDisplayManager* g_pDisplayDevice;                        // 0x013B8390
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
    void ReleaseScene_0042dd28();                  // 0x0042DD28
    void ReleaseXMVCode();                         // 0x0042DBD8
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
