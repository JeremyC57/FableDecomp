#ifndef FABLE_ENGINE_THINGALIVE_H
#define FABLE_ENGINE_THINGALIVE_H
#include "fable_compiler.h"
// Retail opt-in (Codex pass 17): original IS_THING_ALIVE<T>(const T&),
// FableWin instantiation 01CCA150 called by the donor IsRunnable.
#ifdef FABLE_RETAIL_IS_THING_ALIVE
template<class T> inline bool FABLE_FASTCALL IS_THING_DEAD(const T& thing) {return thing==0||thing->IsDead();}
#if defined(FABLE_RETAIL_IS_THING_ALIVE_NOINLINE) && defined(_MSC_VER)
// Per-TU opt-in (chunk 1021, row 0x0076D2F0 CTCEnemy::AreThingsEnemies): the same body kept out of line, as that TU calls the
// IS_THING_ALIVE<const CThingPlayerCreature*> COMDAT 0x0043DBF0 (`lea ecx,[esp+1Ch] ; call 0x0043DBF0`, 0x0076D3C2). The visible
// body tells VC7.1 the callee keeps no reference to the caller's pointer local, so its frame word is shared, as retail does.
template<class T> __declspec(noinline) bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
#else
template<class T> inline bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing) {
#endif
    if(thing!=0) {
        if(!thing->IsDead()) return true;
    }
    return false;
}
#endif
#endif
