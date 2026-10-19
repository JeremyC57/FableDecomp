// CTCContainerRewardHero::AdjustTrajectoryVector(int, C3DVector*) -- normalize the supplied vector and apply its indexed vertical bias
// No C3DVector or hero class header is available; this source models only the observed 12-byte vector and method entry.
// Grade: RELOCATION_MATCH -- VC7.1 byte match after correcting parameter order and x87 expression grouping.
extern unsigned int g_InvSqrtMantissaTable[128]; // 0x013CE558
extern float g_three; // 0x0122DED4
extern float g_half; // 0x0122F59C
extern float g_vector_scale; // 0x01238170
extern float g_bias_base; // 0x0122DED8
extern float g_bias_scale; // 0x012305A0
struct C3DVector { float X,Y,Z; };
struct CTCContainerRewardHero { void AdjustTrajectoryVector(int, C3DVector*); };
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