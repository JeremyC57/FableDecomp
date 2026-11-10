#ifndef FABLE_ENGINE_THINGALIVE_H
#define FABLE_ENGINE_THINGALIVE_H
#include "fable_compiler.h"
// Retail opt-in (Codex pass 17): original IS_THING_ALIVE<T>(const T&),
// FableWin instantiation 01CCA150 called by the donor IsRunnable.
#ifdef FABLE_RETAIL_IS_THING_ALIVE
template<class T> inline bool FABLE_FASTCALL IS_THING_DEAD(const T& thing) {return thing==0||thing->IsDead();}
template<class T> inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
    if(thing!=0) {
        if(!thing->IsDead()) return true;
    }
    return false;
}
#endif
#endif
