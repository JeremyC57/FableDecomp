#ifndef FABLE_ENGINE_CMAPINFO_H
#define FABLE_ENGINE_CMAPINFO_H
#include "engine/C2DBoxI.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
class CMap;
#ifdef FABLE_RETAIL_CMAPINFO_LOADBINARY
class CDataInputStream;
#endif
// Layout: original Ego_r CMapInfo TPI0x27b7a size72; native004fc65d
// indexes72-byte entries and reads PMap at+16. All original fields retained.
class CMapInfo {
public:
 C2DBoxI Area;
 CCountedPointer<CMap> PMap;
 CCharString LevelName,LevelScriptName;
 unsigned long MapUID;
 bool Used,Placed,Activated,Loading,LoadedOnPlayerProximity;
 CCharString SerialiseScript;
#ifdef _MSC_VER
 unsigned __int64 LevStamp,TngStamp;
#else
 unsigned char LevStamp[8],TngStamp[8];
#endif
 bool IsSea;
#ifdef FABLE_RETAIL_CMAPINFO_LOADBINARY
 // Retail opt-in (chunk 1013b, row 0x00507650): Ego_r.pdb ??0CMapInfo@@QAE@XZ, ??0CMapInfo@@QAE@ABV0@@Z and
 // ?LoadBinary@CMapInfo@@QAEXAAVCDataInputStream@@@Z (retail 0x004FB4F0), out of line.
 CMapInfo();
 CMapInfo(const CMapInfo& other);
 void LoadBinary(CDataInputStream& stream);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CMapInfo)==72);
#endif
