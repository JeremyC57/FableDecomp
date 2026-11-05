// CDefString::CDefString() -- default def string: no string-table entry yet (TablePos = -1)
// Leaf: ??0CDefString@@QAE@XZ
// Retail: Fable.exe @ 0x007E37C0, 9 bytes, __thiscall: `mov eax,ecx ; mov dword ptr [eax],0FFFFFFFFh ; ret`.
// Name: Ego_r.pdb and FableWin.pdb both carry ??0CDefString@@QAE@XZ; CDefString is one `long TablePos` (+0x00, PDB layout).
// Retail keeps this out-of-line COMDAT for the NScript TUs that build the default value of
// CPersistContext::Transfer<CDefString> (0x007E39D0): e.g. 0x00F2F48C `lea ecx,[esp+14h] ; call 0x007E37C0 ; push eax`, then the
// member address and a key such as "BeardyBaldyHaircutStyle1" (0x013001F4). 35 call sites, 0x00F2EA95..0x00F2F55B.
// The shared header's ctor is inline; FABLE_RETAIL_CDEFSTRING_DEFAULTCTOR_OUTOFLINE declares it out of line for this TU.
// History: landed 2026-10 as `CVtable_ffffffff::CVtable_ffffffff`, a polymorphic stand-in whose vtable store matched retail's
// literal -1 only because the grader masks relocations (a reloc-masked false match). Re-landed 2026-11-05 as the real ctor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_DEFAULTCTOR_OUTOFLINE
#include "engine/CDefString.h"

CDefString::CDefString()
    : TablePos(-1)
{
}
