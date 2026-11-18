// CTCTargetingPlayer::`scalar deleting destructor'(unsigned int) -- ~CTCTargetingPlayer, then free the object on flag bit 0
// Leaf: ?CTCTargetingPlayer_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E37A3, 28 bytes, `ret 4`; size-optimised TU.
// agentNF69 (q10854): the destructor it calls, 0x004E37BF, is ??1CTCTargetingPlayer@@UAE@XZ (Ego_r.pdb / FableWin.pdb; the implicit
//   destructor of engine/CTCTargetingPlayer.h's retail class, see that row), called through the shared header instead of the
//   CTCTargetingPlayer_Destruct stand-in. The function's own spelling stays the landed free-function form (catalog name
//   CTCTargetingPlayer::`scalar_deleting_destructor'); emitting the compiler's ??_GCTCTargetingPlayer@@UAEPAXI@Z needs a TU that emits the
//   class's vtable (its constructor), which is not reconstructed (NOTES).
#pragma optimize("s", on)
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGBASE_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#include "engine/CThing.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTargetingDef.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTargetingPlayer.h"

void* __fastcall CTCTargetingPlayer_ScalarDeletingDestructor(void* player, int, unsigned int flags)
{
    static_cast<CTCTargetingPlayer*>(player)->CTCTargetingPlayer::~CTCTargetingPlayer();
    if (flags & 1)
        operator delete(player);
    return player;
}
