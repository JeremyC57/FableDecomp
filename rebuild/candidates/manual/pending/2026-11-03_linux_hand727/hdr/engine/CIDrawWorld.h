#ifndef FABLE_ENGINE_CIDRAWWORLD_H
#define FABLE_ENGINE_CIDRAWWORLD_H
// CIDrawWorld (chunk 723b): the game-world interface the engine draws from (RTTI .?AVCIDrawWorld@@, no base; Ego_r.pdb ??1CIDrawWorld@@UAE@XZ).
// CEngineWorldMap::GameWorld +0x198 points at it. The game's CWorld implements it in its second vtable 0x012390DC: slot 1
// ?DrawGetEnvironment@CWorld@@UBEPBVCIDrawEnvironment@@XZ 0x0049E350, slot 2 ?DrawGetWorldMap@CWorld@@UBEPBVCIDrawWorldMap@@XZ 0x0049E1A0.
#include "rebuild_abi.h"
class CIDrawEnvironment;
class CIDrawWorldMap;
class CIDrawWorld {
public:
    CIDrawWorld();                                                       // ??0CIDrawWorld@@QAE@XZ
    virtual ~CIDrawWorld();                                              // slot 0 ??1CIDrawWorld@@UAE@XZ
    virtual const CIDrawEnvironment* DrawGetEnvironment() const = 0;     // slot 1
    virtual const CIDrawWorldMap* DrawGetWorldMap() const = 0;           // slot 2
};
#endif // FABLE_ENGINE_CIDRAWWORLD_H
