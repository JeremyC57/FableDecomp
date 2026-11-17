class CMeshDataBank;

// Leaf: ?GetMeshBank_2@CAnimateThing@@QAEPAVCMeshDataBank@@XZ
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its world (PWorld +0x24, FableWin.pdb ?GetWorld@CMainGameComponent@@QAEAAVCWorld@@XZ) and the world's main mesh bank (PMainMeshBank +0x3C, FableWin.pdb ?GetMeshBank@CWorld@@QAEPAVCMeshDataBank@@XZ, engine/CWorld.h); was the stand-in CEngineSingleton* g_pEngineSingleton with its CInner stand-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

class CAnimateThing
{
public:
    CMeshDataBank* GetMeshBank_2();
};

CMeshDataBank* CAnimateThing::GetMeshBank_2()
{
    return CMainGameComponent::Get()->GetWorld().GetMeshBank();
}