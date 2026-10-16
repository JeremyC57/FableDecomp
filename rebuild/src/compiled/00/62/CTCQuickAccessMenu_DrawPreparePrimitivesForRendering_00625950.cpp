// CTCQuickAccessMenu::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the menu is active and not quitting, advances every UI component by the time since the last render and draws it (skipping the draw on the first render frame)
// Leaf: DrawPreparePrimitivesForRendering
// Retail: Fable.exe 0x00625950, 132 bytes (`ret 4`). Vtable 0x01258B74 (CTCQuickAccessMenu) slot 11 (manifest "vfunc_11";
// slot 11 is DrawPreparePrimitivesForRendering as in the landed CTCInventoryWeapons 0x005C2FE1).
// Callees: GFGetTime() @ 0x009E1BC0 (`__fastcall`, double) twice, component virtual slot 1 (+4, Update(float)) and
// slot 2 (+8, Render(viewport, 0, 0, 0, 0)).
// Layout (landed ctor 0x006259E0: Ego_r.pdb less the 4 bytes of the 16-byte debug vector): Active +0x0C,
// Components +0x10, Quitting +0x1C, LastRenderTime +0x20, FirstRenderFrame +0x28; CEnginePrimitivePrepareInfo
// viewport word +0x34.
// Shape: speed TU (no frame pointer; the float delta lives in the `push ecx` word); unsigned index against
// Components.size() recomputed each pass; `*(begin() + i)` re-reads the start word; the draw is the if (!FirstRenderFrame) arm (falls through).
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
struct CComponentVector {                                      // STLport vector<NUISystem::CComponent*>, 12 bytes
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent** begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    NUISystem::CComponent*& operator[](unsigned long n) { return *(begin() + n); }
};
struct CTCBase_Draw {                                          // polymorphic root (vptr +0x00, ParentThing +0x04, flags +0x08)
    unsigned char _pad00[0x0c];
};
struct CTCQuickAccessMenu : CTCBase_Draw {                     // vtable 0x01258B74
    bool             Active;                                   // +0x0c
    CComponentVector Components;                               // +0x10
    bool             Quitting;                                 // +0x1c
    double           LastRenderTime;                           // +0x20
    bool             FirstRenderFrame;                         // +0x28
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCQuickAccessMenu::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (Active && !Quitting) {
        float deltaTime = (float)(GFGetTime() - LastRenderTime);
        LastRenderTime = GFGetTime();
        for (unsigned long i = 0; i < Components.size(); ++i) {
            Components[i]->Update(deltaTime);
            if (!FirstRenderFrame)
                Components[i]->Render(info.PViewport, 0, 0, 0, 0);
            else
                FirstRenderFrame = false;
        }
    }
}