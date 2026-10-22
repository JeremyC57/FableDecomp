#include "rebuild_abi.h"
#include <cstdio>
struct CCamera { char pad00[0x2c]; float m_seaLevel; float GetNearClip(); };
float CCamera::GetNearClip(){ return m_seaLevel; }
int main(){
    CCamera m; m.m_seaLevel=3.5f;
    if(m.GetNearClip()!=3.5f){std::printf("BAD\n");return 1;}
    std::printf("SEALVL_OK\n"); return 0;
}