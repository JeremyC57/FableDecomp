#ifndef FABLE_ENGINE_C3DMESHVOLUME_H
#define FABLE_ENGINE_C3DMESHVOLUME_H
// New header (chunk 2005, row 0x00A12EE0): Ego_r.pdb C3DMeshVolume (12 bytes: Name CCharString +0, ConvexVolume
// CCountedPointer<CContainmentVolume> +4) with FableWin.pdb's inline ?GetConvexVolume@C3DMeshVolume@@QBEPBVCContainmentVolume@@XZ
// (retail reads the counted pointer's Data at +4 of the value, 0x00A12F1A). Class key V (both PDBs' mangled names).
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CContainmentVolume;
class C3DMeshVolume {
public:
    const CContainmentVolume* GetConvexVolume() const { return ConvexVolume.Get(); }
private:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};
#endif
