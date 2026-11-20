// NParticleEngine::CPSCSingleSprite::Alloc() -- the component factory: a new CPSCSingleSprite (default constructor 0x00B09360, out of line)
// Leaf: ?Alloc@CPSCSingleSprite@NParticleEngine@@SIPAVCPSCBase@2@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF7DF0, 27 bytes: push 0x138 ; call operator new ; test eax,eax ; pop ecx ; je N ;
//   mov ecx,eax ; jmp CPSCSingleSprite::CPSCSingleSprite ; N: xor eax,eax ; ret
// Identity: Ego_r.pdb ?Alloc@CPSCSingleSprite@NParticleEngine@@SIPAVCPSCBase@2@XZ (public static __fastcall, returns CPSCBase*); the
//   particle-component registry (CPSCManager::RegisterPSCs 0x00AF8170) stores it, and DatabaseClone 0x00B0A190 calls it.
// Re-port (Account B, 2026-11-20): on the shared engine/CPSCSingleSprite.h (sizeof 0x138 asserted there). The landed text was a free
//   cdecl stand-in `CPSCSingleSprite* CPSCSingleSprite_Alloc()` over a 0x138-byte struct (no arguments, so the same code).
#define FABLE_RETAIL_CPSCSPLINEBASE_PASS23
#define FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS
#include "engine/CPSCSingleSprite.h"
namespace NParticleEngine {
CPSCBase* FABLE_FASTCALL CPSCSingleSprite::Alloc()
{
    return new CPSCSingleSprite;
}
}
