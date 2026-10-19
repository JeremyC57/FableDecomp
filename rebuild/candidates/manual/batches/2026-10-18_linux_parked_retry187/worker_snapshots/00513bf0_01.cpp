// CMapInfo::operator=(const CMapInfo&) -- memberwise world-map entry assignment.
// Retail00513BF0 length202; strings99EFB0; block destructionBFE9BC.
// No CMapInfo/C2DBoxI/CCPPointerInfo header exists, including _quarantine.
#include "fable_string.h"
void operator delete(void*) throw(); //00BFE9BC scalar CRT delete
struct C2DBoxI { long TLX,TLY,BRX,BRY; };
struct CCPPointerInfo { unsigned long RefCount; void(FABLE_FASTCALL* DeleteFunc)(void*); void* Data; };
template<class T> struct CCountedPointer {
 T* Data; CCPPointerInfo* Info;
 CCountedPointer& operator=(const CCountedPointer& o){
  T* data=o.Data; CCPPointerInfo* info=o.Info;
  if(Info!=info){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=data;Info=info;if(Info)++Info->RefCount;}
  return *this;
 }
};
struct CMap;
struct CMapInfo {
 C2DBoxI Area; CCountedPointer<CMap> PMap; CCharString LevelName,LevelScriptName;
 unsigned long MapUID; bool Used,Placed,Activated,Loading,LoadedOnPlayerProximity;
 CCharString SerialiseScript;
#ifdef _MSC_VER
 unsigned __int64 LevStamp,TngStamp;
#else
 unsigned long long LevStamp,TngStamp;
#endif
 bool IsSea;
 CMapInfo& operator=(const CMapInfo&);
};
CMapInfo& CMapInfo::operator=(const CMapInfo& o){
 Area=o.Area;PMap=o.PMap;LevelName=o.LevelName;LevelScriptName=o.LevelScriptName;
 MapUID=o.MapUID;Used=o.Used;Placed=o.Placed;Activated=o.Activated;Loading=o.Loading;
 LoadedOnPlayerProximity=o.LoadedOnPlayerProximity;SerialiseScript=o.SerialiseScript;
 LevStamp=o.LevStamp;TngStamp=o.TngStamp;IsSea=o.IsSea;return *this;
}
