// GFLoadScriptDef_00cbf647() -- resolve the "SCRIPT_DEF" definition into the global def pointer @ 0x0143E90C through the definition manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF647, length 50 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SCRIPT_DEF" @ 0x012C18DC, -1),
// GFGetPDefinitionManager @ 0x00CD3EF0 (`mov eax,[0143E920h]`; the manifest's "GFGetPreMainMemoryUsed" is a
// propagated label), CDefinitionManager::GetDef(const CCharString&, CDefPointer&) @ 0x00CD2994 (__thiscall, the
// arguments pushed BEFORE the manager getter runs: VC evaluates the arguments, then the object),
// CCharString::~CCharString @ 0x0099EAE0.
// A free function of the script region (the caller @ 0x00CD424C is a bare `call`, ecx unused), compiled with
// frame pointers kept (`push ebp ; mov ebp,esp ; push ecx ; ... ; leave`): the manifest's "CCombatAction_
// Guildmaster_HealFollowersRelease::Initialise" is a propagated label. Pragmas: optimize("gsy", off) + optimize("s", on), as the landed 0x0066106D family.
#pragma optimize("gsy", off)            // no global optimisation, frame pointer kept (the `lea` stays after the first push)
#pragma optimize("s", on)               // size: `leave` epilogue
#include "fable_string.h"

struct CDefPointer;
extern CDefPointer g_scriptDef;                 // 0x0143E90C

struct CDefinitionManager_Calls {              // engine/CDefinitionManager.h exists but is not used: only its GetDef call is used
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x00CD2994
};

CDefinitionManager_Calls* GFGetPDefinitionManager();  // 0x00CD3EF0

void GFLoadScriptDef_00cbf647()
{
    CCharString name("SCRIPT_DEF", -1);
    GFGetPDefinitionManager()->GetDef(name, g_scriptDef);
}
