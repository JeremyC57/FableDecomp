// CDefinitionManager::GetDef<CThingBaseDef>(long index, CDefPointer<const CThingBaseDef>* out) const -- the def at a global index into a typed
// counted pointer: false for a non-positive index or a missing def
// Leaf: ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042AF3C, 98 bytes, thiscall `ret 8`, frame-pointer size TU.
// Identity: FableWin.pdb ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z (const member
//   template, def pointer out by pointer); the landed caller 0x007CBDC0 binds this address under that symbol (reloc_bindings.tsv), and
//   CTCClock::OnCreate / CTCBed::OnCreate (RTTI-proven callers) and the FableWin UI-TU COMDAT order GetDef<CThingBaseDef> then
//   GetSubDef<CObjectAugmentationsDef> (= retail 0x0042AF3C / 0x0042AF9E) agree (NAME_FIXES_QUEUE 522 / 540 / 1011bi).
// Retail bytes:  push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[def] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: and dword ptr [esi],0 ; mov eax,[def] ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; jmp X ; F: xor al,al ; X: pop ebp ; ret 8
// Shape: the manager returns its CDefPointer<const CDefClassBase> by value through the hidden pointer, homed in the dead index slot and
//   re-read at every use; on a def, out->SetDynamicCast(def) (the retail re-reading form, FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD)
//   releases the old def, stores and addrefs, and the temporary's destructor releases. Frame pointer and size optimisation from the
//   owner TU's pragmas.
// Port (NF356, 2026-11-24; T3 nf349): was a local CDefinitionManager / CDefPointer model (generated for a repeated body shape) defining
//   the stand-in ?GetDef_0042af3c@CDefinitionManager@@QAE_NJAAV?$CDefPointer@$$CBVCDefClassBase@@@@@Z with no Leaf line. The body is now the shared
//   header's size-mode specialization FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGBASEDEF_SIZE_BODY (engine/CDefinitionManager.h, NF349; the
//   GETDEF_GLOBAL_BODY template text with T = CThingBaseDef), which the pick-any copies of the NF349 rows also compile. Its block sets
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
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGBASEDEF_SIZE_BODY
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared size-mode GetDef<CThingBaseDef> specialization so this object emits it
// out of line as a pick-any COMDAT (retail 0x0042AF3C is that out-of-line copy). Internal to this TU; never called.
bool (CDefinitionManager::* ReferenceGetDefCThingBaseDef())(long, CDefPointer<const CThingBaseDef>*) const
{
    return &CDefinitionManager::GetDef<CThingBaseDef>;
}
}
