#pragma optimize("s", on)
// NPlayerGui::CDrawMiniMap::IsFinishedOpening() -- true unless the map is opening during a region change: then the fade must report open (the screen must be faded out, the world must be showing the mini map on region load and the fade must be active)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00644179, length 69 bytes.
// Callees: CDisplayEngine::IsScreenFullyFadedOut @ 0x004A9210 (on the component's display engine, `[Component+28h]`), CWorld::IsDisplayingMiniMapOnRegionLoad
// @ 0x0049E9F0 (on CDrawBase::World +0xc), CFadeInFadeOutBase::IsOpen @ 0x0048DA30 (sibling call on FadeInOut +0x100).
// Layout: the engine header CDrawMiniMap.h (PDB) places Enabled/Active/OldActive/RegionChange at +0x108..+0x10b and FadeInOut at +0x120, but the
// retail bytes are 0x20 lower (Active +0xe9, RegionChange +0xeb, FadeInOut +0x100 -- the landed SetAsActive @ 0x00643E2E and ClearPrimitivesIfNecessary
// use the same offsets: the CMiniMapDisplay member is smaller in the shipped build), so the class is modelled locally with the PDB names.
// Shape: five tests in source order through `&&`, the fade addressed once (`lea ecx,[esi+100h]`) for its Active byte and the tail call; the
// early-out result is `mov al,1`.
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
    bool IsOpen();                                 // 0x0048DA30
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
    bool IsFinishedOpening();
};

bool CDrawMiniMap::IsFinishedOpening()
{
    if (RegionChange != 0 && Active != 0 && Component->m_pDisplayEngine->IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad()) {
        CFadeInFadeOutBase* fade = &FadeInOut;
        if (fade->Active != 0)
            return fade->IsOpen();
    }
    return true;
}

}