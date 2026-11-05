class CMeshDataBank;
// Leaf: GetMeshBank
// Naming wave 18: the global stand-in g_animRoot (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
struct SubObj { char pad0[0x10]; CMeshDataBank* bank; };
class CThingManager { public: char pad0[0x1c]; SubObj* sub; };
class CThingManager;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#include "engine/CThing.h"
class CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    (void)self;
    return CThing::GetPCurrentThingManager()->sub->bank;
}