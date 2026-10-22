#include <cstdio>
static int g_base=0;
struct C { char pad[0x28]; int NumHits; int FrameOfLastHit; void Initialise(); };
static void __fastcall base_init(C* p){ (void)p; g_base=1; }
void C::Initialise(){ this->NumHits = 0; this->FrameOfLastHit = 0; base_init(this); }
int main(){ C o; o.NumHits=9; o.FrameOfLastHit=9; o.Initialise();
 if(o.NumHits||o.FrameOfLastHit||!g_base){ std::printf("0096a250_TEST FAIL\n"); return 1;} std::printf("0096a250_TEST PASS\n"); return 0;}