// CEngineWeatherRenderer::ClipLockedBoxToCamera(C3DVector* box, float radius, long index) -- camera-clip a weather box unless the visibility results are locked, caching the result per box index; returns the cached visibility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B52120, length 60 bytes (the oracle row carries 4 bytes of int3 padding).
// Callees: CEngineWeatherRenderer::ClipBoxToCamera(C3DVector*, float) @ 0x00B51F60 (manifest: returns EWeatherBoxVisible). Shape: the lock
// byte is tested through al; on the unlocked path the call result is stored into SavedBoxVisibility[index] and returned from the same
// register, on the locked path the slot is re-read. The index is the THIRD argument (`[esp+10h]`), the box pointer and radius are
// pushed in that order. SavedBoxVisibility is at +0x784 as in engine/CEngineWeatherRenderer.h; the lock byte at +0x11a sits inside the
// header's RainParticle/Mist shader region (the Ego PDB layout differs from retail there), so the retail layout is modelled locally.
class C3DVector;

class CEngineWeatherRenderer { public:
    unsigned char _pad0[0x11a];
    bool          BoxVisibilityLocked;             // +0x11a
    unsigned char _pad1[0x784 - 0x11b];
    long          SavedBoxVisibility[729];         // +0x784 EWeatherBoxVisible[729]
    long ClipBoxToCamera(C3DVector* box, float radius);   // 0x00B51F60
    long ClipLockedBoxToCamera(C3DVector* box, float radius, long index);
};

long CEngineWeatherRenderer::ClipLockedBoxToCamera(C3DVector* box, float radius, long index)
{
    if (!BoxVisibilityLocked)
        SavedBoxVisibility[index] = ClipBoxToCamera(box, radius);
    return SavedBoxVisibility[index];
}