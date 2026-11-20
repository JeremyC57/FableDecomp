// std::vector<C2DBoxF>::vector(size_type n) -- STLport 4.6 _vector.h `explicit vector(size_type __n)`: allocate n boxes and fill them with
//   copies of a default C2DBoxF
// Leaf: ??0?$vector@VC2DBoxF@@V?$allocator@VC2DBoxF@@@std@@@std@@QAE@I@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80360, 122 bytes.
// Names (NF201, 2026-11-20; Account B hand3c3 b8 NAME_FIXES): Ego_d.pdb CDrawWorld UpdateLines' local `node_areas` is std::vector<C2DBoxF>
//   and 0x00A781CB is this constructor's one call (the elements are then Initialise(tlx, tly, brx, bry)d and ContainsPoint-tested as
//   boxes); the landed caller 0x00A781A0 binds this spelling. Descriptive-by-library (STLport instance, no STL names in the PDB name
//   sets). Was ??0?$vector@VC2DLineF@@..@@QAE@I@Z (same 16-byte element, same body) over a local model with a home-made
//   __forceinline uninitialized_fill_n; now the shared stlport/fable_containers.h default body + FABLE_RETAIL_C2DBOXF_COPY (FableWin.pdb
//   ??0C2DBoxF@@QAE@ABV0@@Z, the authored member-wise copy), explicit instantiation.
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_STLP_VECTOR
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"

template std::vector<C2DBoxF, std::allocator<C2DBoxF> >::vector(std::vector<C2DBoxF, std::allocator<C2DBoxF> >::size_type);
