#ifndef FABLE_ENGINE_CIDRAWWORLD_H
#define FABLE_ENGINE_CIDRAWWORLD_H
// CIDrawWorld (new, chunk 2030; the chunk 723b pending model with the retail slot order): the game-world interface the engine draws
// from. RTTI .?AVCIDrawWorld@@ (no base); Ego_r.pdb ??0CIDrawWorld@@QAE@XZ / ??1CIDrawWorld@@UAE@XZ / ??_GCIDrawWorld@@UAEPAXI@Z.
// CEngineWorldMap::GameWorld (+0x198) points at it. The game's CWorld implements it in its second vtable 0x012390DC (4 slots):
//   slot 0 ?DrawGetProceduralTexture@CWorld@@ 0x004AE890 (`xor eax,eax ; ret`),
//   slot 1 ?DrawGetEnvironment@CWorld@@UBEPBVCIDrawEnvironment@@XZ 0x0049E350 (`mov eax,[ecx+14h]` = CWorld::PEnvironment, Ego_r +0x18),
//   slot 2 DrawGetWorldMap 0x0049E1A0 (`mov eax,[ecx+10h] ; ... add eax,4` = CWorld::PWorldMap, Ego_r +0x14, its CIDrawWorldMap base),
//   slot 3 the destructor thunk 0x004AE8A0.
// Header debt: engine/CWorld.h declares slots 1 / 2 the other way round (DrawGetWorldMap first); the retail bodies above prove this order
// (UpdateConstants 0x00BEA13E calls slot 1 and reads the environment's PeekInterpolatedParams from the result). Do not include both.
#include "rebuild_abi.h"
class CIDrawEnvironment;
class CIDrawWorldMap;
class CProceduralTexture;
class CIDrawWorld {
public:
    virtual const CProceduralTexture* DrawGetProceduralTexture() const = 0;   // slot 0
    virtual const CIDrawEnvironment* DrawGetEnvironment() const = 0;          // slot 1
    virtual const CIDrawWorldMap* DrawGetWorldMap() const = 0;                // slot 2
    virtual ~CIDrawWorld();                                                   // slot 3 (Ego_r ??_GCIDrawWorld@@UAEPAXI@Z)
};
#endif // FABLE_ENGINE_CIDRAWWORLD_H
