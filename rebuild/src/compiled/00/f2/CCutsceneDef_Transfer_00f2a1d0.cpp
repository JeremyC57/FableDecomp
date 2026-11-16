// CCutsceneDef::Transfer(CPersistContext& context) -- transfer the cutscene def's eight string lists (Macro, SkipCond, SetupCond, Lights, LightScene, Sound, Answer0, Answer1) through the binary vector transfer
// Leaf: ?Transfer@CCutsceneDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F2A1D0, 107 bytes (+ int3 padding), `ret 4`; slot 18 of the no-RTTI
//   CCutsceneDef vtable 0x012FB6E0 (its Sizeof slot 0x00F29D90 returns 0x9c; stored by the ctor at 0x00F29D2B). Ego_r.pdb
//   ?Transfer@CCutsceneDef@@UAEXAAVCPersistContext@@@Z. The catalog's "InitialiseDefaultCameraTags" is wrong.
// Callee: CPersistContext::TransferVectorBinary<CCharString>(vector<CCharString>&) @ 0x004331F9 (landed),
//   eight times. No base-class Transfer call.
// Layout: Ego_r.pdb CCutsceneDef (192 bytes) -> retail 0x9c: the CParentDefClassBase part is 0x3c in retail (as the landed
//   CFrontEndDef::Transfer 0x004331C0 models it, PDB 0x40) and each STLport vector is 12 bytes (PDB 16): Macro +0x3c,
//   SkipCond +0x48, Lights +0x54, LightScene +0x60, SetupCond +0x6c, Sound +0x78, Answer0 +0x84, Answer1 +0x90.
//   Virtual (2026-11-10, chunk 897 queue land1011j): the vtable slot makes it `virtual` (Ego_r.pdb UAE); the vptr is the
//   first word of the CParentDefClassBase part, so the opaque base bytes after it are 0x38.
// Shape: one call per member in source order (SetupCond is transferred third, before Lights), `this` in esi, the context
//   in edi; the last member address is formed by `add esi,90h`.
// Port (round 4, NF40): the local CPersistContext / def-class models (and any vector stand-ins) are replaced by
//   engine/CCutsceneDef.h and engine/CPersistContext.h; vector members go through the unnamed TransferVector<T>.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "engine/CCutsceneDef.h"
#include "engine/CPersistContext.h"

void CCutsceneDef::Transfer(CPersistContext& context)
{
    context.TransferVector(Macro);
    context.TransferVector(SkipCond);
    context.TransferVector(SetupCond);
    context.TransferVector(Lights);
    context.TransferVector(LightScene);
    context.TransferVector(Sound);
    context.TransferVector(Answer0);
    context.TransferVector(Answer1);
}