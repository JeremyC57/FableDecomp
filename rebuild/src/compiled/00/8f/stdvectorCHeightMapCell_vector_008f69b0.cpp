// std::vector<CHeightMapCell>::vector(size_type n) -- STLport's size constructor: start / finish / end-of-storage nulled, n 12-byte
// cells malloc'd (none for zero), then every cell copy-constructed from one default CHeightMapCell() temporary
// Leaf: ??0?$vector@VCHeightMapCell@@V?$allocator@VCHeightMapCell@@@std@@@std@@QAE@I@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F69B0, length 116 bytes, `ret 4`.
// Flags: /Gr
// Callees: malloc @ 0x00BFEA0E (`__cdecl`).
// Shape: CHeightMapCell's copy constructor is implicit -- one zero register (ebx) for the base initialisers, the T() words hoisted
//   into ebx/edx/ecx before the loop and the cursor copied to ebp (the user-copy-ctor form is 0x007C2AB0, vector<C3DVector>(n)); its
//   default constructor is empty (T() leaves the frame temporary unset): engine/CHeightMapCell.h FABLE_RETAIL_CHEIGHTMAPCELL_EMPTY_CTOR_PASS27.
// Names (agentNF250, 2026-11-22, CODEXP27 L83): was the placeholder CSTLportVector<C3DVector>::CSTLportVector (a local _Vector_base /
//   alloc-proxy model with a __forceinline fill helper; the catalog label "CSimpleMemoryPool::New<C3DAnimationSequence>" was
//   propagated). FableWin.pdb S_PUB32 ??0?$vector@VCHeightMapCell@@V?$allocator@VCHeightMapCell@@@std@@@std@@QAE@I@Z, called from
//   CHeightMap::Resize (FableWin 0x029E2912); retail's only caller 0x008F620F (landed 0x008F61A0 CHeightMap::Resize, which binds this
//   spelling) passes one count and the body allocates 12-byte cells. Now STLport's own body from stlport/fable_containers.h, emitted by
//   an explicit instantiation of that one member (the 0x007C2AB0 pattern).
// Flags note: the CHeightMap TU is /Gr (__fastcall default): the landed caller 0x008F61A0 builds with /Gr and its pick-any copy of this
//   constructor equals retail 0x008F69B0 (116 bytes); at the default convention VC7.1 keeps uninitialized_fill_n out of line (82 bytes).
//   Defines: the caller's vector / cell opt-ins.
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_EMPTY_CTOR_PASS27
#define FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"
#include "engine/CHeightMapCell.h"

template std::vector<CHeightMapCell, std::allocator<CHeightMapCell> >::vector(std::vector<CHeightMapCell, std::allocator<CHeightMapCell> >::size_type);
