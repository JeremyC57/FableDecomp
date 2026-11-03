#ifndef FABLE_ENGINE_SCRIPT_CUTSCENE_H
#define FABLE_ENGINE_SCRIPT_CUTSCENE_H
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// Original PDB global fastcall signature; real std::map types, native00cbfb7d.
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
#endif
