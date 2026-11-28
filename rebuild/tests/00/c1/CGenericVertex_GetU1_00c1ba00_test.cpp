#include "rebuild_abi.h"
#include <cstdio>
#include <cmath>
const float kUvScale_c1ba00 = 0.01f;
struct CGenericVertex { char pad00[1]; unsigned char m_u1; float GetU1(); };
float CGenericVertex::GetU1(){ return (float)m_u1 * kUvScale_c1ba00; }
int main(){
    CGenericVertex v; v.m_u1=50;
    if(std::fabs(v.GetU1()-0.5f)>1.0e-5f){std::printf("BAD\n");return 1;}
    std::printf("00c1ba00_TEST PASS\n"); return 0;
}