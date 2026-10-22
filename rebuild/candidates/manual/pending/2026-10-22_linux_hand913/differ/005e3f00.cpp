// Grade: DIFFER(295v295) -- retail loads PBlack (mov ecx,[esi+0BCh]) before the fstp qword [esi+48h] of LastRenderTime; ours stores first (scheduling only)
// CTCQuestCompletionUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while active and not quitting: advance and draw the black backdrop component and every UI component by the time since the last render, accumulate the fade time and draw the user-profile UI background at the fade fraction (clamped to 1)
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E3F00, 295 bytes (`ret 4`, esp frame). Vtable 0x0125589C (CTCQuestCompletionUI) slot 11.
// Callees: GFGetTime() @ 0x009E1BC0 (`__fastcall`, double) twice, component slots 1 Update(float) / 2 Render(viewport, 0, 0, 0, 0),
// NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 (static `__fastcall`) and its slot 42 (+0xA8, a reference-counted object by
// hidden pointer; float at +0x234), CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0, CUserProfileManager::RenderUIBackground
// (float) @ 0x004075C0; the front-end object's slot 1 on the last release. Float pool 0x0122DED8 = 1.0f.
// Layout: retail CTCQuestCompletionUI as the landed constructor 0x005E4030: Active +0x0C, Components +0x10 (12-byte STLport vector),
// Quitting +0x1C, LastRenderTime +0x48, AccumFadeTime +0xB4, PBlack +0xBC.
// Shape: the landed sibling CTCInventoryQuests::DrawPreparePrimitivesForRendering 0x0061B3C0 (the delta as a float local kept as raw
// bits in ebx, the index loop over `*(begin() + i)`, the front-end object released through its own inline ReleaseTemporary).
double __fastcall GFGetTime();                                 // 0x009E1BC0

struct CThing;
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

class CUserProfileManager {
public:
    void RenderUIBackground(float alpha);                      // 0x004075C0
};
CUserProfileManager& __fastcall GetUserProfileManager();       // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)

struct CTCBase {
    virtual void v00();
    CThing*       ParentThing;                                 // +0x04
    bool          ToBeRemoved;                                 // +0x08
    bool          IsInGlobalUpdateTCs;                         // +0x09
};

struct CTCQuestCompletionUI : CTCBase {                        // vtable 0x0125589C
    bool                   Active;                             // +0x0c
    bool                   ReallyActive;                       // +0x0d
    CComponentVector       Components;                         // +0x10
    bool                   Quitting;                           // +0x1c
    unsigned char          _pad1d[0x48 - 0x1d];
    double                 LastRenderTime;                     // +0x48
    unsigned char          _pad50[0xb4 - 0x50];
    float                  AccumFadeTime;                      // +0xb4
    NUISystem::CComponent* PFrame;                             // +0xb8
    NUISystem::CComponent* PBlack;                             // +0xbc
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCQuestCompletionUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (Active && !Quitting) {
        float deltaTime = (float)(GFGetTime() - LastRenderTime);
        LastRenderTime = GFGetTime();
        PBlack->Update(deltaTime);
        PBlack->Render(info.PViewport, 0, 0, 0, 0);
        for (unsigned long i = 0; i < Components.size(); ++i) {
            Components[i]->Update(deltaTime);
            Components[i]->Render(info.PViewport, 0, 0, 0, 0);
        }
        AccumFadeTime += deltaTime;
        float alpha = AccumFadeTime / NUISystem::CFrontEndManager::GetInstance()->GetFadeSettings()->FadeTime;
        if (alpha > 1.0f)
            alpha = 1.0f;
        GetUserProfileManager().RenderUIBackground(alpha);
    }
}
