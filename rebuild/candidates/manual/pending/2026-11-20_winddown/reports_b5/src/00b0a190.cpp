// NParticleEngine::CPSCSingleSprite::DatabaseClone() const -- allocate a fresh component of this class (Alloc 0x00AF7DF0) and copy this one into it (virtual slot 18 = DatabaseCopy), returning the copy
// Leaf: ?DatabaseClone@CPSCSingleSprite@NParticleEngine@@UBEPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B0A190, 24 bytes: push esi ; mov esi,ecx ; call Alloc ; mov edx,[eax] ; push esi ; mov ecx,eax ;
//   call [edx+48h] ; ... ret.
// Identity: vtable slot 2 of NParticleEngine::CPSCSingleSprite (0x0129FB84); Ego_r.pdb ?DatabaseClone@CPSCSingleSprite@NParticleEngine@@UBEPAVCPSCBase@2@XZ;
//   Ego_d.pdb local `psc` (debug length 41). Slot 18 (+0x48) is CPSCBase::DatabaseCopy (engine/CPSCBase.h), the sprite override 0x00B05EA0.
// Re-port (Account B, 2026-11-20): on the shared engine/CPSCSingleSprite.h. The landed text was a stand-in (an IDrawableObj model with
//   the slot called through a member-function-pointer table and Alloc bound as `GetDrawableObject_00af7df0`).
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#include "engine/CPSCSingleSprite.h"
namespace NParticleEngine {
CPSCBase* CPSCSingleSprite::DatabaseClone() const
{
    CPSCBase* psc = Alloc();
    psc->DatabaseCopy(*this);
    return psc;
}
}
