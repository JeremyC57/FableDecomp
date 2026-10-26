#include <cstdio>
#include <cmath>
#include <cstring>
// CTCContainerRewardHero::AdjustTrajectoryVector(int, C3DVector*) -- normalize the supplied vector and apply its indexed vertical bias
// No C3DVector or hero class header is available; this source models only the observed 12-byte vector and method entry.
// Grade: RELOCATION_MATCH -- VC7.1 byte match after correcting parameter order and x87 expression grouping.
extern unsigned int g_InvSqrtMantissaTable[128]; // 0x013CE558
extern float g_three; // 0x0122DED4
extern float g_half; // 0x0122F59C
extern float g_vector_scale; // 0x01238170
extern float g_bias_base; // 0x0122DED8
extern float g_bias_scale; // 0x012305A0
class C3DVector { public: float X,Y,Z; };
class CTCContainerRewardHero { public: void AdjustTrajectoryVector(int, C3DVector*); };
void CTCContainerRewardHero::AdjustTrajectoryVector(int bias, C3DVector* v) {
    float x=v->X, y=v->Y, z=v->Z;
    float lengthSquared=x*x+y*y+z*z;
    unsigned int bits=*(unsigned int*)&lengthSquared;
    unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23) | (g_InvSqrtMantissaTable[(bits>>17)&0x7f]<<15);
    float r=*(float*)&estimate;
    r=(g_three-r*r*lengthSquared)*r;
    r=r*g_half;
    v->X=(v->X*r)*g_vector_scale;
    v->Y=(v->Y*r)*g_vector_scale;
    float vertical=((float)bias+g_bias_base)*g_bias_scale;
    v->Z=vertical+(r*v->Z)*g_vector_scale;
}

unsigned int g_InvSqrtMantissaTable[128];
float g_three=3.0f, g_half=0.5f, g_vector_scale=2.0f, g_bias_base=1.5f, g_bias_scale=0.25f;
static int near(float a,float b) { return std::fabs(a-b)<0.003f; }
static void seed(float x,float y,float z) {
    float l=x*x+y*y+z*z; unsigned int bits; std::memcpy(&bits,&l,4);
    unsigned int exp=(bits>>23)&0xff, index=(bits>>17)&0x7f;
    float target=1.0f/std::sqrt(l); unsigned int tb; std::memcpy(&tb,&target,4);
    unsigned int base=(((0x17c-exp)>>1)<<23)&0xff800000u;
    g_InvSqrtMantissaTable[index]=(tb-base)>>15;
}
static int check(float x,float y,float z,int bias) {
    seed(x,y,z); C3DVector v={x,y,z}; CTCContainerRewardHero hero;
    hero.AdjustTrajectoryVector(bias,&v);
    float len=std::sqrt(x*x+y*y+z*z);
    float expectedZ=((float)bias+g_bias_base)*g_bias_scale+(z/len)*g_vector_scale;
    return near(v.X,(x/len)*g_vector_scale) && near(v.Y,(y/len)*g_vector_scale) && near(v.Z,expectedZ);
}
int main() {
    for(int i=0;i<128;++i) g_InvSqrtMantissaTable[i]=256;
    if(!check(1.0f,0.0f,0.0f,1) || !check(0.0f,1.0f,0.0f,-2) ||
       !check(1.0f,1.0f,1.0f,0) || !check(1.0f,2.0f,3.0f,4)) {
        std::printf("0074e080_TEST FAIL\n"); return 1;
    }
    std::printf("0074e080_TEST PASS\n"); return 0;
}