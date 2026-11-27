#ifndef FABLE_ENGINE_CTCCREATUREITEMLEVITATIONBASE_H
#define FABLE_ENGINE_CTCCREATUREITEMLEVITATIONBASE_H
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
class CTCCreatureItemLevitationBase : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_CREATURE_ITEM_LEVITATION; }
    enum ELevitationLeaves { LEVITATION_HERO_PICK_UP_FEED_BACK, LEVITATION_NYMPH_THEFT };
    virtual ELevitationLeaves GetLeafType() const=0;
    virtual void DropItems();
protected:
    virtual long GetMaxNumItems() const=0;
    virtual void LeafUpdate();
private:
    std::list<CThing*> LevitatingItems;
    long NumItems;
    long LastAddFrame;
};
#endif
