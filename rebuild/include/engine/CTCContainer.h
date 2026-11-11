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
#ifdef FABLE_RETAIL_CTCCONTAINER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsComponentSerialisable() const;  // slot 23, 0x0074FCA0
public:
#endif
};
#endif
