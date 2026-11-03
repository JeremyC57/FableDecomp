#ifndef FABLE_ENGINE_CSCOPEDLOCKFLAGBOOL_H
#define FABLE_ENGINE_CSCOPEDLOCKFLAGBOOL_H
#include "rebuild_abi.h"
// Layout: original FableWin PDB class, four-byte bool reference Flag; native scoped true/false stores.
class CScopedLockFlagBool {
public:
 CScopedLockFlagBool(bool& flag):Flag(flag){Flag=true;}
 ~CScopedLockFlagBool(){Flag=false;}
private:bool& Flag;
};
FABLE_STATIC_ASSERT(sizeof(CScopedLockFlagBool)==4);
#endif
