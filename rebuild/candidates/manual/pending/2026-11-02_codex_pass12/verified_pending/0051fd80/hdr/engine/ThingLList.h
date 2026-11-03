#ifndef FABLE_ENGINE_THINGLLIST_H
#define FABLE_ENGINE_THINGLLIST_H
class CThing;
// Original struct key and fields; native vector element stride8.
struct ThingLList { CThing* Head; CThing* Tail; };
FABLE_STATIC_ASSERT(sizeof(ThingLList)==8);
#endif
