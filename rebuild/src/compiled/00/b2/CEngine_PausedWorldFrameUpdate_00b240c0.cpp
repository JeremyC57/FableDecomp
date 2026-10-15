// CEngine::PausedWorldFrameUpdate() -- once initialised and rendering, forward a paused world frame to every engine component whose scene flags match the engine's filter; then count the paused frame and advance the paused world time by 1/WorldFramesPerSecond
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B240C0, length 140 bytes (+ int3 padding).
// Callees: CEngineComponent virtual slots 10 (+0x28, flags) and 8 (+0x20, the paused update, no argument). Float pool 0x0122DCB4 =
// 2^32 (the unsigned-to-float fix-up), 0x0122DED8 = 1.0f.
// Layout: the engine header CEngine.h carries the PDB offsets, which are 8 bytes too high for these members in retail (IsInitialised
// PDB +0x8c / retail +0x84, CurrentRenderFrame +0x88, CurrentWorldFrame +0x8c, CurrentPausedWorldFrame +0x90, NonInterpolatedWorldTime
// +0x98, NonInterpolatedPausedWorldTime +0xa0, WorldFramesPerSecond +0xb0, SceneFilterFlags +0xb8, EngineComponents +0x168), so the
// class is modelled locally. Component vtable slots are named by use (no RTTI names): 10 = scene filter flags, 7/8 = the frame updates.
// Shape: twin of WorldFrameUpdate @ 0x00B24030 without the argument (the unused `push ecx` frame word comes out of the same source);
// the component loop re-reads `end()` every pass.
struct CEngineComponentParams;

struct CEngineComponent {                    // engine component interface (vtable slots used here only)
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void WorldFrameUpdate(const CEngineComponentParams& params);   // slot 7 (+0x1c)
    virtual void PausedWorldFrameUpdate();                                 // slot 8 (+0x20)
    virtual void v09();
    virtual unsigned long GetSceneFilterFlags();                           // slot 10 (+0x28)
};

template <class T>
struct CRetailVector {                       // STLport vector: 12 bytes
    T* m_start;
    T* m_finish;
    T* m_endOfStorage;
    T* begin() const { return m_start; }
    T* end() const { return m_finish; }
};

struct CEngine {                             // retail layout: the PDB members from IsInitialised on sit 8 bytes lower (see header comment)
    unsigned char  _pad00[0x84];
    bool           IsInitialised;            // +0x84 (PDB +0x8c)
    long           CurrentRenderFrame;       // +0x88
    long           CurrentWorldFrame;        // +0x8c
    long           CurrentPausedWorldFrame;  // +0x90
    unsigned char  _pad94[0x4];
    double         NonInterpolatedWorldTime;        // +0x98
    double         NonInterpolatedPausedWorldTime;  // +0xa0
    double         InterpolatedWorldTime;           // +0xa8
    unsigned long  WorldFramesPerSecond;     // +0xb0
    bool           BlockOnLoad;              // +0xb4
    unsigned long  SceneFilterFlags;         // +0xb8
    unsigned char  _padbc[0x168 - 0xbc];
    CRetailVector<CEngineComponent*> EngineComponents;   // +0x168 (PDB +0x178, retail CArray/vector 12 bytes)
    void WorldFrameUpdate(const CEngineComponentParams& params);
    void PausedWorldFrameUpdate();
};

void CEngine::PausedWorldFrameUpdate()
{
    if (IsInitialised && CurrentRenderFrame != 0) {
        for (CEngineComponent** it = EngineComponents.begin(); it != EngineComponents.end(); ++it) {
            if ((*it)->GetSceneFilterFlags() & SceneFilterFlags)
                (*it)->PausedWorldFrameUpdate();
        }
    }
    ++CurrentPausedWorldFrame;
    NonInterpolatedPausedWorldTime += 1.0f / (float)WorldFramesPerSecond;
}