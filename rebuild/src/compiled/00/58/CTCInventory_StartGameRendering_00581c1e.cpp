#pragma optimize("s", on)
// CTCInventory::StartGameRendering() -- switches the renderer back to in-game rendering and stamps the world with the current frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581C1E, length 46 bytes.
// Callees: virtual slot 76 (`call [eax+130h]`, argument 1) on the renderer reached through the global
//          game object 0x013B86A0 (+0x28 -> +0x2c), CTCBase world getter 0x00686A70 (landed name GetWorld),
//          CWorld::GetFrame 0x0049D870 (static; returns the frame counter 0x013B89BC).
// Ego_r PDB: ?StartGameRendering@CTCInventory@@QAEXXZ. The world word written (+0x8c) has no PDB name
// in the retail layout, so it is named by its use.
// Shape: the global chain is loaded before `push esi` (scheduling); the world pointer is fetched
// before the frame call because it is a named local (`mov esi,eax`).
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its display engine (PDisplayEngine +0x28, FableWin.pdb ?GetDisplayEngine@CMainGameComponent@@QAEAAVCDisplayEngine@@XZ) and that engine's Engine3D +0x2C (FableWin.pdb ?Get3DEngine@CDisplayEngine@@QAEAAVCIEngine@@XZ), slot 76 CIEngine::Enable3DSceneRendering(bool) (engine/CIEngine.h); was the stand-in CGameObject* g_pGameObject with its CDisplaySystem / CRenderer::SetGameRendering stand-ins.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"

class CWorld { public:
    unsigned char _pad_0x00[0x8c];
    long          m_gameRenderingStartFrame;         // +0x8c
    static long __fastcall GetFrame();                     // 0x0049D870
};

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventory : public NInventory::CTCInventoryBase { public:
    void StartGameRendering();
};

void CTCInventory::StartGameRendering()
{
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
    CWorld* world = (&GetWorld());
    world->m_gameRenderingStartFrame = CWorld::GetFrame();
}