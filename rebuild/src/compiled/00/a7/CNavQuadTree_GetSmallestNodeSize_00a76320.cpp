// CNavQuadTree::GetSmallestNodeSize() -- the side of the smallest quad-tree node, a 0.5f constant (static __fastcall, no arguments)
// Leaf: ?GetSmallestNodeSize@CNavQuadTree@@SIMXZ
// Naming (NF173, 2026-11-20): q4787: re-authored from the free stand-in GetCombatMinDistance(CAIStateGroup_EnemyCreatureBase*) onto the
//   shared engine/CNavQuadTree.h (FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE). Ego_r.pdb 0x00BBC070 / FableWin.pdb 0x03294880
//   ?GetSmallestNodeSize@CNavQuadTree@@SIMXZ; the catalog row already names this address CNavQuadTree::GetSmallestNodeSize, and every landed
//   caller calls it with no `this` (0x00A15140, 0x00A16050, 0x00A1C010 at 0x00A1C1E9 / 0x00A1C58A, 0x00A472F0, the static initializers
//   0x012242B0 / 0x01224430). The protected virtual CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance (MBEMXZ) has an identical body
//   (`fld [0x0122F59C] ; ret`) and is folded onto this address by the linker (identical COMDAT folding); the address keeps the name its
//   callers bind.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A76320: fld dword ptr [0x0122F59C] (0.5f) ; ret.
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#include "engine/CNavQuadTree.h"

float __fastcall CNavQuadTree::GetSmallestNodeSize()
{
    return 0.5f;
}
