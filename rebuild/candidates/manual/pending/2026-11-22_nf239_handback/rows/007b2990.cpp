// CTCGuard::GetWaitForRebribeInSeconds() const -- the seconds a guard waits before he can be bribed again (a constant 15)
// Leaf: ?GetWaitForRebribeInSeconds@CTCGuard@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B2990, 6 bytes: `mov eax,0Fh ; ret`.
// Name (NF239, 2026-11-22, CODEXP27 L61): Ego_r + FableWin publics ?GetWaitForRebribeInSeconds@CTCGuard@@QBEJXZ (Ego_r 0x007B3CF0 /
//   FableWin 0x026DE2C0); the landed caller 0x009334F0 (CAIStateGroup_MinionGuardReactToBribe::Update) already binds it here and uses
//   the result twice (the full and the half waiting period). engine/CTCGuard.h declares it (FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27).
//   Was the stand-in ?AddChildPrimitive@C@@QAEHXZ (catalog _global::ReturnConst_15_007b2990, a byte-twin label).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCGuard.h"
long CTCGuard::GetWaitForRebribeInSeconds() const
{
    return 15;
}
