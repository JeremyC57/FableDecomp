#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifdef _MSC_VER
typedef unsigned int PlacementSize;
#else
typedef __SIZE_TYPE__ PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
class CWideStringData;
class CWideString { public: CWideStringData* PStringData; CWideString(); };
class CProfileManager;
class CProfile { public: CProfileManager* Manager; double StartTime,TotalTime,LastTime; bool Running; double Percentage; CWideString Name; long TreeDepth; bool NewNode; long Visible,CallCounter,UpdatedCallCounter; CProfile(); };
CProfile::CProfile() : Manager(0) {}

static int calls=0; static void* lastThis;
CWideString::CWideString() { ++calls; lastThis=this; PStringData=0; }
int main() { void* storage=malloc(sizeof(CProfile)); memset(storage,0xA5,sizeof(CProfile)); CProfile* object=new(storage) CProfile(); if(calls!=1 || object->Manager || object->Name.PStringData || lastThis!=&object->Name) { puts("009f8c60_TEST FAIL"); free(storage); return 1; } object->~CProfile(); free(storage); puts("009f8c60_TEST PASS"); return 0; }