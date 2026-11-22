// std::_List_base<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry>::clear() -- STLport 4.6 _list.c: walk the ring from the sentinel's
// next, free each node (trivially destructible 16-byte entries), then relink the sentinel to itself.
// Leaf: ?clear@?$_List_base@VCSpriteTrailEntry@CEnginePrimitiveSpriteTrail@@V?$allocator@VCSpriteTrailEntry@CEnginePrimitiveSpriteTrail@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B09D50. Name: STLport 4.6 _List_base::clear (descriptive-by-library: the PDBs
// carry Dinkumware); the element type is Ego_r.pdb's CEnginePrimitiveSpriteTrail::CSpriteTrailEntry, the list member the landed
// binder 0x00B08530 (NParticleEngine::CPSCSingleSprite::Update) already clears through this spelling. The old stand-in
// ?Clear@List@@QAEXXZ (and the catalog's "SortTreeRecursively" label) were propagated by body shape.
// Body: the shared stlport/fable_containers.h out-of-class clear (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS), explicitly instantiated.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFCLASS
#include "stlport/fable_containers.h"
#include "engine/CEnginePrimitiveSpriteTrail.h"

template void std::_List_base<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry,
                              std::allocator<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry> >::clear();
