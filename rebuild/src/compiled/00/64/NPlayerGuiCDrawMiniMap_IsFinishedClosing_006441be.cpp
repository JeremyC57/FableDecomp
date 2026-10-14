#pragma optimize("s", on)
// NPlayerGui::CDrawMiniMap::IsFinishedClosing() -- true unless the map is still active with the screen faded out, the world showing the mini map on region load and the fade active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006441BE, length 56 bytes.
// Callees: CDisplayEngine::IsScreenFullyFadedOut @ 0x004A9210 (on the component's display engine, `[Component+28h]`), CWorld::IsDisplayingMiniMapOnRegionLoad
// @ 0x0049E9F0 (on CDrawBase::World +0xc).
// Layout: as IsFinishedOpening @ 0x00644179 -- the retail offsets are 0x20 below the engine header's PDB layout (Active +0xe9, FadeInOut +0x100), so
// the class is modelled locally with the PDB names. Shape: four tests in source order through `&&`, `xor al,al` / `mov al,1` results.
struct CDisplayEngine {
    bool IsScreenFullyFadedOut();                  // 0x004A9210
};
struct CMainGameComponent {
    unsigned char   _pad[0x28];
    CDisplayEngine* m_pDisplayEngine;              // +0x28
};
struct CWorld {
    bool IsDisplayingMiniMapOnRegionLoad();        // 0x0049E9F0
};
struct CFadeInFadeOutBase {
    unsigned char Active;                          // +0x00
    unsigned char _rest[0x1b];
};

namespace NPlayerGui {

struct CDrawMiniMap {
    unsigned char       _pad00[0x0c];              // vptr, PlayerGui, PlayerManager
    CWorld*             World;                     // +0x0c (CDrawBase)
    CMainGameComponent* Component;                 // +0x10 (CDrawBase)
    unsigned char       _pad14[0xe8 - 0x14];       // Bank, Drawing, PrimitiveHandles, MiniMap
    unsigned char       Enabled;                   // +0xe8
    unsigned char       Active;                    // +0xe9
    unsigned char       OldActive;                 // +0xea
    unsigned char       RegionChange;              // +0xeb
    unsigned char       Alpha;                     // +0xec
    unsigned char       _padED[0x100 - 0xed];      // ScreenBox
    CFadeInFadeOutBase  FadeInOut;                 // +0x100
    bool IsFinishedClosing();
};

bool CDrawMiniMap::IsFinishedClosing()
{
    if (Active != 0 && Component->m_pDisplayEngine->IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad() && FadeInOut.Active != 0)
        return false;
    return true;
}

}