#ifndef FABLE_ENGINE_CTCCONTAINER_H
#define FABLE_ENGINE_CTCCONTAINER_H
#include "engine/CTCBase.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
// Layout: native RTTI1269424 places CTCContainer between RewardHero and CTCBase.
// Ego_r ContainerItems+12; native STLport vector is12 bytes rather than donor16.
class CTCContainer : public CTCBase {
protected:
    std::vector<std::pair<long,CCharString> > ContainerItems;
};
#endif
