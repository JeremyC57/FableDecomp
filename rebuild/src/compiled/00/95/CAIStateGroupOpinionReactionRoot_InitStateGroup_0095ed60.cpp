// Leaf: ?CAIStateGroup_OpinionReactionRoot_InitStateGroup@@YIXPAVCAIStateGroup_OpinionReactionRoot@@HABVCWorld@@AAVCThingAICreature@@@Z
#include <string.h>

class CWorld;
class CThingAICreature;

class CAIStateGroup_OpinionReactionRoot { public:
    char pad[0x18];
    int CurrentReactionType;
    unsigned char states[79];
};

void __fastcall CAIStateGroup_OpinionReactionRoot_InitStateGroup(CAIStateGroup_OpinionReactionRoot* self, int edx_dummy,
                               const CWorld& world, CThingAICreature& creature)
{
    unsigned char* d = self->states;
    self->CurrentReactionType = 0;
    for (int i = 0; i < 79; ++i) d[i] = 1;
}