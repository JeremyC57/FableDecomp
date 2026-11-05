// GetDisplayEngine @ 0x004c7aa0 (CViewBase)
// Leaf: GetDisplayEngine
// Naming wave 18: the global stand-in g_pViewBase (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail:
//   mov eax, ds:[0x013b8a1c]  ; load global singleton pointer
//   mov eax, [eax+0x24]       ; return field at +0x24
//   ret
// Idiomatic: a global CViewBase* singleton; return m_pDisplayEngine at +0x24.

class CDisplayEngine;

class CThingManager { public:
    char pad[0x24];
    CDisplayEngine* m_pDisplayEngine; // +0x24
};

// Global singleton pointer (reloc-masked extern -> ds:[imm32]).
class CThingManager;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#include "engine/CThing.h"

CDisplayEngine* GetDisplayEngine()
{
    return CThing::GetPCurrentThingManager()->m_pDisplayEngine;
}