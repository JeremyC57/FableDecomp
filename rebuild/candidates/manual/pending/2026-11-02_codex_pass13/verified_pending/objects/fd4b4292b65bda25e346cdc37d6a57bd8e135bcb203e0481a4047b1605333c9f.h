#ifndef FABLE_ENGINE_CMAPINFO_H
#define FABLE_ENGINE_CMAPINFO_H
#include "engine/C2DBoxI.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
class CMap;
// Complete original fields and public access, both PDBs; native stride72.
class CMapInfo {
public:
    C2DBoxI Area;
    CCountedPointer<CMap> PMap;
    CCharString LevelName;
    CCharString LevelScriptName;
    unsigned long MapUID;
    bool Used;
    bool Placed;
    bool Activated;
    bool Loading;
    bool LoadedOnPlayerProximity;
    CCharString SerialiseScript;
    unsigned __int64 LevStamp;
    unsigned __int64 TngStamp;
    bool IsSea;
};
FABLE_STATIC_ASSERT(sizeof(CMapInfo)==72);
#endif
