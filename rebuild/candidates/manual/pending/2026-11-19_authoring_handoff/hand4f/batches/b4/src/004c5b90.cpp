// std::map<void*, NGraphicAppearance::CTCGraphicAppearance::CMeshEffectPriorityEntry>::operator[](void* const& __k) -- the mesh-effect entry registered under the owner, inserting a default entry (null effect) when absent
// Leaf: ??A?$map@PAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@U?$less@PAX@std@@V?$allocator@U?$pair@QAXUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@@std@@@5@@std@@QAEAAUCMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@ABQAX@Z
// Retail: Fable.exe @ 0x004C5B90, 161 bytes, thiscall `ret 4`.
// Identity: only caller NGraphicAppearance::CTCGraphicAppearance::SetPMeshEffect 0x004C0240 on CTCGraphicAppearance::MeshEffects
//   (engine/NGraphicAppearance_CTCGraphicAppearance.h); FableWin.pdb 0x01D131C0 names the instantiation (same decorated symbol).
// Callees: _Rb_tree<...>::insert_unique(iterator, const value_type&) 0x004C5480 (landed); the effect's virtual Delete (slot 1).
// Shape: the shared stlport/fable_map.h map::operator[] with the opt-ins of the twin family 0x00554030 / 0x005698A0; the entry from the
//   shared header (EFFECT_ENTRY_PASS23), its default constructor's body defined here: FableWin.pdb 0x01D07550
//   ??0CMeshEffectPriorityEntry@CTCGraphicAppearance@NGraphicAppearance@@QAE@XZ only constructs PEffect (the CIVCountedPointer default
//   constructor), i.e. an empty body; retail expands it inline (no call), so it is visible in this TU.
// Access: the entry is a protected member type and VC7.1 checks access in explicit instantiations, so the instantiation names it through
//   a derived class's typedef (no code; the decorated symbols are unchanged).
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
