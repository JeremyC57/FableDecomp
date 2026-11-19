// Test for NPlayerGui::GFToScreen(const C2DVector&) @ 0x00643BE2 (identity copy into the hidden result). NF95 2026-11-19: names
// follow the PDB (was the stand-in CanMove).
#include <cstdio>
class C2DVector { public: float X; float Y; };
namespace NPlayerGui { static C2DVector GFToScreen(const C2DVector& pos) { return pos; } }
int main(){ C2DVector src; src.X=7.0f; src.Y=9.0f;
 C2DVector r=NPlayerGui::GFToScreen(src);
 if(r.X!=7.0f || r.Y!=9.0f){ std::printf("00643be2_TEST FAIL\n"); return 1;}
 std::printf("00643be2_TEST PASS\n"); return 0;}
