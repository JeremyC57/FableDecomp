#pragma optimize("s", on)
// CTCInventory::StartGameRendering() -- switches the renderer back to in-game rendering and stamps the world with the current frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581C1E, length 46 bytes.
// Callees: virtual slot 76 (`call [eax+130h]`, argument 1) on the renderer reached through the global
//          game object 0x013B86A0 (+0x28 -> +0x2c), CTCBase world getter 0x00686A70 (landed name GetWorld),
//          GetWorldFrame 0x0049D870 (static; returns the frame counter 0x013B89BC).
// Ego_r PDB: ?StartGameRendering@CTCInventory@@QAEXXZ. The world word written (+0x8c) has no PDB name
// in the retail layout, so it is named by its use.
// Shape: the global chain is loaded before `push esi` (scheduling); the world pointer is fetched
// before the frame call because it is a named local (`mov esi,eax`).
struct CRenderer {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75();
    virtual void SetGameRendering(long enable);      // slot 76
};

struct CDisplaySystem {
    unsigned char _pad_0x00[0x2c];
    CRenderer*    m_pRenderer;                       // +0x2c
};

struct CGameObject {
    unsigned char   _pad_0x00[0x28];
    CDisplaySystem* m_pDisplay;                      // +0x28
};

extern CGameObject* g_pGameObject;                   // 0x013B86A0

struct CWorld {
    unsigned char _pad_0x00[0x8c];
    long          m_gameRenderingStartFrame;         // +0x8c
    static long GetWorldFrame();                     // 0x0049D870
};

struct CTCInventory {
    CWorld* GetWorld();                              // CTCBase, 0x00686A70
    void StartGameRendering();
};

void CTCInventory::StartGameRendering()
{
    g_pGameObject->m_pDisplay->m_pRenderer->SetGameRendering(1);
    CWorld* world = GetWorld();
    world->m_gameRenderingStartFrame = CWorld::GetWorldFrame();
}
