// probe p1a (NF343 isolation probe): map<CCharString,STB>::operator[] on the shared headers in a size TU
// Leaf: ??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR

#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/fable_script_cutscene.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
template CScriptGameResourceObjectScriptedThingBase& std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[](const CCharString&);
