#include "rebuild_abi.h"
// Leaf: @FableIsDead_5cbdbd@4
// IS_THING_DEAD(CThing*) @ 0x005cbdbd
class CThing { public: char pad[0x91]; unsigned char m_flags; };
struct Wrap { CThing *m_p; };
extern "C" char FABLE_FASTCALL FableIsDead_5cbdbd(Wrap *w){
    CThing *p = w->m_p;
    if (p && !(p->m_flags & 1)) return 0;
    return 1;
}