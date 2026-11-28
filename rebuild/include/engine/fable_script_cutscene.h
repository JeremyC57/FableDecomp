#ifndef FABLE_ENGINE_SCRIPT_CUTSCENE_H
#define FABLE_ENGINE_SCRIPT_CUTSCENE_H
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#ifdef FABLE_RETAIL_CUTSCENE_SCRIPT_HELPERS_PASS30
class CSymbolMap;
class CScriptThing;
bool FABLE_FASTCALL IsTrue(const CCharString& text);
bool FABLE_FASTCALL IsFalse(const CCharString& text);
bool FABLE_FASTCALL IsNull(const CCharString& text);
bool FABLE_FASTCALL IsCutsceneSkipped();

CScriptThing FABLE_FASTCALL CSFindThing(const CCharString& name, std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>& actors);
CScriptThing FABLE_FASTCALL GetBestThingFromMarkerList(const std::vector<CScriptThing>&, const CScriptThing&, const CScriptThing&, bool);
void FABLE_FASTCALL InitializeMusicTypes();
void FABLE_FASTCALL PlayCutsceneSound(std::map<int, CCharString>&, int);
bool FABLE_FASTCALL CutsceneCameraPreloader(std::vector<CCharString>&, bool, long*);
float FABLE_FASTCALL GFCharStringToFloat(const CCharString&);
long FABLE_FASTCALL GFCharStringToInt(const CCharString&);
long FABLE_FASTCALL GFStringToInt(const char*);
float FABLE_FASTCALL GFStringToFloat(const char*);
double FABLE_FASTCALL GFGetTime();
extern CSymbolMap* AtmosMusicType;
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }
inline int FABLE_FASTCALL CSExtractParameterList(CCharString& arg, std::vector<CCharString>& vec)
{
    CCharString curpas("");
    for (int c = 0; c < arg.GetLength(); ++c) {
        if (((const char*)arg)[c] == ',') {
            vec.push_back(curpas);
            curpas = "";
        } else {
            curpas = curpas + ((const char*)arg)[c];
        }
    }
    if (curpas.GetLength() > 0)
        vec.push_back(curpas);
    return vec.size();
}
void FABLE_FASTCALL CutsceneMacro_ReadLightScene(CCharString& line, CCharString& name, unsigned char* red, unsigned char* green, unsigned char* blue);

#endif
// Original PDB global fastcall signature; real std::map types, native00cbfb7d.
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
#endif
