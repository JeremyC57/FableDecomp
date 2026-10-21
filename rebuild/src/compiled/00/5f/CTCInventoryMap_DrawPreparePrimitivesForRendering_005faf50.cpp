// CTCInventoryMap::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- with a quest waiting to be dropped, leave the player's current mode, close the hero-information screens and queue game event 9 (one zero byte); otherwise, while the inventory is open and the information screen is not active, advance and draw every UI component, accumulate the fade time and draw the backdrop at the fade fraction (clamped to 1)
// Leaf: DrawPreparePrimitivesForRendering
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FAF50, 439 bytes, `ret 4`, esp frame (speed TU) with the /GS cookie
//   (0x0139C8A8, checked by 0x00BFE9F9 on both exits) guarding the game event's 32-byte data array. Vtable ref 0x01257AC8.
// Callees: CPlayerManager::GetPlayer(long) @ 0x004498C0 (engine 0x013B86A0 +0x1C), CPlayer::GetCurrentMode @ 0x00633BE0 /
//   RemoveMode @ 0x00634190, CPlayerGui::GetPHeroInfoScreensManager @ 0x00437CD0 (global 0x013B8790) and the manager's
//   close @ 0x0066E73D (name unconfirmed, as the landed 0x005BC527 family), CMemoryBuffer::Copy @ 0x009F1760,
//   CMainGameComponent::AddGameEvent @ 0x0041649C, and the else arm's callees exactly as the landed sibling
//   CTCInventoryQuests::DrawPreparePrimitivesForRendering 0x0061B3C0 (GFGetTime 0x009E1BC0, component slots 1/2,
//   CFrontEndManager::GetInstance 0x0041E5F2 + slot 42, RenderUIBackdropQuad 0x005BC6D6).
// Shape: the quests sibling's body as an `else if (InventoryOpen && !IsActive())`; the event is the inline CGameEvent(type) ctor
//   (as 0x005BC527) plus an inline AddUBYTE whose by-value byte VC homes in the dead `info` argument slot.
// Layout: retail CTCInventoryBase as the landed siblings (Components +0x0C, FirstRenderFrame +0x18, LastRenderTime +0x50,
//   InventoryOpen +0x58), NHeroInformationScreens::CBase at +0x130; CTCInventoryMap AccumFadeTime +0x140 and QuestToBeDropped
//   +0x1A4 (Ego_r.pdb +0x15C / +0x1DC with 16-byte containers; retail STLport containers are 12 bytes).
double __fastcall GFGetTime();                                 // 0x009E1BC0

struct CViewport;
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CViewport* PViewport;                                      // +0x34
};
namespace NUISystem {
struct CComponent {
    virtual void v00();
    virtual void Update(float deltaTime);                      // slot 1 (+4)
    virtual void Render(CViewport* viewport, long a, long b, long c, long d);   // slot 2 (+8)
};
}
struct CComponentVector {                                      // STLport vector<NUISystem::CComponent*>
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent** begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return *(begin() + n); }
};

struct CRefCount {
    long RefCount;
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
};
struct CFadeSettings {                                         // reference-counted front-end object
    virtual void v00();
    virtual void Destroy();                                    // slot 1 (+4)
};
struct CFadeSettingsObject : CFadeSettings, CRefCount {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    unsigned char _pad08[0x234 - 0x08];
    float FadeTime;                                            // +0x234
};
struct CFadeSettingsPtr {
    CFadeSettingsObject* Object;
    ~CFadeSettingsPtr()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CFadeSettingsObject* operator->() const { return Object; }
};
namespace NUISystem {
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();        // 0x0041E5F2
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41();
    virtual CFadeSettingsPtr GetFadeSettings();               // slot 42 (+0xA8)
};
}

struct CTCBase_Draw {                                          // the polymorphic root (vptr +0x00, ParentThing +0x04, flags +0x08)
    virtual void v00();
    unsigned char    _pad04[0x08];
};
struct CTCInventoryBase : CTCBase_Draw {
    CComponentVector Components;                               // +0x0c
    bool             FirstRenderFrame;                         // +0x18
    unsigned char    _pad19[0x50 - 0x19];
    double           LastRenderTime;                           // +0x50
    bool             InventoryOpen;                            // +0x58
    unsigned char    _pad59[0x130 - 0x59];
    void RenderUIBackdropQuad(float alpha);                    // 0x005BC6D6
};
namespace NHeroInformationScreens {
struct CBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool IsActive();                                   // slot 3 (+0x0C)
};
struct CManager {
    void Close();                                              // 0x0066E73D (name unconfirmed)
};
}
struct CPlayerGui {
    NHeroInformationScreens::CManager* GetPHeroInfoScreensManager() const;   // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                               // 0x013B8790

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);         // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                        // +0x00
    signed char   Player;                                      // +0x04
    unsigned char Data[32];                                    // +0x05
    unsigned char EndPos;                                      // +0x25
    bool          Valid;                                       // +0x26
    bool          Replacement;                                 // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};
struct CPlayer {
    long GetCurrentMode();                                     // 0x00633BE0
    void RemoveMode(long mode);                                // 0x00634190
};
struct CPlayerManager {
    CPlayer* GetPlayer(long index);                            // 0x004498C0
};
struct CMainGameComponent {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                             // +0x1C
    void AddGameEvent(const CGameEvent& event);                // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;               // 0x013B86A0

struct CTCInventoryMap : CTCInventoryBase, NHeroInformationScreens::CBase {
    unsigned char    _pad134[0x140 - 0x134];                   // VisitedRegions (12-byte STLport vector)
    float            AccumFadeTime;                            // +0x140
    unsigned char    _pad144[0x1a4 - 0x144];
    long             QuestToBeDropped;                         // +0x1A4
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCInventoryMap::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (QuestToBeDropped > 0) {
        CPlayer* player = g_pMainGameComponent->PlayerManager->GetPlayer(0);
        player->RemoveMode(player->GetCurrentMode());
        g_pPlayerGui->GetPHeroInfoScreensManager()->Close();
        CGameEvent event(9);
        event.AddUBYTE(0);
        g_pMainGameComponent->AddGameEvent(event);
    } else if (InventoryOpen && !IsActive()) {
        float deltaTime = (float)(GFGetTime() - LastRenderTime);
        LastRenderTime = GFGetTime();
        for (unsigned long i = 0; i < Components.size(); ++i) {
            Components[i]->Update(deltaTime);
            if (FirstRenderFrame)
                FirstRenderFrame = false;
            else
                Components[i]->Render(info.PViewport, 0, 0, 0, 0);
        }
        AccumFadeTime += deltaTime;
        float alpha = AccumFadeTime / NUISystem::CFrontEndManager::GetInstance()->GetFadeSettings()->FadeTime;
        if (alpha > 1.0f)
            alpha = 1.0f;
        RenderUIBackdropQuad(alpha);
    }
}