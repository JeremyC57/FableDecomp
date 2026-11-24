// CDefinitionManager::GetDef(long index, CDefPointer* out) const -- GetDef<NUISystem::CUIMiscThingsDef>: the def at a global index into a
// typed counted pointer: false for a non-positive index or a missing def
// Leaf: ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042B312, 98 bytes, thiscall `ret 8`, frame-pointer size TU.
// Name (agentNF32, 2026-11-12; was the placeholder CDefinitionManager::GetDef_0042b312(long, CDefPointer<const CDefClassBase>&)):
//   FableWin.pdb 0x01A526F0 ??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@@Z
//   (const member template, def pointer out by pointer); the landed 0x0047A9F0 (CGameDefinitionManager::Get) binds this address under
//   that symbol (rebuild/oracles/reloc_bindings.tsv).
// Retail bytes:  push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[def] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: and dword ptr [esi],0 ; mov eax,[def] ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; jmp X ; F: xor al,al ; X: pop ebp ; ret 8
// Shape: the manager returns its CDefPointer<const CDefClassBase> by value through the hidden pointer, homed in the dead index slot and
//   re-read at every use; on a def, out->SetDynamicCast(def) (the retail re-reading form, FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD)
//   releases the old def, stores and addrefs, and the temporary's destructor releases. Frame pointer and size optimisation from the
//   owner TU's pragmas.
// Port (NF356, 2026-11-24; T3 nf349): was a local CDefinitionManager / CDefPointer model explicitly instantiated (generated for a
//   repeated body shape). The def type is only forward-declared (the block declares NUISystem::CUIMiscThingsDef); engine/CDefClassBase.h
//   completes CDefClassBase for the temporary (engine/NUISystem_CUIMiscThingsDef.h would add /DEFAULTLIB:"libcp" to .drectve). The body is now the shared
//   header's size-mode specialization FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CUIMISCTHINGSDEF_SIZE_BODY (engine/CDefinitionManager.h, NF349; the
//   GETDEF_GLOBAL_BODY template text with T = CUIMiscThingsDef), which the pick-any copies of the NF349 rows also compile. Its block sets
//   optimize("y", off) / ("s", on) and ends with optimize("", on), so this TU sets its own two pragmas AFTER the includes (nf189), keeping
//   the rest of the TU in the size mode it had. The specialization is `inline`, and VC7.1 rejects an explicit instantiation of a member
//   template's explicit specialization (C2945), so the anonymous-namespace address helper below makes this object emit it out of line as
//   a pick-any COMDAT (GOTCHAS nf86 / nf88; the landing nf135 owner form, 0x008C7A70). Leaf bytes and relocations are unchanged.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CUIMISCTHINGSDEF_SIZE_BODY
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared size-mode GetDef<CUIMiscThingsDef> specialization so this object emits it
// out of line as a pick-any COMDAT (retail 0x0042B312 is that out-of-line copy). Internal to this TU; never called.
bool (CDefinitionManager::* ReferenceGetDefCUIMiscThingsDef())(long, CDefPointer<const NUISystem::CUIMiscThingsDef>*) const
{
    return &CDefinitionManager::GetDef<NUISystem::CUIMiscThingsDef>;
}
}
