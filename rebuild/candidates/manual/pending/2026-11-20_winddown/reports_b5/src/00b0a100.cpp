// NParticleEngine::CPSCSingleSprite::GetComponentSubType() const -- returns (EPSCSubType)7
// Leaf: ?GetComponentSubType@CPSCSingleSprite@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A100, thiscall, `ret`.
// Identity: slot 5 of NParticleEngine::CPSCSingleSprite's vtable 0x0129FB84 (defined here: the slot holds this body in 1 vtable(s), all of NParticleEngine::CPSCSingleSprite or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 13 TLC-named slots of that vtable puts GetComponentSubType(void) at this slot; Ego_r/FableWin name set: ?GetComponentSubType@CPSCSingleSprite@NParticleEngine@@UBE?AW4EPSCSubType@2@XZ; `ret` matches the signature.
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#include "engine/CPSCSingleSprite.h"
namespace NParticleEngine {
NParticleEngine::EPSCSubType CPSCSingleSprite::GetComponentSubType() const
{
    return (EPSCSubType)7;
}
}
