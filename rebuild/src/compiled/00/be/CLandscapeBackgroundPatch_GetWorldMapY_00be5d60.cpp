// CLandscapeBackgroundPatch::GetWorldMapY() -- the patch's Y in world grid units: its map's world Y origin (CEngineMap::WorldPosY) plus the patch's own MapY
// Leaf: GetWorldMapY
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5D60, 26 bytes, `ret`. Unreferenced out-of-line copy of an inline getter
//   (catalogued FUN_00be5d60; its twin 0x00BE5D40 does X). Name descriptive, from the members it adds.
// Chain: Parent (+0x04, the CLandscapeBackgroundTreeNode) -> Map (+0xA4, CEngineLandscapeMap) -> Map (+0x14, CEngineMap)
//   -> WorldPosY (+0x62, unsigned short), plus MapY (+0x10E, unsigned short); both zero-extended, the sum in eax.
// Layout: retail differs from Ego_r.pdb for the two inner classes: the tree node's Map is PDB +0xAC (retail +0xA4,
//   the node is 8 bytes shorter ahead of it) and CEngineMap's WorldPosX/Y are PDB +0x64/+0x66 (retail +0x60/+0x62,
//   the STLport list iterator ahead of them is 4 bytes, not 8). Both are modelled locally as retail views; the patch
//   itself uses the engine header (MapX +0x10C, MapY +0x10E agree).
#include "engine/CLandscapeBackgroundPatch.h"

class CEngineMap { public:
    unsigned char  _pad00[0x5C];
    unsigned short MapWidth;                      // +0x5C
    unsigned short MapHeight;                     // +0x5E
    unsigned short WorldPosX;                     // +0x60
    unsigned short WorldPosY;                     // +0x62
};
class CEngineLandscapeMap { public:
    void*          __vftable;                     // +0x00
    void*          PatchTree;                     // +0x04
    void*          Patches;                       // +0x08
    long           PatchGridWidth;                // +0x0C
    long           PatchGridHeight;               // +0x10
    CEngineMap*    Map;                           // +0x14
};
class CLandscapeBackgroundTreeNode { public:
    unsigned char        _pad00[0xA4];
    CEngineLandscapeMap* Map;                     // +0xA4 (retail)
};

struct CLandscapeBackgroundPatchMethods : CLandscapeBackgroundPatch {
    long GetWorldMapY();
};

long CLandscapeBackgroundPatchMethods::GetWorldMapY()
{
    return Parent->Map->Map->WorldPosY + MapY;
}