#ifndef FABLE_ENGINE_NUISYSTEM_CLISTSTATE_H
#define FABLE_ENGINE_NUISYSTEM_CLISTSTATE_H
#include "stlport/fable_containers.h"
// Ego_r TPI 0x2b4bd and FableWin SaveState use this single public list.
namespace NUISystem {
struct CListState {
    std::list<std::pair<long, long> > Structure;
};
}
#endif
