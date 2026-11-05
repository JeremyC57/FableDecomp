// Naming wave 18: the global stand-in g_root (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
// Leaf: ?CViewBase_GetDisplayEngine@@YIPAVCDisplayEngine@@PAUCViewBase@@@Z
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
class CDisplayEngine;
class CThingManager { public: char pad[0x1c]; CDisplayEngine* engine; };
class CThingManager;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#include "engine/CThing.h"
struct CViewBase {};
CDisplayEngine* __fastcall CViewBase_GetDisplayEngine(CViewBase* self)
{
    (void)self;
    return CThing::GetPCurrentThingManager()->engine;
}