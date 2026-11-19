// probe
#include "fable_compiler.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/CCountedPointer.h"
#include "stlport/fable_map.h"
namespace NUISystem { enum ETableSprites { TABLE_SPRITES_TOP_LEFT = 0 }; class CComponent; }

namespace std {
template CCountedPointer<NUISystem::CComponent>& map<NUISystem::ETableSprites, CCountedPointer<NUISystem::CComponent> >::operator[](const NUISystem::ETableSprites&);
}
