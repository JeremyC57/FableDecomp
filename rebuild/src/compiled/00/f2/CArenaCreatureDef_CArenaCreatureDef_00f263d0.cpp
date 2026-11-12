// CArenaCreatureDef::CArenaCreatureDef() -- the CSubDefClassBase base constructor (expanded around the out-of-line CDefClassBase ctor), this class's vtable, then the two CCharString members CreatureType +0x28 and HUDType +0x30 default-constructed (NumCreatures / DeathScore left uninitialised)
// Leaf: ??0CArenaCreatureDef@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F263D0, length 48 bytes.
// Name fix (agentNF24, land1011de): was the stand-in CFireheartMinigameDef_00f263d0::CFireheartMinigameDef_00f263d0 over a CPSCBase
//   stand-in base. Vtable 0x012FB358 is CArenaCreatureDef's (agentFAM08: also stored by the copy constructor 0x00F25DD0,
//   ??0CArenaCreatureDef@@QAE@ABV0@@Z; slots 18/19/20 Transfer / Copy / GetSizeofClass; engine/CArenaCreatureDef.h names this address
//   its default constructor); Ego_r.pdb CArenaCreatureDef (0x38: CreatureType +0x28, NumCreatures +0x2C, HUDType +0x30, DeathScore +0x34).
// Callees: CDefClassBase::CDefClassBase @ 0x0042D9E0 (FableWin ??0CDefClassBase@@AAE@XZ), CCharString::CCharString @ 0x0099E4B0 (x2).
// Shape: the CSubDefClassBase ctor is expanded (its vtable store is dead under this class's and drops out), as in the landed
//   CConversation::CConversation 0x00F336F0; the first member address is formed before the vtable store; `this` is returned.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CARENACREATUREDEF_IMPLICIT_DTOR
#include "engine/CArenaCreatureDef.h"

CArenaCreatureDef::CArenaCreatureDef()
{
}
