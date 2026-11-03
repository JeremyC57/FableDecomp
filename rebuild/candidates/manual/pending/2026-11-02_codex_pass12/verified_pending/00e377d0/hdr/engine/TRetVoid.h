#ifndef FABLE_ENGINE_TRETVOID_H
#define FABLE_ENGINE_TRETVOID_H
#include "rebuild_abi.h"
// Layout: FableWin TPI fa51 proves the original empty class, sizeof 1.
// Original script_language.hpp:23 has the public void Return() member.
// The 11-byte debug body has only this; retail generated-script returns have no helper operation.
class TRetVoid {
public:
    void Return() { }
};
FABLE_STATIC_ASSERT(sizeof(TRetVoid)==1);
#endif
