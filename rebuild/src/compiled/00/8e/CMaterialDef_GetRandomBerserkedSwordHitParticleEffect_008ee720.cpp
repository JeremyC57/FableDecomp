// CMaterialDef::GetRandomBerserkedSwordHitParticleEffect() const -- pick one entry of the material's berserked sword-hit particle-effect list with the world's local random seed (0 when the list is empty)
// Leaf: ?GetRandomBerserkedSwordHitParticleEffect@CMaterialDef@@QBEJXZ
// Name (NF237, land1122auth24): the const member, Ego_r.pdb + FableWin.pdb publics ?GetRandomBerserkedSwordHitParticleEffect@CMaterialDef@@QBEJXZ
//   (engine/CMaterialDef.h FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS declares it); landed 0x00839420 binds that spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EE720, length 100 bytes.
// Callees: ?GetLocalSeed@CWorld@@QBEPAKXZ @ 0x0049D800 (Ego_r.pdb / FableWin.pdb; &Component->LocalSeed; engine/CWorld.h
//   FABLE_RETAIL_CWORLD_GETLOCALSEED, the spelling the four sibling GetRandom*Effect rows 0x008EE5A0 / 0x008EE600 / 0x008EE660 / 0x008EE6C0 bind)
//   on the main game component's world (FableWin.pdb inline ?GetWorld@CMainGameComponent@@QAEAAVCWorld@@XZ: the scoped_ptr word +0x24 of
//   the protected static PMainGameComponent 0x013B86A0, read through the inline static Get()), GFROR13 @ 0x00497890 (`__fastcall`).
// Body: the inlined GFRandom(n, seed) (Ego_r.pdb ?GFRandom@@YIKKPAK@Z, engine/GFRandom.h), the seed pointer fetched before the size is
//   computed (the siblings' shape); the non-empty arm is the nested one so `return 0` lands at the end.
// Layout: engine/CMaterialDef.h (retail: 12-byte STLport vectors, BerserkedSwordHitParticleEffect +0x80, sizeof 0x98).
// NF253 (q12712, landing nf250): ported off the HEAD local models (the stand-ins CEngineRandomOwner / CRandomGenerator, the local
//   CMainGameComponent, vector and CMaterialDef) onto the shared headers; 0x0049D800 now binds ?GetLocalSeed@CWorld@@QBEPAKXZ (was
//   ?GetRandomGenerator@CEngineRandomOwner@@QAEPAUCRandomGenerator@@XZ), so the address has one spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CWORLD_GETLOCALSEED
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CMaterialDef.h"
#include "engine/GFRandom.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

long CMaterialDef::GetRandomBerserkedSwordHitParticleEffect() const
{
    if (!BerserkedSwordHitParticleEffect.empty()) {
        unsigned long* seed = CMainGameComponent::Get()->GetWorld().GetLocalSeed();
        return BerserkedSwordHitParticleEffect[GFRandom(BerserkedSwordHitParticleEffect.size(), seed)];
    }
    return 0;
}
