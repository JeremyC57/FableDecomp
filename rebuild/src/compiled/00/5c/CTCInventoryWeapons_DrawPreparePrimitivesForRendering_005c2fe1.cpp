// CTCInventoryWeapons::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the inventory is open, advances every UI component by the time since the last render and draws it (skipping the draw on the first render frame)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2FE1, length 118 bytes (`ret 4`). Vtable 0x01253FF4 slot 11.
// Frame-pointer size TU: optimize("y",off) + ("s",on) (`push ecx ; fstp [esp]` float argument, one shared zero register).
// Ego_r.pdb: ?DrawPreparePrimitivesForRendering@CTCInventoryWeapons@@UAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees: GFGetTime() @ 0x009E1BC0 (`__fastcall`, double) twice, component virtual slot 1 (+4, Update(float)) and slot 2
// (+8, Render(viewport, 0, 0, 0, 0)).
// Shape: the frame delta is a float local ([ebp-4]) taken before LastRenderTime is reset from a second GFGetTime call; the loop
// index is unsigned (`jb`) against `Components.size()` recomputed each pass; `*(begin() + i)` re-reads the start word.
// Layout (retail CTCInventoryBase, Ego_r.pdb less the 4 bytes of the 16-byte debug vector): Components +0x0C,
// FirstRenderFrame +0x18, LastRenderTime +0x50, InventoryOpen +0x58; CEnginePrimitivePrepareInfo viewport word +0x34.
#pragma optimize("y", off)
#pragma optimize("s", on)
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

struct CTCBase_Draw {                                          // the polymorphic root (vptr +0x00, ParentThing +0x04, flags +0x08)
    virtual void v00();
    unsigned char    _pad04[0x08];
};
struct CTCInventoryWeapons : CTCBase_Draw {                    // a vptr declared next to the double would be padded to 8
    CComponentVector Components;                               // +0x0c
    bool             FirstRenderFrame;                         // +0x18
    unsigned char    _pad19[0x50 - 0x19];
    double           LastRenderTime;                           // +0x50
    bool             InventoryOpen;                            // +0x58
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCInventoryWeapons::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
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
    }
}