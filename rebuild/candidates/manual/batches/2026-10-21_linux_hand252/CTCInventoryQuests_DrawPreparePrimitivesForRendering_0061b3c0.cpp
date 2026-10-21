// CTCInventoryQuests::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the inventory is open: on the information screen, clear the quest-card select primitives and draw the inventory frame and the active test quests; otherwise advance every UI component by the time since the last render and draw it (skipping the first render frame), accumulate the fade time and draw the backdrop at the fade fraction of the front end's fade time (clamped to 1)
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061B3C0, 368 bytes (+ int3 padding), `ret 4`, esp frame (speed TU).
//   Vtable slot 11 (manifest "vfunc_11"); Ego_r.pdb `?DrawPreparePrimitivesForRendering@CTCInventoryQuests@@UAEX...`.
// Callees: GFGetTime() @ 0x009E1BC0 (`__fastcall`, double) twice, component slots 1 Update(float) / 2 Render(viewport,
//   0, 0, 0, 0) (as the landed sibling CTCInventoryWeapons 0x005C2FE1), NUISystem::CFrontEndManager::GetInstance
//   @ 0x0041E5F2 (static `__fastcall`) and its slot 42 (+0xA8, a reference-counted object by hidden pointer; float
//   at +0x234), CTCInventoryBase::RenderUIBackdropQuad(float) @ 0x005BC6D6, the NHeroInformationScreens::CBase
//   (+0x130) slot 3 (+0x0C, bool), std::_For_each<CEnginePrimitiveHandle*, mem_fun_ref_t<void, CEnginePrimitiveHandle> >
//   @ 0x0043F360 (`__fastcall`, functor by hidden pointer; label "CAddDefNamesToComboBox" is propagated) with
//   CEnginePrimitiveHandle::Clear @ 0x0041BE70, vector::erase(first, last) @ 0x00440D70, CTCInventoryBase 0x005BC4D8
//   (info, bool; label "$E2"), CTCInventoryQuests::DrawActiveTestQuests @ 0x0061AD60.
// Shape: the front-end object is released through its own inline ReleaseTemporary (`DecRef(); if (GetRefCount() == 0)
//   Destroy();` on the CRefCount base at +4) -- the count is loaded, decremented and stored through edi with the
//   `add eax,4` base pointer; a plain `--RefCount == 0` gives `dec [eax+4]`.
// Layout: retail CTCInventoryBase as the landed sibling 0x005C2FE1 (Components +0x0C, FirstRenderFrame +0x18,
//   LastRenderTime +0x50, InventoryOpen +0x58); NHeroInformationScreens::CBase at +0x130;
//   InventoryQuestCardSelectPrimitiveHandles +0x134, AccumFadeTime +0x184 (Ego_r.pdb order, release containers).
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

struct CEnginePrimitiveHandle {
    long Handle;
    void Clear();                                              // 0x0041BE70
};
template <class R, class T> struct mem_fun_ref_t {
    R (T::*_M_f)();
    explicit mem_fun_ref_t(R (T::*f)()) : _M_f(f) { }
    R operator()(T& r) const { return (r.*_M_f)(); }
};
template <class R, class T> inline mem_fun_ref_t<R, T> mem_fun_ref(R (T::*f)()) { return mem_fun_ref_t<R, T>(f); }
template <class I, class F> F __fastcall _For_each(I first, I last, F f);   // 0x0043F360
template <class I, class F> inline F for_each(I first, I last, F f) { return _For_each(first, last, f); }
struct CPrimitiveHandleVector {                                // STLport vector<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void clear() { erase(begin(), end()); }
};

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
    void DrawInventoryFrame(const CEnginePrimitivePrepareInfo& info, bool informationScreen);   // 0x005BC4D8
};
namespace NHeroInformationScreens {
struct CBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool IsActive();                                   // slot 3 (+0x0C)
};
}
struct CTCInventoryQuests : CTCInventoryBase, NHeroInformationScreens::CBase {
    CPrimitiveHandleVector InventoryQuestCardSelectPrimitiveHandles;   // +0x134
    unsigned char    _pad140[0x184 - 0x140];
    float            AccumFadeTime;                            // +0x184
    void DrawActiveTestQuests(const CEnginePrimitivePrepareInfo& info);   // 0x0061AD60
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCInventoryQuests::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (InventoryOpen) {
        if (!IsActive()) {
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
        } else {
            for_each(InventoryQuestCardSelectPrimitiveHandles.begin(), InventoryQuestCardSelectPrimitiveHandles.end(),
                     mem_fun_ref(&CEnginePrimitiveHandle::Clear));
            InventoryQuestCardSelectPrimitiveHandles.clear();
            DrawInventoryFrame(info, IsActive());
            DrawActiveTestQuests(info);
        }
    }
}
