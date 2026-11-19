// probe
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_EFFECT_ENTRY_PASS23
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry::CMeshEffectPriorityEntry()
{
}
struct CTCGraphicAppearanceMeshEffects : NGraphicAppearance::CTCGraphicAppearance {
    typedef CMeshEffectPriorityEntry TEntry;
};
namespace std {
template CTCGraphicAppearanceMeshEffects::TEntry& map<void*, CTCGraphicAppearanceMeshEffects::TEntry>::operator[](void* const&);
}
