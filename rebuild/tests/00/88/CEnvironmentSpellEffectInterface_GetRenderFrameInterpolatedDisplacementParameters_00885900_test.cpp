// CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedDisplacementParameters(float) -- blend current and previous monochrome colour.
// Retail 00885900, 226 bytes. No calls. PDB interface has no generated header; its native offsets agree with donor.
struct CRGBFloatColour {
 float R,G,B,A;
 CRGBFloatColour() {}
 CRGBFloatColour(float r,float g,float b,float a):R(r),G(g),B(b),A(a) {}
 CRGBFloatColour operator*(float s) const { return CRGBFloatColour(s*R,s*G,s*B,s*A); }
 CRGBFloatColour operator+(const CRGBFloatColour& x) const { return CRGBFloatColour(R+x.R,G+x.G,B+x.B,A+x.A); }
};
struct CIDrawEnvironmentDisplacementParameters { CRGBFloatColour MonochromeEffectColour; };
struct CEnvironmentSpellEffectInterface {
 unsigned char beforeDisplacement[0x54];
 CRGBFloatColour DisplacementParams;
 unsigned char beforePrevious[0x70];
 CRGBFloatColour PrevFrameDisplacementParams;
 CIDrawEnvironmentDisplacementParameters GetRenderFrameInterpolatedDisplacementParameters(float t);
};
CIDrawEnvironmentDisplacementParameters CEnvironmentSpellEffectInterface::GetRenderFrameInterpolatedDisplacementParameters(float t) {
 if(t<0.0f) t=0.0f; else if(t>1.0f) t=1.0f;
 CIDrawEnvironmentDisplacementParameters result;
 result.MonochromeEffectColour=PrevFrameDisplacementParams*(1.0f-t)+DisplacementParams*t;
 return result;
}

#include <cstdio>
#include <cstring>
#include <cstddef>
int main(){
 CEnvironmentSpellEffectInterface x;std::memset(&x,0x5a,sizeof x);
 x.DisplacementParams=CRGBFloatColour(10,-8,6,40);
 x.PrevFrameDisplacementParams=CRGBFloatColour(2,4,-10,-8);
 unsigned char before[sizeof x];std::memcpy(before,&x,sizeof x);
 bool ok=offsetof(CEnvironmentSpellEffectInterface,DisplacementParams)==0x54 && offsetof(CEnvironmentSpellEffectInterface,PrevFrameDisplacementParams)==0xd4 && sizeof(CIDrawEnvironmentDisplacementParameters)==16;
 volatile float zero=0;float nan=zero/zero,inf=1.0f/zero;
 float ts[9]={-inf,-2,-0.0f,0,0.25f,0.5f,1,2,inf};
 const float expected[9][4]={{2,4,-10,-8},{2,4,-10,-8},{2,4,-10,-8},{2,4,-10,-8},{4,1,-6,4},{6,-2,-2,16},{10,-8,6,40},{10,-8,6,40},{10,-8,6,40}};
 for(int i=0;i<9;++i){CRGBFloatColour c=x.GetRenderFrameInterpolatedDisplacementParameters(ts[i]).MonochromeEffectColour;ok=ok&&c.R==expected[i][0]&&c.G==expected[i][1]&&c.B==expected[i][2]&&c.A==expected[i][3]&&std::memcmp(before,&x,sizeof x)==0;}
 CRGBFloatColour c=x.GetRenderFrameInterpolatedDisplacementParameters(nan).MonochromeEffectColour;
 ok=ok&&c.R!=c.R&&c.G!=c.G&&c.B!=c.B&&c.A!=c.A&&std::memcmp(before,&x,sizeof x)==0;
 // Retail evaluates both endpoints even when a weight is zero.
 x.DisplacementParams.R=nan;c=x.GetRenderFrameInterpolatedDisplacementParameters(0).MonochromeEffectColour;ok=ok&&c.R!=c.R&&c.G==4;
 x.DisplacementParams.R=10;x.PrevFrameDisplacementParams.A=nan;c=x.GetRenderFrameInterpolatedDisplacementParameters(1).MonochromeEffectColour;ok=ok&&c.A!=c.A&&c.R==10;
 std::printf("00885900_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}