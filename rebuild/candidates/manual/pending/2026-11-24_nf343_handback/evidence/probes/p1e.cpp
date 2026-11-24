// probe p1e (NF343 isolation probe): map<CCharString,STB>::operator[] on the shared headers in a size TU
// Leaf: ??A?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_STLP_LESS_CCHARSTRING_SIZE_BODY
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/fable_script_cutscene.h"

#pragma optimize("s", on)
template CScriptGameResourceObjectScriptedThingBase& std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::operator[](const CCharString&);
