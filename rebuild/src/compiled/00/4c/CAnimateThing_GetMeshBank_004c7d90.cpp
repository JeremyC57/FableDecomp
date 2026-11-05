// Naming wave 18: the global stand-in g_root (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
// Leaf: ?CAnimateThing_GetMeshBank@@YIPAVCMeshDataBank@@PAVCAnimateThing@@@Z
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
class CMeshDataBank;
struct SubObj { char pad[0x161c8]; CMeshDataBank* bank; };
class CThingManager { public: char pad[0x1c]; SubObj* sub; };
class CThingManager;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#include "engine/CThing.h"
class CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    return CThing::GetPCurrentThingManager()->sub->bank;
}