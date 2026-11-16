// CDefinitionManager::GetDef<CHeroStatsDef>(const CCharString&, long, CDefPointer<const CHeroStatsDef>*) const -- GetPDefFromClassIndex @ 0x009ADE10 returns the def pointer through the hidden pointer homed in the dead key slot; on a hit the out pointer is reassigned (old def released, new one addref'd) and the temporary released; true, else false; frame-pointer size TU
// Leaf: ??$GetDef@VCHeroStatsDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCHeroStatsDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004513B9, oracle length 101 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; cmp dword ptr [index],0 ; jle F ; push [index] ; lea eax,[index] ; push [kind] ; push eax ; call GetPDefFromGlobalIndex ; mov eax,[def] ; test eax,eax ; je F ; push esi ; mov esi,[out] ; mov ecx,[esi] ; cmp ecx,eax ; je K ; test ecx,ecx ; je A ; dec dword ptr [ecx+4] ; jne Z ; mov eax,[ecx] ; call [eax+4] ; Z: and dword ptr [esi],0 ; mov eax,[def] ; A: test eax,eax ; mov [esi],eax ; je R ; inc dword ptr [eax+4] ; mov eax,[def] ; K: mov ecx,eax ; add eax,4 ; dec dword ptr [eax] ; cmp dword ptr [eax],0 ; jne R ; mov eax,[ecx] ; call [eax+4] ; R: mov al,1 ; pop esi ; jmp X ; F: xor al,al ; X: pop ebp ; ret 0Ch
// Definition lookup into a counted def pointer: GetPDefFromClassIndex @ 0x009ADE10 returns a CDefPointer by
// value through the hidden pointer, which VC homes in the dead key argument slot; the
// temporary is re-read from that slot at every use. A null def fails (the false path sits at
// the end: the success branch is the `if` body). Otherwise `out = def` is inlined -- unless
// the out pointer already holds the def, its old def is released through CDefBase::Release
// (`this` in ecx, `dec [ecx+4] ; jne`, virtual slot 1 destroys at zero) and nulled, the new one
// stored and addref'd -- and the temporary's destructor releases the def through the
// ref-count base's two inline accessors (decrement, then test the count): the adjusted base
// pointer is materialised once for both (`add eax,4`), the stored value is forwarded to the
// test (re-read in the size TU: `dec [eax] ; cmp [eax],0`) and the def is kept in ecx for the virtual call. Frame pointer and size optimisation from the TU's pragmas. The manifest
// label names one instantiation; the def type is opaque. One of 6 bodies of this shape.
// Name (round 4, NF40): the member template instance ??$GetDef@VCHeroStatsDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCHeroStatsDef@@@@@Z (Ego_r.pdb / FableWin.pdb name sets; the landed
//   caller row 0x0044C72B already binds this spelling here; FableWin orders GetDef<CEnvironmentDef> 0x0199E1AB before
//   GetDef<CHeroStatsDef> 0x0199E24E as retail 0x00451354 / 0x004513B9). Was the stand-in GetDef_004513b9 on a local
//   CDefinitionManager / CDefPointer model; now the header's by-class-index body (FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY with
//   FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD, the SetDynamicCast form of 0x006B25E0) instantiated for CHeroStatsDef.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#include "engine/CHeroStatsDef.h"

template bool CDefinitionManager::GetDef<CHeroStatsDef>(const CCharString&, long, CDefPointer<const CHeroStatsDef>*) const;
