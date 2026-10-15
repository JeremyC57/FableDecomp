// CTCBoastUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) -- while the boast UI is active and not quitting, let every UI component prepare with the info's first object and then draw with its +0x34 object
// Grade: DIFFER(104v104) -- loop shape: retail spills the hoisted info.Renderer into the dead `info` argument slot (`mov [esp+14h],eax`) as well as ebp and reads the element at the loop top each pass (1-byte nop alignment); ours keeps it only in ebp and rotates the first element load before the loop (`jmp` + 6-byte pad) | tried: renderer local (best, this file), info.Renderer read per call, 4-byte struct by value, const member
// Leaf: ?DrawPreparePrimitivesForRendering@CTCBoastUI@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056A8C0, length 104 bytes (+8 int3 padding). `ret 4`.
// Callees: each NUISystem::CComponent's virtual slot 1 (+4, one argument) and slot 2 (+8, five arguments: the +0x34 object
// and four zeros).
// Shape: `for (unsigned i = 0; i < Components.size(); ++i)` with operator[] as `*(begin() + i)` (the element re-read for the
// second call, size() recomputed at the foot); the info's first word is a local kept in ebp (also spilled to the dead argument
// slot).
// Layout: engine/CTCBoastUI.h (Ego_r.pdb) with the retail 12-byte map and vector (header 4 bytes off): Active +0x18,
// Components +0x1C, Quitting +0x28 (as the landed Deactivate 0x0056A880).
struct CEngineRenderer;
struct CEngineCamera;
struct CEnginePrimitivePrepareInfo {
    CEngineRenderer* Renderer;                  // +0x00
    unsigned char _pad04[0x30];
    CEngineCamera* Camera;                      // +0x34
};
namespace NUISystem {
struct CComponent {
    virtual void v00();
    virtual void PreparePrimitives(CEngineRenderer* renderer);                       // slot 1 (+4)
    virtual void DrawPrimitives(CEngineCamera* camera, long a, long b, long c, long d);   // slot 2 (+8)
};
}
struct CComponentVector {                       // STLport vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent** begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    NUISystem::CComponent* operator[](unsigned int n) const { return *(begin() + n); }
};

struct CTCBoastUI {
    virtual void v00();
    unsigned char _pad04[0x18 - 0x04];          // CTCBase, BoastGroups (12-byte map)
    bool Active;                                // +0x18 (PDB +0x1c)
    unsigned char _pad19[3];
    CComponentVector Components;                // +0x1c (PDB +0x20)
    bool Quitting;                              // +0x28 (PDB +0x30)
    bool AllowedToOpen;                         // +0x29
    bool ActivationRequested;                   // +0x2a
    bool ActivateLater;                         // +0x2b
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCBoastUI::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (Active && !Quitting) {
        CEngineRenderer* renderer = info.Renderer;
        for (unsigned int i = 0; i < Components.size(); ++i) {
            Components[i]->PreparePrimitives(renderer);
            Components[i]->DrawPrimitives(info.Camera, 0, 0, 0, 0);
        }
    }
}
