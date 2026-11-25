// NF52 (round 7): the 0x013BC470 store is &CShaderRenderManager::SingletonInstance (?SingletonInstance@CShaderRenderManager@@0V1@A,
// engine/CShaderRenderManager.h Get()); was the made-up g_FableRenderStateStorage_013BC470 byte.
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
#include "fable_boot.h"

// GFInitialise() -- cache the engine's core manager pointers, pick the texture bit depth and display size, and start the loading screen
// Retail: Fable.exe (TLC, VC7.1) @ 0x004022B0, 311 bytes. Callees: engine root accessor 0x00404A80, CDisplayManager::IsTextureBitDepthSupported,
//   CXMVPlayer::GlobalRegisterPlayer, GFGetBuildNumber2 0x00401F30, GFInitialise_SetupProgressDisplay.
// Sound manager (agentSND2, 2026-11-10): PSoundManager is engine/CASoundManager.h's (was a local forward declaration).
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

class CDisplayManager;

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

class CDisplayManager
{ public:
    fable_u8 unknown00[0x08];
    FableDisplayPrimitive* displayPrimitive08;  // the primary display primitive
    fable_u8 unknown0C[0x1BC];
    fable_i32 displayWidth1C8;                  // current mode width, halved for UI centring

    // Can the device create textures at this bit depth?
    bool IsTextureBitDepthSupported(unsigned long bitDepth);
};
#pragma pack(pop)

struct CXMVPlayer
{
    // Registers the XMV movie player with the engine's video playback hooks.
    static void __fastcall GlobalRegisterPlayer();   // PDB SIXXZ (static __fastcall; no arguments, same call bytes)
};

// Accessor for the engine root object.
// Reads full_build_version.txt into the global build string when present.
extern void FABLE_CDECL GFGetBuildNumber2();

// Game-side caches of the engine managers, filled in here.
#include "engine/CASoundManager.h"                  // PSoundManager 0x013B8394
extern CDisplayManager* GPDisplayManager;
extern FableDisplayPrimitive* g_FableDisplayPrimitive_013B8384;
extern void* g_FableRenderState_013B8380;
#include "engine/CShaderRenderManager.h"            // CShaderRenderManager::SingletonInstance 0x013BC470 (through Get())
class CInputManager;
extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A
class CFontManager;
extern CFontManager* PFontManager;
class CGraphicBankManager;
extern CGraphicBankManager* PGraphicBankManager;
// Chosen texture bit depth, and the depth the configuration asked for.
extern fable_u32 g_FableTextureBitDepth_013B7D68;
extern fable_u32 g_FableRequestedTextureBitDepth_01375470;
// Upper bound on either display dimension, and the resulting working size.
extern fable_u32 g_FableMaximumDisplayDimension_0137546C;
extern fable_u32 g_FableDisplayWidth_013B7D64;
extern fable_u32 g_FableDisplayHeight_013B7D60;
extern fable_i32 g_FableHalfDisplayWidth_0137544C;
// The CGame object (Ego_r / FableWin dynamic initializer 'GGame'); the fable_boot.h callee view of 0x00413120 takes it as
// GFInitialiseState (its identity, probably CGame::Initialise, is not settled: naming wave 13 DEFERRED).
class CGame;
extern CGame GGame;                                     // 0x013B83D0

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009A4EC0 ?GFGetSystemManager@@YIPAVCSystemManager@@XZ (was ?AddChildPrimitive@@YIPAUFableEngineRoot@@XZ)
class CSystemManager;
CSystemManager* __fastcall GFGetSystemManager();
bool FABLE_FASTCALL GFInitialise()
{
    // Copy the manager pointers out of the engine root.
    PSoundManager = (CASoundManager*)(reinterpret_cast<FableEngineRoot*>(GFGetSystemManager())->primitiveManager7C);

    GPDisplayManager =
        reinterpret_cast<FableEngineRoot*>(GFGetSystemManager())->displayManager60;
    g_FableDisplayPrimitive_013B8384 =
        GPDisplayManager->displayPrimitive08;
    g_FableRenderState_013B8380 = &CShaderRenderManager::Get();

    GPInputManager = (CInputManager*)(reinterpret_cast<FableEngineRoot*>(GFGetSystemManager())->renderDevice58);
    PFontManager = (CFontManager*)(reinterpret_cast<FableEngineRoot*>(GFGetSystemManager())->resourceManager84);
    PGraphicBankManager = (CGraphicBankManager*)(reinterpret_cast<FableEngineRoot*>(GFGetSystemManager())->textureManager6C);

    // Texture bit depth: the requested depth if the card can do it, else the first supported of 16/24/32.
    g_FableTextureBitDepth_013B7D68 =
        g_FableRequestedTextureBitDepth_01375470;
    if (GPDisplayManager->IsTextureBitDepthSupported(
            g_FableRequestedTextureBitDepth_01375470))
    {
        g_FableTextureBitDepth_013B7D68 =
            g_FableRequestedTextureBitDepth_01375470;
    }
    else if (
        GPDisplayManager->IsTextureBitDepthSupported(16))
    {
        g_FableTextureBitDepth_013B7D68 = 16;
    }
    else if (
        GPDisplayManager->IsTextureBitDepthSupported(24))
    {
        g_FableTextureBitDepth_013B7D68 = 24;
    }
    else if (
        GPDisplayManager->IsTextureBitDepthSupported(32))
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
            (GFInitialiseState*)&GGame) == 1)
    {
        g_FableHalfDisplayWidth_0137544C =
            GPDisplayManager->displayWidth1C8 / 2;
        return true;
    }

    return false;
}
