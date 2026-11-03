#ifndef FABLE_ENGINE_FABLE_HANGING_TREE_H
#define FABLE_ENGINE_FABLE_HANGING_TREE_H
#define FABLE_RETAIL_HANGING_TREE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SCRIPT_QUEST
#define FABLE_STLPORT_HTTP
#define FABLE_STLPORT_CONSTRUCTORS
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST,SCRIPT_AI_PRIORITY_LOW,SCRIPT_AI_PRIORITY_MEDIUM,SCRIPT_AI_PRIORITY_HIGH,SCRIPT_AI_PRIORITY_HIGHEST };
#include "engine/C3DVector.h"
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
    if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}
    Data=0;Info=0;
}
#pragma auto_inline(on)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFunc.h"
#include "stlport/fable_map.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
#ifdef FABLE_RETAIL_FISTICUFFS
inline bool CScriptThing::IsAlive()const{if(!PImp.IsNull()&&PImp->IsAlive())return true;return false;}
#else
inline bool CScriptThing::IsAlive()const{if(PImp.IsNull())return false;return PImp->IsAlive();}
#endif
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
#endif
