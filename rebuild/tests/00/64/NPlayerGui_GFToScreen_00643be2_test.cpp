// Test for NPlayerGui::GFToScreen(const C2DVector&) @ 0x00643BE2 (identity copy into the hidden result). NF95 2026-11-19: names
// follow the PDB (was the stand-in CanMove). NF97 2026-11-19 (port): C2DVector and the declaration come from the shared headers
// (engine/NPlayerGui_Graphics.h with FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_CALLED), no local model; the test links the row.
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_NPLAYERGUI_GFTOSCREEN_CALLED
#include <cstdio>
#include "engine/NPlayerGui_Graphics.h"
int main(){ C2DVector src; src.X=7.0f; src.Y=9.0f;
 C2DVector r=NPlayerGui::GFToScreen(src);
 if(r.X!=7.0f || r.Y!=9.0f){ std::printf("00643be2_TEST FAIL\n"); return 1;}
 std::printf("00643be2_TEST PASS\n"); return 0;}
