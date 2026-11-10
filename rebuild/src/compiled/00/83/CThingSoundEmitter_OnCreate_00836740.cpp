// CThingSoundEmitter::OnCreate() -- after the CThing creation succeeds and while the sound manager exists, create the thing's 3-D sound emitter at Pos; true when one was made
// Leaf: ?OnCreate@CThingSoundEmitter@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00836740, thiscall, vtable 0x01272C24 slot 8.
// Callees: CThing::OnCreate 0x004C9560, CASoundManager::Create3DEmitter (PSoundManager slot 4, +0x10).
// Model (agentSND2, 2026-11-10): the shared engine/CThingSoundEmitter.h with the opt-ins FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
//   (+0x98 PSoundEmitter) and _MEMBERS (Pos C3DVector +0xA6, packed), engine/CASoundManager.h's PSoundManager and its slot 4
//   Create3DEmitter; was a local padded CThing / CThingSoundEmitter model with a CGameState stand-in for PSoundManager.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING_LOADSINGLE                        // CThing slot 8 OnCreate (Ego_r.pdb ?OnCreate@CThing@@UAE_NXZ)
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#define FABLE_RETAIL_CTHINGSOUNDEMITTER_MEMBERS
#include "engine/CASoundManager.h"
#include "engine/CThingSoundEmitter.h"

bool CThingSoundEmitter::OnCreate()
{
    if (CThing::OnCreate() && PSoundManager) {
        PSoundEmitter = PSoundManager->Create3DEmitter(Pos);
        if (PSoundEmitter)
            return true;
    }
    return false;
}
