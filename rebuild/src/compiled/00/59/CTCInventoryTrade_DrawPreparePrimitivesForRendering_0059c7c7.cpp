// CTCInventoryTrade::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the inventory is open: advance every UI component by the time since the last render and draw it (skipping the first render frame), accumulate the fade time and draw the backdrop at the fade fraction of the front end's fade time (clamped to 1)
// Leaf: ?DrawPreparePrimitivesForRendering@CTCInventoryTrade@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059C7C7, 231 bytes, thiscall, `ret 4`, ebp frame.
// Identity (proven): slot 11 of vtable 0x01253034 (RTTI .?AVCTCInventoryTrade@@); Ego_r.pdb ?DrawPreparePrimitivesForRendering@CTCInventoryTrade@@...;
//   AccumFadeTime (Ego_r.pdb +0x278) is the float at retail +0x228. The manifest's CTCInventory label is propagated.
// Callees: GFGetTime() 0x009E1BC0 (`__fastcall`, double) twice, component slots 1 Update(float) / 2 Render(viewport, 0, 0, 0, 0),
//   NUISystem::CManager::GetInstance 0x0041E5F2 and its slot 42 (+0xA8, the reference-counted fade settings by hidden
//   pointer; FadeTime +0x234), CTCInventoryBase::RenderUIBackdropQuad(float) 0x005BC6D6.
// Shape (family M139, representative 0x0056F090): the landed CTCInventoryQuests 0x0061B3C0 body without the information-screen
//   branch, `optimize("y", off)` + `optimize("s", on)`; the fade settings temporary is homed in the dead deltaTime word and released through its
//   inline `DecRef(); if (GetRefCount() == 0) Destroy();` (no clearing store: a temporary); the fraction lives in the dead info slot.
// Header debt: CTCInventoryBase / the inventory TC modelled locally (the landed sibling's layout; AccumFadeTime by retail offset).
#pragma optimize("y", off)
#pragma optimize("s", on)
double __fastcall GFGetTime();                               // 0x009E1BC0
struct CViewport;
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CViewport* PViewport;                                    // +0x34
};
namespace NUISystem {
class CComponent { public:
    virtual void v00();
    virtual void Update(float deltaTime);                    // slot 1 (+4)
    virtual void Render(CViewport* viewport, long a, long b, long c, long d);   // slot 2 (+8)
};
}
struct CComponentVector {                                    // STLport vector<NUISystem::CComponent*>
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
struct CFadeSettings {
    virtual void v00();
    virtual void Destroy();                                  // slot 1 (+4)
};
struct CFadeSettingsObject : CFadeSettings, CRefCount {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
    unsigned char _pad08[0x234 - 0x08];
    float FadeTime;                                          // +0x234
};
struct CFadeSettingsPtr {
    CFadeSettingsObject* Object;
    ~CFadeSettingsPtr()
    {
        if (Object)
            Object->ReleaseTemporary();
    }
    CFadeSettingsObject* operator->() const { return Object; }
};
namespace NUISystem {
class CManager { public:
    static CManager* __fastcall GetInstance();      // 0x0041E5F2
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual CFadeSettingsPtr GetFadeSettings();             // slot 42 (+0xA8)
};
}
struct CTCBase_Draw {
    virtual void v00();
    unsigned char _pad04[0x08];
};
struct CTCInventoryBase : CTCBase_Draw {
    CComponentVector Components;                             // +0x0c
    bool             FirstRenderFrame;                       // +0x18
    unsigned char    _pad19[0x50 - 0x19];
    double           LastRenderTime;                         // +0x50
    bool             InventoryOpen;                          // +0x58 (the base pads to 0x60: double alignment)
    void RenderUIBackdropQuad(float alpha);                  // 0x005BC6D6
};
class CTCInventoryTrade : public CTCInventoryBase { public:
    unsigned char    _pad60[0x228 - 0x60];
    float AccumFadeTime;                                     // +0x228
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};
void CTCInventoryTrade::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (InventoryOpen) {
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
        float alpha = AccumFadeTime / NUISystem::CManager::GetInstance()->GetFadeSettings()->FadeTime;
        if (alpha > 1.0f)
            alpha = 1.0f;
        RenderUIBackdropQuad(alpha);
    }
}