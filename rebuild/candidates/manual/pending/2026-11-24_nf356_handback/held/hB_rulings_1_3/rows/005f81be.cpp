// CDefinitionManager::GetDef<CExpressionDef>(long, CDefPointer<const CExpressionDef>*) const -- GetPDefFromGlobalIndex @ 0x009AD9E0 returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer is reassigned (old def released, new one addref'd) and the temporary released; true, else false; frame-pointer size TU
// Leaf: ??$GetDef@VCExpressionDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCExpressionDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F81BE, oracle length 98 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[def] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: and dword ptr [esi],0 ; mov eax,[def] ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; jmp X ; F: xor al,al ; X: pop ebp ; ret 8
// Definition lookup into a counted def pointer: GetPDefFromGlobalIndex @ 0x009AD9E0 returns a CDefPointer by
// value through the hidden pointer, which VC homes in the dead key argument slot; the
// temporary is re-read from that slot at every use. A null def fails (the false path sits at
// the end: the success branch is the `if` body). Otherwise `out = def` is inlined -- unless
// the out pointer already holds the def, its old def is released through CDefBase::Release
// (`this` in ecx, `dec [ecx+4] ; jne`, virtual slot 1 destroys at zero) and nulled, the new one
// stored and addref'd -- and the temporary's destructor releases the def through the
// ref-count base's two inline accessors (decrement, then test the count): the adjusted base
// pointer is materialised once for both (`add eax,4`), the stored value is forwarded to the
// test (re-read in the size TU: `dec [eax] ; cmp [eax],0`) and the def is kept in ecx for the virtual call. Frame pointer and size optimisation from the TU's pragmas. The manifest
// label names one instantiation; the def type is opaque. One of 13 bodies of this shape.
// Name (round 4, NF40): the member template instance ??$GetDef@VCExpressionDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCExpressionDef@@@@@Z (FableWin.pdb 0x01DFC5A0; the
//   landed callers 0x005F4C8F, 0x006AC430 and 0x007EEFE0 already bind this spelling here). Was the stand-in GetDef_005f81be on
//   a local CDefinitionManager / CDefPointer model; now the header's global-index body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY,
//   native 0x00513160) instantiated for CExpressionDef.
// Port (NF356, 2026-11-24; T3 nf349): the body is now the shared header's size-mode specialization
//   FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CEXPRESSIONDEF_SIZE_BODY (engine/CDefinitionManager.h, NF349; the GETDEF_GLOBAL_BODY template
//   text with T = CExpressionDef, token for token), which the pick-any copy of 0x006AC430 also compiles. Its block ends with
//   optimize("", on), so this TU's two pragmas moved below the includes (nf189). VC7.1 rejects an explicit instantiation of a member
//   template's explicit specialization (C2945), so the anonymous-namespace address helper below emits it out of line as a pick-any COMDAT
//   (GOTCHAS nf86 / nf88; the landing nf135 owner form, 0x008C7A70). Leaf bytes and relocations are unchanged.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CEXPRESSIONDEF_SIZE_BODY
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#include "engine/CExpressionDef.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace {
// Fidelity: scaffolding, not retail: takes the address of the shared size-mode GetDef<CExpressionDef> specialization so this object emits
// it out of line as a pick-any COMDAT (retail 0x005F81BE is that out-of-line copy). Internal to this TU; never called.
bool (CDefinitionManager::* ReferenceGetDefCExpressionDef())(long, CDefPointer<const CExpressionDef>*) const
{
    return &CDefinitionManager::GetDef<CExpressionDef>;
}
}
