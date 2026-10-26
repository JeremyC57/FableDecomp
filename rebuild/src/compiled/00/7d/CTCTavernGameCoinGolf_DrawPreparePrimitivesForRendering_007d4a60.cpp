// CTCTavernGameCoinGolf::DrawPreparePrimitivesForRendering(CRenderContext* context) -- DrawPreparePrimitivesForRendering override: base draw-prepare, then FadeOutTableOccupants(false) while the game is being played
// Retail: Fable.exe @ 0x007D4A60, 33 bytes (vtable 0x0126F49C slot 11, `ret 4`; slot 11 of the
//         CTCTavernGame vtable 0x012560BC is DrawPreparePrimitivesForRendering 0x005ECA50).
// Callees: CTCTavernGame::DrawPreparePrimitivesForRendering 0x005ECA50 (non-virtual base call),
//          CTCTavernGameCoinGolf::FadeOutTableOccupants(bool) 0x007D46D0.
// +0x20 is CTCTavernGame::IsBeingPlayed: the PDB places it at +0x28, retail's UIComponents vector is
// 8 bytes smaller (0x0C vs 0x14), so the engine header's offsets do not apply (local model).
// Shape: byte member tested with `mov al ; test al,al` (a bool member).
// Layout (supersedes any local-model note above): engine/CTCTavernGame.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCTavernGame CTCTavernGameDonor
#include "engine/CTCTavernGame.h"
#undef CTCTavernGame
struct CRenderContext;

class CTCTavernGame : public CTCTavernGameDonor {
 public:
    void DrawPreparePrimitivesForRendering(CRenderContext* context);    // 0x005ECA50
};

class CTCTavernGameCoinGolf : public CTCTavernGame { public:
    void FadeOutTableOccupants(bool fadeOut);                           // 0x007D46D0
    void vfunc_11(CRenderContext* context);
};

void CTCTavernGameCoinGolf::vfunc_11(CRenderContext* context)
{
    CTCTavernGame::DrawPreparePrimitivesForRendering(context);
    if (IsBeingPlayed)
        FadeOutTableOccupants(false);
}