// StaticInitialization::_$E1 -- initialize the scaled float at013D27DC.
// Leaf: _$E1
// Naming (NF173, 2026-11-20): q4787: the callee 0x00A76320 is Ego_r.pdb 0x00BBC070 / FableWin.pdb 0x03294880 ?GetSmallestNodeSize@CNavQuadTree@@SIMXZ (static __fastcall, no arguments; engine/CNavQuadTree.h FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE), replacing the extern "C" stand-in provider.
// Identity: descriptive data/function identity, proven static initializer role; Own native static initializer calls00a76320 (loads float0.5 from0122f59c with no native args), multiplies actual0.9f literal01253af0 and stores exactglobal013D27DC. Original global/provider names and provider highlevel role unknown; old GetCombatMinDistance is unsupported catalog propagation. Cdecl zeroarg float-return projection matches actual call ABI; not adopted from donor-address labels.
// Layout: actual single float global; compiler emits initialization helper from ordinary dynamic global initializer. No applicable shared class header.
#ifndef _MSC_VER
#define __cdecl
#endif
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_SMALLESTNODESIZE
#include "engine/CNavQuadTree.h"
float g_UnprovenScaledFloat_013D27DC=CNavQuadTree::GetSmallestNodeSize()*0.9f;