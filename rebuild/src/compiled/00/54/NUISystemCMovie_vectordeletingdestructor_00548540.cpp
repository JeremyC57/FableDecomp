// NUISystem::CMovie::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x00548550
// Leaf: ??_ECMovie@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00548540, 8 bytes: `sub ecx,18h ; jmp 0x00548550`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CMovie's CActionParam vtable 0x012487EC at +0x18 (rtti-vtables.tsv, RTTI .?AVCMovie@NUISystem@@; the catalog's
//   "vfunc_0"), and its jump target 0x00548550 is slot 0 of the primary vtable 0x0124881C, the class's ??_G. VC7.1 names the slot-0
//   entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4, WBI@ = 24).
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU:
//   CMovie(CIEngine&, long) (0x005482D0: CChangingStateComponent(long) 0x0052CC50, the vptrs, Playing cleared, the Handle / Movie /
//   Playback members constructed, AttempToPlay cleared, the static engine pointer stored, GetDataFromDef 0x00547C10). That function
//   is not graded here; the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCMovie@NUISystem@@UAEPAXI@Z 0x00548550 (bound as its ??_E alias).
// Header: engine/CMovie.h (FABLE_RETAIL_CMOVIE + FABLE_RETAIL_CMOVIE_MEMBERS).
// Twin: 0x00548530 is the other adjustor thunk of the same vtable set (same TU).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMOVIE
#define FABLE_RETAIL_CMOVIE_MEMBERS
#include "engine/CMovie.h"
namespace NUISystem {
CMovie::CMovie(CIEngine& engine, long def)
    : CChangingStateComponent(def)
    , Playing(false)
    , AttempToPlay(false)
{
    PEngine = &engine;
    GetDataFromDef();
}
}