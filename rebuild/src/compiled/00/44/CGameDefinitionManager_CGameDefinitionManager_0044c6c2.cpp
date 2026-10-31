// 0044c6c2 CGameDefinitionManager::CGameDefinitionManager
// Flags:
// Leaf: ??0CGameDefinitionManager@@QAE@XZ
#pragma optimize("s",on)
#pragma optimize("y",off)
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CResourceList CResourceListFields
#include "engine/CResourceList.h"
#undef CResourceList
// Layout: RTTI CResourceList : CFailedAllocationHandler. Native list size3c.
class CFailedAllocationHandler {public:virtual ~CFailedAllocationHandler();virtual bool HandleFailedAllocation(unsigned long)=0;};
class CResourceList:public CFailedAllocationHandler {
public:
 virtual ~CResourceList();
 virtual bool HandleFailedAllocation(unsigned long);
 void SetMaximumMemory(unsigned long);
private:
 CResource Head;
 unsigned long ResourceCount;
 long AllocatedMemory,MaximumMemory;
 unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;
};
class CCharString;
class CWideStringData;
class CWideString {
 CWideStringData* PStringData;
public:CWideString(const unsigned short*);~CWideString();
};
// Layout: original resource-list base3c, unread manager data throughcf, extentd0.
class CDefinitionManager:public CResourceList {
 unsigned char unreadManagerState[sizeof(CDefinitionManagerFields)-sizeof(CResourceList)];
public:
 CDefinitionManager(const CWideString&,void(__fastcall*)(const CCharString&));
 virtual ~CDefinitionManager();
 virtual void InitAndCompile();
};
class CGlobalDef;class CEngineDef;class CEnvironmentDef;class CHeroStatsDef;
// Layout: InitAndCompile stores GLOBAL/ENGINE/ENVIRONMENT/HERO_STATS definitions
// into d0/d4/d8/dc. Original GetDef template donor order proves their const types.
// Member spellings do not survive either PDB; the four explicit fallback names
// are not claimed as recovered original names. See NAMING_EVIDENCE.md.
class CGameDefinitionManager:public CDefinitionManager {
 CDefPointer<const CGlobalDef> CUnprovenField_0044c6c2_d0;
 CDefPointer<const CEngineDef> CUnprovenField_0044c6c2_d4;
 CDefPointer<const CEnvironmentDef> CUnprovenField_0044c6c2_d8;
 CDefPointer<const CHeroStatsDef> CUnprovenField_0044c6c2_dc;
protected:static void __fastcall ErrorCallback(const CCharString&);
public:
 CGameDefinitionManager();
 virtual ~CGameDefinitionManager();
 virtual void InitAndCompile();
};
CGameDefinitionManager::CGameDefinitionManager():CDefinitionManager(L"game",ErrorCallback){SetMaximumMemory(0x80000);}
FABLE_STATIC_ASSERT(sizeof(CResourceList)==sizeof(CResourceListFields));
FABLE_STATIC_ASSERT(sizeof(CDefinitionManager)==0xd0);
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);