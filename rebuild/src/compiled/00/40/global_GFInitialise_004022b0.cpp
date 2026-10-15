#include "fable_boot.h"

// GFInitialise() -- cache the engine's core manager pointers, pick the texture bit depth and display size, and start the loading screen
// Retail: Fable.exe (TLC, VC7.1) @ 0x004022B0, 311 bytes. Callees: engine root accessor 0x00404A80, CDisplayManager::IsTextureBitDepthSupported,
//   CXMVPlayer::GlobalRegisterPlayer, GFGetBuildNumber2 0x00401F30, GFInitialise_SetupProgressDisplay.
//
// Runs from GFMain after the system manager has created the window and the D3D device. It is the point
// where the game-side globals that everything else uses (primitive manager, display manager, display
// primitive, render state, render device, resource manager, texture manager) are copied out of the engine
// root object so later code can reach them without walking the object graph.
//
// Texture bit depth: the depth the user asked for (from the profile / config detection) is used if the
// card supports it, otherwise the first of 16, 24, 32 that does. The working display width and height
// are the display primitive's maximum size clamped to the configured maximum dimension. Then the XMV
// video player registers itself, the optional full build number is read from disk, and the loading
// progress display is set up; if that succeeds the half-width used for centring UI is computed and the
// function reports success. A failed progress-display setup is the only failure path.
//
// Matching: the engine root is re-fetched through the accessor for each manager rather than cached in a
// local; the four IsTextureBitDepthSupported calls are a flat if/else-if chain; the width/height clamps
// share one `displayDimension` local (two `cmp/jbe` sequences); the `== 1` test on the progress-display
// result is a byte compare.

struct CDisplayManager;

#pragma pack(push, 1)
// The engine root object as far as this function needs it.
struct FableEngineRoot
{
    fable_u8 unknown00[0x58];
    fable_u32 renderDevice58;           // D3D render device wrapper
    fable_u8 unknown5C[0x04];
    CDisplayManager* displayManager60;  // display manager (modes, surfaces)
    fable_u8 unknown64[0x08];
    fable_u32 textureManager6C;         // texture manager
    fable_u8 unknown70[0x0C];
    fable_u32 primitiveManager7C;       // primitive (mesh) manager
    fable_u8 unknown80[0x04];
    fable_u32 resourceManager84;        // resource manager
};

// The display primitive: only its maximum back-buffer size is read here.
struct FableDisplayPrimitive
{
    fable_u8 unknown00[0x3AB0];
    fable_u32 maximumWidth3AB0;         // largest supported display width
    fable_u32 maximumHeight3AB4;        // largest supported display height
};

struct CDisplayManager
{
    fable_u8 unknown00[0x08];
    FableDisplayPrimitive* displayPrimitive08;  // the primary display primitive
    fable_u8 unknown0C[0x1BC];
    fable_i32 displayWidth1C8;                  // current mode width, halved for UI centring

    // Can the device create textures at this bit depth?
    bool IsTextureBitDepthSupported(int bitDepth);
};
#pragma pack(pop)

struct CXMVPlayer
{
    // Registers the XMV movie player with the engine's video playback hooks.
    static void GlobalRegisterPlayer();
};

// Accessor for the engine root object.
extern FableEngineRoot* FABLE_FASTCALL AddChildPrimitive();
// Reads full_build_version.txt into the global build string when present.
extern void FABLE_CDECL GFGetBuildNumber2();

// Game-side caches of the engine managers, filled in here.
extern fable_u32 g_FablePrimitiveManager_013B8394;
extern CDisplayManager* g_FableDisplayManager_013B8390;
extern FableDisplayPrimitive* g_FableDisplayPrimitive_013B8384;
extern void* g_FableRenderState_013B8380;
extern fable_u8 g_FableRenderStateStorage_013BC470;
extern fable_u32 g_FableRenderDevice_013B8388;
extern fable_u32 g_FableResourceManager_013B838C;
extern fable_u32 g_FableTextureManager_013B837C;
// Chosen texture bit depth, and the depth the configuration asked for.
extern fable_u32 g_FableTextureBitDepth_013B7D68;
extern fable_u32 g_FableRequestedTextureBitDepth_01375470;
// Upper bound on either display dimension, and the resulting working size.
extern fable_u32 g_FableMaximumDisplayDimension_0137546C;
extern fable_u32 g_FableDisplayWidth_013B7D64;
extern fable_u32 g_FableDisplayHeight_013B7D60;
extern fable_i32 g_FableHalfDisplayWidth_0137544C;
// Start-up state object that owns the progress display.
extern GFInitialiseState g_GFInitialiseState_013B83D0;

bool FABLE_FASTCALL GFInitialise()
{
    // Copy the manager pointers out of the engine root.
    g_FablePrimitiveManager_013B8394 =
        AddChildPrimitive()->primitiveManager7C;

    g_FableDisplayManager_013B8390 =
        AddChildPrimitive()->displayManager60;
    g_FableDisplayPrimitive_013B8384 =
        g_FableDisplayManager_013B8390->displayPrimitive08;
    g_FableRenderState_013B8380 = &g_FableRenderStateStorage_013BC470;

    g_FableRenderDevice_013B8388 =
        AddChildPrimitive()->renderDevice58;
    g_FableResourceManager_013B838C =
        AddChildPrimitive()->resourceManager84;
    g_FableTextureManager_013B837C =
        AddChildPrimitive()->textureManager6C;

    // Texture bit depth: the requested depth if the card can do it, else the first supported of 16/24/32.
    g_FableTextureBitDepth_013B7D68 =
        g_FableRequestedTextureBitDepth_01375470;
    if (g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(
            g_FableRequestedTextureBitDepth_01375470))
    {
        g_FableTextureBitDepth_013B7D68 =
            g_FableRequestedTextureBitDepth_01375470;
    }
    else if (
        g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(16))
    {
        g_FableTextureBitDepth_013B7D68 = 16;
    }
    else if (
        g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(24))
    {
        g_FableTextureBitDepth_013B7D68 = 24;
    }
    else if (
        g_FableDisplayManager_013B8390->IsTextureBitDepthSupported(32))
    {
        g_FableTextureBitDepth_013B7D68 = 32;
    }

    // Working display size: the primitive's maximum, clamped to the configured ceiling.
    FableDisplayPrimitive* const displayPrimitive =
        g_FableDisplayPrimitive_013B8384;
    fable_u32 displayDimension =
        displayPrimitive->maximumWidth3AB0;
    if (
        g_FableMaximumDisplayDimension_0137546C <
        displayDimension)
    {
        displayDimension =
            g_FableMaximumDisplayDimension_0137546C;
    }
    g_FableDisplayWidth_013B7D64 = displayDimension;

    displayDimension =
        displayPrimitive->maximumHeight3AB4;
    if (
        g_FableMaximumDisplayDimension_0137546C <
        displayDimension)
    {
        displayDimension =
            g_FableMaximumDisplayDimension_0137546C;
    }
    g_FableDisplayHeight_013B7D60 = displayDimension;

    // Video playback hook, optional on-disk build number, then the loading screen.
    CXMVPlayer::GlobalRegisterPlayer();
    GFGetBuildNumber2();

    if (GFInitialise_SetupProgressDisplay(
            &g_GFInitialiseState_013B83D0) == 1)
    {
        g_FableHalfDisplayWidth_0137544C =
            g_FableDisplayManager_013B8390->displayWidth1C8 / 2;
        return true;
    }

    return false;
}
