#ifndef FABLE_ENGINE_NUISYSTEM_CUISCREENUNITCONVERTER_H
#define FABLE_ENGINE_NUISYSTEM_CUISCREENUNITCONVERTER_H
// Retail model (chunk 733): NUISystem::CUIScreenUnitConverter, the static UI back-buffer size. Ego_r.pdb / FableWin.pdb statics
// IsUsingRelativeCoords 0x013B8768, BackBufferWidth 0x013B876C, BackBufferHeight 0x013B8770 (the landed
// SetIsUsingRelativeCoords 0x004299A8 spellings). ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ (Ego_r 0x0040C590,
// FableWin 0x01884760) is inline in retail: CUserProfileManager::ApplyVideoValues 0x0040A7F0 expands it twice (the render
// target's size through the hidden-pointer GetRenderTargetDimensions, then `fild / fstp` into the two floats).
// SetIsUsingRelativeCoords 0x004299A8 is out of line.
#include "rebuild_abi.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static void FABLE_FASTCALL SetIsUsingRelativeCoords(bool relative);   // 0x004299A8
    static void FABLE_FASTCALL ResetDimensions()
    {
        C2DExtentsI dimensions = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
        BackBufferWidth = (float)dimensions.X;
        BackBufferHeight = (float)dimensions.Y;
    }
    // public statics: the landed spelling ?BackBufferWidth@CUIScreenUnitConverter@NUISystem@@2MA (SetIsUsingRelativeCoords
    // 0x004299A8, CDisplayManager::ResetDevice 0x009BFB30); one retail address, one name.
    static bool  IsUsingRelativeCoords;                             // 0x013B8768
    static float BackBufferWidth;                                   // 0x013B876C
    static float BackBufferHeight;                                  // 0x013B8770
};
}
#endif
