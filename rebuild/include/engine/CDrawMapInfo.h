#ifndef FABLE_ENGINE_CDRAWMAPINFO_H
#define FABLE_ENGINE_CDRAWMAPINFO_H
#ifdef FABLE_RETAIL_CDRAWMAPINFO_PASS23
#include "fable_string.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
class CIDrawMap;
// Both original PDB complete types agree on these public fields and types.
// Ego_r has the native four-byte CCharString: offsets0,1,2,4,8,12,20,24.
class CDrawMapInfo {
public:
    bool Valid;
    bool Loadable;
    bool GameLoadable;
    const CIDrawMap* PMap;
    CCharString StaticMeshName;
    C2DCoordI Pos;
    unsigned long UID;
    C2DExtentsI Extents;
};
FABLE_STATIC_ASSERT(sizeof(CDrawMapInfo)==32);
#endif
#endif
