// CTCContainerRewardHero::AdjustTrajectoryVector(int, C3DVector*) -- normalize the supplied vector and apply its indexed vertical bias
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// No C3DVector or hero class header is available; this source models only the observed 12-byte vector and method entry.
// Grade: RELOCATION_MATCH -- VC7.1 byte match after correcting parameter order and x87 expression grouping.
// Name fix (agentNF6 2026-11-10): 0x013CE558 is ?GFastOneOverSqrtTable@@3PAKA (unsigned long[128]; Ego_r.pdb / FableWin.pdb public and typed records).
extern unsigned long GFastOneOverSqrtTable[128]; // 0x013CE558
extern float g_three; // 0x0122DED4
extern float g_vector_scale; // 0x01238170
extern float g_bias_scale; // 0x012305A0
#include "engine/C3DVector.h"
class CTCContainerRewardHero { public: void AdjustTrajectoryVector(int, C3DVector*); };
void CTCContainerRewardHero::AdjustTrajectoryVector(int bias, C3DVector* v) {
    float x=v->X, y=v->Y, z=v->Z;
    float lengthSquared=x*x+y*y+z*z;
    unsigned int bits=*(unsigned int*)&lengthSquared;
    unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23) | (GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);
    float r=*(float*)&estimate;
    r=(g_three-r*r*lengthSquared)*r;
    r=r*0.5f;
    v->X=(v->X*r)*g_vector_scale;
    v->Y=(v->Y*r)*g_vector_scale;
    float vertical=((float)bias+1.0f)*g_bias_scale;
    v->Z=vertical+(r*v->Z)*g_vector_scale;
}