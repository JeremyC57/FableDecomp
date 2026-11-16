// CEngineWeatherRenderer::ClipLockedBoxToCamera(C3DVector* box, float radius, long index) -- camera-clip a weather box unless the visibility results are locked, caching the result per box index; returns the cached visibility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B52120, length 60 bytes (the oracle row carries 4 bytes of int3 padding).
// Callees: CEngineWeatherRenderer::ClipBoxToCamera(const C3DVector&, float) const @ 0x00B51F60 (private, returns EWeatherBoxVisible;
// respelled at landing nf46 to the PDB symbol the landed callee now defines). Shape: the lock
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
    enum EWeatherBoxVisible { WEATHER_BOX_VISIBLE = 0, WEATHER_BOX_CLIPPED = 1 };   // as engine/CEngineWeatherRenderer.h
    long ClipLockedBoxToCamera(C3DVector* box, float radius, long index);
private:
    // Ego_r.pdb / FableWin.pdb ?ClipBoxToCamera@CEngineWeatherRenderer@@ABE?AW4EWeatherBoxVisible@1@ABVC3DVector@@M@Z (landed 0x00B51F60).
    EWeatherBoxVisible ClipBoxToCamera(const C3DVector& pos, float radius) const;   // 0x00B51F60
};

long CEngineWeatherRenderer::ClipLockedBoxToCamera(C3DVector* box, float radius, long index)
{
    if (!BoxVisibilityLocked)
        SavedBoxVisibility[index] = ClipBoxToCamera(*box, radius);
    return SavedBoxVisibility[index];
}