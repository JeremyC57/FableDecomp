#include "rebuild_abi.h"
#include <cstdio>
#include <cmath>
const float kTexScale_c1b9e0 = 0.01f;
class CGraphicFrameData { public: char pad00[8]; unsigned short m_tlx; float GetFloatTLX(); };
float CGraphicFrameData::GetFloatTLX(){ return (float)m_tlx * kTexScale_c1b9e0; }
int main(){
    CGraphicFrameData g; g.m_tlx=100;
    if(std::fabs(g.GetFloatTLX()-1.0f)>1.0e-5f){std::printf("BAD\n");return 1;}
    std::printf("00c1b9e0_TEST PASS\n"); return 0;
}