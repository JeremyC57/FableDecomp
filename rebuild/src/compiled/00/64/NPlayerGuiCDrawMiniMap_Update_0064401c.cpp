// NPlayerGui::CDrawMiniMap::Update(const float& elapsed) -- while the screen is faded out for a region load that shows the mini map, open the map fade (remembering the old active flag) and close it once the world reaches load stage 8; otherwise restore the active flag after a region change, start the fade for an active map or close it for an inactive one; then step the fade and mirror the world's mini-map enable flag into SetAsActive
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064401C, length 347 bytes, `ret 4`.
// Callees: CDisplayEngine::IsScreenFullyFadedOut @ 0x004A9210 (on the component's display engine, `[Component+28h]`), CWorld::
// IsDisplayingMiniMapOnRegionLoad @ 0x0049E9F0, CWorld @ 0x0048DB13 (a bool query on the world; the manifest label "CPersistContext::IsBinary"
// is propagated), CFadeInFadeOutBase::Start(float, float, CRGBColour) @ 0x00434840 and ::Update(float) @ 0x00434870, CDrawMiniMap::
// SetAsActive(bool) @ 0x00643E2E (landed). Float pool: 0.5f @ 0x0122F59C.
// Layout: engine/CDrawMiniMap.h minus 0x20 (the landed SetAsActive / IsFinishedClosing): World +0x0c, Component +0x10, Enabled +0xe8,
// Active +0xe9, OldActive +0xea, RegionChange +0xeb, FadeInOut +0x100 (CFadeInFadeOutBase: Active +0, Closing +0xc, Opening +0xd).
// CWorld: RegionLoadStage +0x104, MiniMapEnabled +0xdd, MiniMapActive +0xde (names unverified).
// Shape: NPlayerGui module (`#pragma optimize("s", on)`); the by-value CRGBColour argument (user copy constructor) is built in its argument slot (stores R, G,
// B, A); the world pointer is named inside the third `&&` term; the fade close is the inline FadeOut() helper (Opening = false, Closing = true) of the landed SetAsActive.
#pragma optimize("s", on)
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    CRGBColour(const CRGBColour& other) : B(other.B), G(other.G), R(other.R), A(other.A) {}
};
class CFadeInFadeOutBase { public:
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0c
    bool       Opening;                            // +0x0d
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
    void Start(float fadeInTime, float fadeOutTime, CRGBColour toColour);   // 0x00434840
    void Update(float elapsed);                    // 0x00434870
    void FadeOut()
    {
        Opening = false;
        Closing = true;
    }
};
class CDisplayEngine { public:
    bool IsScreenFullyFadedOut();                  // 0x004A9210
};
class CMainGameComponent { public:
    unsigned char _pad[0x28];
    CDisplayEngine* m_pDisplayEngine;              // +0x28
};
class CWorld { public:
    unsigned char _pad[0xdd];
    bool MiniMapEnabled;                           // +0xdd
    bool MiniMapActive;                            // +0xde
    unsigned char _pad2[0x104 - 0xdf];
    long RegionLoadStage;                          // +0x104
    bool IsDisplayingMiniMapOnRegionLoad();        // 0x0049E9F0
    bool IsRegionLoadShowingMiniMap();             // 0x0048DB13 (label unverified)
};

struct CDrawMiniMap {
    unsigned char       _pad0[0x0c];
    CWorld*             World;                     // +0x0c
    CMainGameComponent* Component;                 // +0x10
    unsigned char       _pad1[0xe8 - 0x14];
    bool                Enabled;                   // +0xe8
    bool                Active;                    // +0xe9
    bool                OldActive;                 // +0xea
    bool                RegionChange;              // +0xeb
    unsigned char       _pad2[0x100 - 0xec];
    CFadeInFadeOutBase  FadeInOut;                 // +0x100
    void SetAsActive(bool active);                 // 0x00643E2E
    void Update(const float& elapsed);
};

void CDrawMiniMap::Update(const float& elapsed)
{
    CWorld* world;
    if (Component->m_pDisplayEngine->IsScreenFullyFadedOut() && World->IsDisplayingMiniMapOnRegionLoad() && (world = World)->IsRegionLoadShowingMiniMap()) {
        if (!RegionChange) {
            FadeInOut.Start(0.5f, 1.0f, CRGBColour(0, 0, 0, 255));
            OldActive = Active;
            Active = true;
            RegionChange = true;
        }
        else if (!FadeInOut.Closing && world->RegionLoadStage == 8)
            FadeInOut.FadeOut();
    }
    else {
        if (RegionChange) {
            Active = OldActive;
            RegionChange = false;
        }
        else if (Active) {
            if (!FadeInOut.Active)
                FadeInOut.Start(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
        }
        else if (FadeInOut.Active)
            FadeInOut.FadeOut();
    }
    FadeInOut.Update(elapsed);
    world = World;
    bool enabled = world->MiniMapEnabled;
    if (enabled != Enabled) {
        Enabled = enabled;
        if (enabled)
            SetAsActive(world->MiniMapActive);
        else
            SetAsActive(false);
    }
}