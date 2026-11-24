// Leaf: ?InitStateGroup@CAIStateGroup_OpinionReactionRoot@@UAEXABVCWorld@@AAVCThingAICreature@@@Z
#include <string.h>

class CWorld;
class CThingAICreature;

class CAIStateGroup_OpinionReactionRoot { public:
    char pad[0x14];
    int CurrentReactionType;
    unsigned char states[79];
    virtual void InitStateGroup(const class CWorld&, class CThingAICreature&);
};

void CAIStateGroup_OpinionReactionRoot::InitStateGroup(const class CWorld& world, class CThingAICreature& creature)
{
    CAIStateGroup_OpinionReactionRoot* self = this;
    unsigned char* d = self->states;
    self->CurrentReactionType = 0;
    for (int i = 0; i < 79; ++i) d[i] = 1;
}
// Flatten route (flatten_pilot v4, NF324): CAIStateGroup_OpinionReactionRoot_InitStateGroup -> CAIStateGroup_OpinionReactionRoot::InitStateGroup, the RTTI-placed PDB member; class model: the row's local class model.
