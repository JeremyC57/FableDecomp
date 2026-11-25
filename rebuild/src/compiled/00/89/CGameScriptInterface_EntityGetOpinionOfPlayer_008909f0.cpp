// CGameScriptInterface::EntityGetOpinionOfPlayer(const CScriptThing& thing, EOpinion opinion) const -- the world's opinion reaction manager's opinion of the player held by the script thing's thing (slot 11, no validity check; CWorld+0x74 the counted manager, COpinionReactionManager::GetThingOpinionOfPlayer 0x00701750); the .rdata 0.0f constant @ 0x0122DEDC without a thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008909F0, length 33 bytes.
// Callees: the script thing's virtual slot 11 (+0x2c) GetPThing (no validity check), COpinionReactionManager::GetThingOpinionOfPlayer @ 0x00701750 (a float in st0), the float constant @ 0x0122DEDC (0.0f, loaded from the pool as in the landed 0x00692400). Shape: the opinion re-read from the stack and pushed before the thing, the constant path `fld dword ptr [pool]`, `ret 8`.
// Leaf: EntityGetOpinionOfPlayer
// Float literals (NF418, 2026-11-25): retail's merged __real@ pool constants were modelled as data: g_fNoOpinion_0122dedc (table ?g_fNoOpinion_0122dedc@@3MB -> 0x0122DEDC) -> 0.0f. Spelled at the use
//   site; VC7.1 references a plain float literal through __real@<bits> (COMDAT, exe-wide merged), e.g. `return 0.0f;` -> fld [__real@00000000].
// NF65 (round 7): ported onto the shared headers (engine/CScriptThing.h, engine/CWorld.h GetOpinionReactionManager, engine/COpinionReactionManager.h
// GetThingOpinionOfPlayer under the FABLE_RETAIL_GREETING_PASS23 opt-in); the CThing_Retail / COpinionReactionManager_Retail / local CWorld stand-ins
// are gone. The call now binds Ego_r.pdb / FableWin.pdb ?GetThingOpinionOfPlayer@COpinionReactionManager@@QBEMABVCThing@@W4EOpinion@@@Z (was
// ?GetOpinionValue@COpinionReactionManager_Retail@@QAEMPAUCThing_Retail@@J@Z).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_GREETING_PASS23
#include "fable_compiler.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
class CTCOpinionSource;
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CWorld.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CGameScriptInterface.h"


float CGameScriptInterface::EntityGetOpinionOfPlayer(const CScriptThing& thing, EOpinion opinion) const
{
    CThing* t = thing.GetPThing();
    if (t) {
        float value = World->GetOpinionReactionManager().GetThingOpinionOfPlayer(*t, opinion);
        return value;
    }
    return 0.0f;
}
