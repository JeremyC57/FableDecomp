#include "rebuild_abi.h"
#include <cstdio>
struct CMorphingSprite;
static int g_based_5540f0=0;
extern "C" void FABLE_FASTCALL FableStreamBaseClear_5540f0(CMorphingSprite *sprite){ g_based_5540f0=1; }
struct CMorphingSprite { char pad00[0x198]; long m_length; void Initialise(); };
void CMorphingSprite::Initialise(){ FableStreamBaseClear_5540f0(this); m_length=0; }
int main(){
    CMorphingSprite s; s.m_length=5; s.Initialise();
    if(s.m_length!=0||!g_based_5540f0){std::printf("BAD\n");return 1;}
    std::printf("MEMCLEAR_OK\n"); return 0;
}