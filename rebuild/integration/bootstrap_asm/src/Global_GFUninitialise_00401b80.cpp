#include "fable_gfuninitialise.h"

// GFUninitialise() -- orderly shutdown of the engine's global subsystems, in reverse start-up order
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401B80, 64 bytes. Callees: CInitBase::Uninitialise, FableFileInstallerSingletonRelease,
//   CBankFileManager::Close, GFGetSystemManager / CSystemManager::Uninitialise, CThreadedFileManager::Uninitialise,
//   UninitializeStreamingFontTextureCache, engine primitive manager Uninitialise.
//
// Counterpart of GFInitialise (0x004022B0): called once from the main loop's exit path after the game
// has saved and the front end has closed. Each line releases one global service the start-up brought
// up: the base init object, the file installer singleton, the open bank files (the big .big archives),
// the system manager (window, input, D3D device), the threaded file reader, the streaming font texture
// cache, and finally the engine's primitive manager. Order matters because later services hold handles
// into earlier ones, so this walks back down the stack the start-up built.
//
// Matching: every callee is a plain call on a global object or singleton accessor; the primitive
// manager accessor returns its pointer in eax and the call follows immediately.
void FABLE_CDECL GFUninitialise()
{
    g_FableInitBase_013B83D0.Uninitialise();
    FableFileInstallerSingletonRelease();
    g_FableBankFileManager_013CA79C.Close();
    GFGetSystemManager()->Uninitialise();
    g_FableThreadedFileManager_013BC870.Uninitialise();
    UninitializeStreamingFontTextureCache();
    FableGetEnginePrimitiveManager_00404A80()->Uninitialise();
}
