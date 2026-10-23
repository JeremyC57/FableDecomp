// Grade: DIFFER(626v645): register allocation -- ours caches a zero in ebx for the whole function (pushes/stores of 0, cmp al,bl), retail materialises it only in the handle-release loops and keeps the player GUI/font in ebx/ebp across the three GetText calls
// CTCBetting::FrameUpdate() -- follow the global betting flag: on start enter the betting player mode, show the betting instructions (three text-bank lines) and announce it with a betting game event, then schedule the start; on stop leave the mode, announce it, and release every bet and money primitive handle
// Leaf: FrameUpdate
// Flags: /GS
// Retail 0x007E4E30, 656 bytes, `ret`, /GS cookie. Callees: CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::
// GetMainPlayer 0x00449970, CPlayer::GetCurrentMode 0x00633BE0 / AddMode 0x006345C0 / RemoveMode 0x00634190,
// GetSubDef<CBettingDef> 0x007E5820, CWideString(const wchar_t*) 0x0099B6B0 / dtor 0x0099B510, CTCBetting::GetText
// 0x007E4D70, CSubtitleRenderer::SetText (wide) 0x00492F69, CMemoryBuffer::Copy 0x009F1760, CMainGameComponent::
// AddGameEvent 0x0041649C and frame getter 0x00416225 (g_pMainGameComponent 0x013B86A0). Globals: betting-active flag
// 0x013BAE1D, the bet 0x013BAE20, start delay seconds 0x01384050, frames per second 0x01375550.
// Layout: retail CTCBetting (landed 0x007E57C0): Active +0xc, BetHandles +0x10, MoneyHandles +0x1c, FrameToStart +0x34;
// CGameEvent as the landed 0x00779080; retail CBettingDef text ids +0x4c/+0x50/+0x54.
struct CThing;
struct CFontBank;
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CWideString& text, const CWideString& button1, const CWideString& button2,
                 const CWideString& button3, unsigned long colour, float time, bool a, bool b);   // 0x00492F69
};
struct CPlayerGui {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBOOL(bool value) { Copy(1, &value); }
};
struct CMainGameComponent {
    unsigned char _pad00[0x1614c];
    CFontBank*    Font;                                 // +0x1614C
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
    long GetFrame();                                    // 0x00416225
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
extern bool g_BettingActive;                            // 0x013BAE1D
extern long g_CurrentBet;                               // 0x013BAE20
extern long g_BettingStartDelay;                        // 0x01384050
extern long g_framesPerSecond;                          // 0x01375550

struct CPlayer {
    long GetCurrentMode() const;                        // 0x00633BE0
    void AddMode(long mode, long param);                // 0x006345C0
    void RemoveMode(long mode);                         // 0x00634190
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CBettingDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x4c - 0x08];
    long          InstructionText;                      // +0x4c
    long          Button2Text;                          // +0x50
    long          Button3Text;                          // +0x54
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CBettingDef>* out);      // 0x007E5820
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
// Stand-in body (the real template instance @ 0x007E5820 is out of line): it writes the out slot only, so VC7.1 knows
// the def pointer's address does not escape the call (the landed 0x008C8380 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CBettingDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* m_pDef;                              // +0x70
};
struct CEnginePrimitive {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    virtual void v02();
    virtual void RemoveHandle(void* handle);            // slot 3
    long RefCount;                                      // +0x04
};
struct CEnginePrimitiveHandle {
    void*             __vftable;                        // +0x00
    CEnginePrimitive* PPrimitive;                       // +0x04
    void Release()
    {
        if (PPrimitive && PPrimitive->RefCount == 1)
            PPrimitive->RemoveHandle(this);
        if (PPrimitive) {
            if (--PPrimitive->RefCount == 0)
                PPrimitive->Destroy();
            PPrimitive = 0;
        }
        PPrimitive = 0;
    }
};
struct CRetailHandleVector {
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
};
struct CTCBetting {
    void*               __vftable;                      // +0x00
    CThing*             ParentThing;                    // +0x04
    unsigned char       _pad08[0x4];
    bool                Active;                         // +0x0c
    CRetailHandleVector BetHandles;                     // +0x10
    CRetailHandleVector MoneyHandles;                   // +0x1c
    long                FrameBetWasLastChanged;         // +0x28
    long                FrameBetChangeStarted;          // +0x2c
    long                Mult;                           // +0x30
    long                FrameToStart;                   // +0x34
    long                PreviousDonation;               // +0x38
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    CWideString GetText(long id);                       // 0x007E4D70
    void FrameUpdate();
};

void CTCBetting::FrameUpdate()
{
    if (Active == g_BettingActive)
        return;
    Active = g_BettingActive;
    CPlayer* player = GetPlayerManager()->GetMainPlayer();
    if (Active) {
        if (player->GetCurrentMode() != 0x27)
            player->AddMode(0x27, 0);
        g_CurrentBet = 0;
        CDefPointer<CBettingDef> def;
        if (ParentThing->m_pDef->GetSubDef(&def)) {
            g_pPlayerGui->SubtitleRenderer.SetText(g_pMainGameComponent->Font, GetText(def->InstructionText),
                (const unsigned short*)L"", GetText(def->Button2Text), GetText(def->Button3Text), 0x404, 0.0f, false, true);
            CGameEvent event(9);
            event.AddBOOL(true);
            g_pMainGameComponent->AddGameEvent(event);
        }
        FrameToStart = g_pMainGameComponent->GetFrame() + g_BettingStartDelay * g_framesPerSecond;
    } else {
        if (player->GetCurrentMode() == 0x27) {
            player->RemoveMode(0x27);
            CGameEvent event(9);
            event.AddBOOL(false);
            g_pMainGameComponent->AddGameEvent(event);
        }
        for (CEnginePrimitiveHandle* handle = BetHandles.m_pFirst; handle != BetHandles.m_pLast; ++handle)
            handle->Release();
        for (CEnginePrimitiveHandle* handle = MoneyHandles.m_pFirst; handle != MoneyHandles.m_pLast; ++handle)
            handle->Release();
    }
}
