// CASoundManager::CanAllocateEmitter() -- base CASoundManager::CanAllocateEmitter
// Name (agentSND, 2026-11-10): vtable slot 6 (+0x18); spelling from the Fable Anniversary 360 PDB publics (CSoundManagerUN / CASoundManager at the same vtable position) with the TLC slot evidence in SLOTS.tsv; was CCreatureActionBase::GetAnimStartFrame.
struct CASoundManager { unsigned int (__fastcall** vtbl)(CASoundManager*); char pad[0x18-4]; unsigned int f18; int CanAllocateEmitter(); };
// Leaf: ?CanAllocateEmitter@CASoundManager@@QAEHXZ
int CASoundManager::CanAllocateEmitter(){ unsigned int r=this->vtbl[0x28/4](this); return (unsigned int)r < this->f18 ? 1 : 0; }