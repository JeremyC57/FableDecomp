#include "rebuild_abi.h"
#include <cstdio>
struct CCategory { char data[12]; };
struct CParticleDataBank { char pad00[0x1e0]; CCategory *m_categories; CCategory *GetEmitterInfo(long index); };
CCategory *CParticleDataBank::GetEmitterInfo(long index){ return &m_categories[index]; }
int main(){
    CCategory arr[4]; CParticleDataBank inv; inv.m_categories=arr;
    if(inv.GetEmitterInfo(0)!=&arr[0]){std::printf("B0\n");return 1;}
    if(inv.GetEmitterInfo(3)!=&arr[3]){std::printf("B3\n");return 1;}
    std::printf("GETCAT_OK\n"); return 0;
}