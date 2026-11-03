#ifndef FABLE_ENGINE_CCOUNTER_H
#define FABLE_ENGINE_CCOUNTER_H
#include "rebuild_abi.h"
// Original Ego_r c79d/FableWin c4c0 private long+0; native d90550/efd630.
class CCounter {
public:
    CCounter();
    CCounter& operator=(float);
    bool operator<=(float) const;
    bool operator>=(float) const;
    bool operator<(float) const;
    bool operator>(float) const;
    float GetCounter() const;
private:
    long GetFrameDelta() const;
    long RelevantWorldFrame;
};
FABLE_STATIC_ASSERT(sizeof(CCounter)==4);
#endif
