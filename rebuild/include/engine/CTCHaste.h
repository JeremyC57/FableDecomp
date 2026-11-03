#ifndef FABLE_ENGINE_CTCHASTE_H
#define FABLE_ENGINE_CTCHASTE_H
#include "engine/CTCBase.h"
// TPI0x2f115 original fields; native00667ff5 writes PauseFrames+14.
class CTCHaste : public CTCBase {
private:
    float Multiplier;bool ToTerminate;long PauseFrames;
    unsigned char Multipliers[12]; // std::map<void*,float>, retail STLport.
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_HASTE; }
    void SetOnHitPause(long frames) { PauseFrames=frames; }
};
FABLE_STATIC_ASSERT(sizeof(CTCHaste)==0x24);

#endif
