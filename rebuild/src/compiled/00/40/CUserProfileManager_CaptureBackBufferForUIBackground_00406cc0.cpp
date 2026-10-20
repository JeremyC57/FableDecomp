// CUserProfileManager::CaptureBackBufferForUIBackground() -- when the UI background capture is enabled, render one 3D-only frame twice from the last interpolation info and copy the back buffer into PBackBufferTexture (0 otherwise)
// Leaf: CaptureBackBufferForUIBackground
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00406CC0, length 204 bytes (208 with padding).
// Callees: CEngine::Enable3DSceneRendering(bool) (CEngine slot 76, +0x130) and Enable2DSceneRendering(bool) (slot 78,
// +0x138) on the display engine's Engine3D, CDisplayEngine::DoRender(CInterpolationInfo*, bool, bool) @ 0x00435F70,
// CDisplayManager::CopyBackBufferToTexture @ 0x009BF5E0. Globals: the enable flag @ 0x013750A7 (descriptive name),
// g_pMainGameComponent @ 0x013B86A0, the display manager @ 0x013B8390.
// Layout (Ego_r.pdb): CMainGameComponent PDisplayEngine +0x28, LastInterpolationInfo (0x34 bytes) +0x70; CDisplayEngine
// Engine3D +0x2c; CUserProfileManager PBackBufferTexture +0x138 (PDB +0x13c, retail 4 bytes lower).
// Shape: the global chain re-read for every call; the interpolation info is copied whole (`rep movsd`) into a local.
struct CTexture;
struct CInterpolationInfo { long m_words[13]; };            // 0x34 bytes (six CInterpolationInfoSet + WFServerCurrentTime)
struct CEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75();
    virtual void Enable3DSceneRendering(bool enable);       // slot 76 (+0x130) 0x00B23F90
    virtual void v77();
    virtual void Enable2DSceneRendering(bool enable);       // slot 78 (+0x138) 0x00B23FD0
};
struct CDisplayEngine {
    unsigned char _pad00[0x2c];
    CEngine*      Engine3D;                                 // +0x2c
    void DoRender(CInterpolationInfo* info, bool a, bool b);   // 0x00435F70
};
struct CMainGameComponent {
    unsigned char      _pad00[0x28];
    CDisplayEngine*    PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
    unsigned char      _pad2c[0x70 - 0x2c];
    CInterpolationInfo LastInterpolationInfo;               // +0x70
};
struct CDisplayManager {
    CTexture* CopyBackBufferToTexture();                    // 0x009BF5E0
};
extern bool                g_bCaptureUIBackground;         // 0x013750A7
extern CMainGameComponent* g_pMainGameComponent;           // 0x013B86A0
extern CDisplayManager*    g_pDisplayManager;              // 0x013B8390

struct CUserProfileManager {
    unsigned char _pad000[0x138];
    CTexture*     PBackBufferTexture;                       // +0x138
    void CaptureBackBufferForUIBackground();
};

void CUserProfileManager::CaptureBackBufferForUIBackground()
{
    if (g_bCaptureUIBackground) {
        g_pMainGameComponent->PDisplayEngine->Engine3D->Enable3DSceneRendering(true);
        g_pMainGameComponent->PDisplayEngine->Engine3D->Enable2DSceneRendering(false);
        CInterpolationInfo info = g_pMainGameComponent->LastInterpolationInfo;
        g_pMainGameComponent->PDisplayEngine->DoRender(&info, false, false);
        g_pMainGameComponent->PDisplayEngine->DoRender(&info, false, false);
        g_pMainGameComponent->PDisplayEngine->Engine3D->Enable3DSceneRendering(false);
        g_pMainGameComponent->PDisplayEngine->Engine3D->Enable2DSceneRendering(true);
        PBackBufferTexture = g_pDisplayManager->CopyBackBufferToTexture();
    }
    else
        PBackBufferTexture = 0;
}