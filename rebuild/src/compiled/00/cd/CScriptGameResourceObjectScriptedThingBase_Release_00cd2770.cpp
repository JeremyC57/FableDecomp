#pragma optimize("s",on)
// CScriptGameResourceObjectScriptedThingBase::Release() -- drop the implementation pointer: PImp's ReleaseData, then PImp.Data cleared
// Leaf: ?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD2770: `push esi ; lea esi,[ecx+8] ; mov ecx,esi ; call 0x007E70E0 ; and dword ptr [esi],0 ; pop esi ; ret`.
// NF28 (2026-11-12, queue land1011ei): was landed as a free __fastcall Clear under the propagated label CMemoryDataOutputStream::Clear.
//   Identity: manifest functions.tsv 0x00CD2770 = Release / CScriptGameResourceObjectScriptedThingBase; Ego_r.pdb 0x00CB4574 and FableWin.pdb
//   0x035C5B25 ?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ; engine/CScriptGameResourceObjectScriptedThingBase.h declares it at
//   0x00CD2770 (PImp +0x08, release 0x007E70E0 = CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::ReleaseData); every entity
//   Main calls it on MyEntityHandle after IsValid() (0x00EB3FA1; landed 0x00D37330, 0x00EB53A0 bind this spelling).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

void CScriptGameResourceObjectScriptedThingBase::Release()
{
    PImp.ReleaseData();
    PImp.Data = 0;
}
