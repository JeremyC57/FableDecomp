// SetGlobalScriptDef() -- resolve the "SCRIPT_DEF" definition into the global def pointer @ 0x0143E90C through the definition manager
// Leaf: ?SetGlobalScriptDef@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF647, length 50 bytes.
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?SetGlobalScriptDef@@YIXXZ (a __fastcall free function: ecx is unused, so the
//   bytes equal the cdecl form; labels_chunk691; the caller 0x00CD3F50 binds it); the def pointer is the ESTABLISHED
//   ?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A and the lookup FableWin's
//   ??$GetDef@VCScriptDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCScriptDef@@@@@Z. Was GFLoadScriptDef_00cbf647.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SCRIPT_DEF" @ 0x012C18DC, -1),
// GFGetPDefinitionManager @ 0x00CD3EF0 (`mov eax,[0143E920h]`; the manifest's "GFGetPreMainMemoryUsed" is a
// propagated label), CDefinitionManager::GetDef(const CCharString&, CDefPointer&) @ 0x00CD2994 (__fastcall, the
// arguments pushed BEFORE the manager getter runs: VC evaluates the arguments, then the object),
// CCharString::~CCharString @ 0x0099EAE0.
// A free function of the script region (the caller @ 0x00CD424C is a bare `call`, ecx unused), compiled with
// frame pointers kept (`push ebp ; mov ebp,esp ; push ecx ; ... ; leave`): the manifest's "CCombatAction_
// Guildmaster_HealFollowersRelease::Initialise" is a propagated label. Pragmas: optimize("gsy", off) + optimize("s", on), as the landed 0x0066106D family.
#pragma optimize("gsy", off)            // no global optimisation, frame pointer kept (the `lea` stays after the first push)
#pragma optimize("s", on)               // size: `leave` epilogue
#include "fable_string.h"

class CScriptDef;
template <class T> class CDefPointer;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;  // 0x0143E90C

#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefinitionManager : public CDefinitionManagerLayout {
public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* def) const;   // 0x00CD2994 (<CScriptDef>)
};

CDefinitionManager* GFGetPDefinitionManager();          // 0x00CD3EF0

void __fastcall SetGlobalScriptDef()
{
    CCharString name("SCRIPT_DEF", -1);
    GFGetPDefinitionManager()->GetDef<CScriptDef>(name, &pGlobalScriptDef);
}