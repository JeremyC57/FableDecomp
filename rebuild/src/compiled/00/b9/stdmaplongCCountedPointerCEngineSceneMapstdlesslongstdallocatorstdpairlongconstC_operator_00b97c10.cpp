// std::map::operator[](const long&) -- finds or inserts an empty counted pointer at a scene-map coordinate.
// Leaf: ??A?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@QAEAAV?$CCountedPointer@VCEngineSceneMap@@@@ABJ@Z
// Flags: default
// STLport 4.6 _map.h. Visible native insert bodies keep the temporary pair
// and counted-pointer release in the same form as the retail compiler.
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_ITERATOR_TRAITS
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_STLP_RB_GLOBAL_INCREMENT_SIZE_BODY_PASS27
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
class CEngineSceneMap;
template CCountedPointer<CEngineSceneMap>& std::map<long, CCountedPointer<CEngineSceneMap> >::operator[](const long&);