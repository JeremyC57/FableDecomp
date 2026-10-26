// CWorld::SetCreatureGenerationGroupAsEnabled(ECreatureGroup group, bool enabled) -- enable a creature-generation group by clearing its bits in the world's disabled-groups mask, or disable it by setting them
// Leaf: ?SetCreatureGenerationGroupAsEnabled@CWorld@@QAEXW4ECreatureGroup@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049D930, 52 bytes (+12 int3 padding), thiscall, `ret 8`.
//   Ego_r.pdb ?SetCreatureGenerationGroupAsEnabled@CWorld@@QAEXW4ECreatureGroup@@_N@Z (0x004A6C90, right after
//   IsCreatureGenerationEnabledForGroup -- retail 0x0049D910, landed, which tests the same +0xD0 mask).
// Layout: local model. engine/CWorld.h (Ego_r.pdb) puts CreatureGenerationDisabledGroups at +0xDC; retail reads +0xD0
//   (the three containers at +0xAC..+0xD8 are 4 bytes smaller each in retail), so the header is not used here.
// Shape: two arms, each a full read-modify-write of the mask (`not ; and` for enable, `or` for disable).
// Layout (supersedes any local-model note above): engine/CWorld.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CWorld CWorldDonor
#include "engine/CWorld.h"
#undef CWorld
enum ECreatureGroup { CREATURE_GROUP_NONE };

class CWorld : public CWorldDonor {
public:
    void SetCreatureGenerationGroupAsEnabled(ECreatureGroup group, bool enabled);
};

void CWorld::SetCreatureGenerationGroupAsEnabled(ECreatureGroup group, bool enabled)
{
    if (enabled)
        CreatureGenerationDisabledGroups &= ~group;
    else
        CreatureGenerationDisabledGroups |= group;
}