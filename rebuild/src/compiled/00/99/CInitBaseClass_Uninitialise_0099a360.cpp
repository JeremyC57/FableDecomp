// CInitBaseClass::Uninitialise() -- if the object is initialised (Valid), decrement the global initialised-object counter and clear Valid
// Leaf: ?Uninitialise@CInitBaseClass@@IAEXXZ
// Identity: Ego_r.pdb ?Uninitialise@CInitBaseClass@@IAEXXZ (0x00B01FB0, beside Initialise IAE_N 0x00B01FA0) and FableWin.pdb
//   ?Uninitialise@CInitBaseClass@@IAEXXZ (0x03044AB0): protected, non-virtual. Retail 0x0099A360 follows Initialise 0x0099A350
//   (`mov al,1 ; mov [ecx+4],al`) and IsValid 0x0099A330 (`mov al,[ecx+4]`); derived Uninitialise bodies (CScratchBuffer
//   0x009F4950, CFrameRateSmoother 0x0049BA20, CPlayer 0x00487AF0, CSystemManager 0x009A60D0, CEngine 0x00B25D80) chain to it.
// Retail: `cmp byte [ecx+4],1 ; jne ret ; dec dword [0x013BCA14] ; mov byte [ecx+4],0 ; ret`.
// The counter 0x013BCA14 keeps the spelling this row already bound (`g_count`; reloc_bindings also lists g_initCount /
//   g_initialisedCount_013bca14 for it: an existing split, not widened here).
#define FABLE_RETAIL_CINITBASECLASS_UNINITIALISE
#include "engine/CInitBaseClass.h"

extern int g_count;                                      // 0x013BCA14

void CInitBaseClass::Uninitialise()
{
    if (Valid == true) {
        --g_count;
        Valid = false;
    }
}
