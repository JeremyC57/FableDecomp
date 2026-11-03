#ifndef FABLE_ENGINE_CSCRIPTEDMAPBRUSHINIT_H
#define FABLE_ENGINE_CSCRIPTEDMAPBRUSHINIT_H
#include "engine/C2DBoxI.h"
#include "engine/CCharString.h"
// Both donor PDBs and native82117f: original complete 28-byte initialiser.
class CScriptedMapBrushInit {public:C2DBoxI GameArea;CCharString Name;float CreationTimeSeconds;long CreationPattern;};
FABLE_STATIC_ASSERT(sizeof(CScriptedMapBrushInit)==28);
#endif
