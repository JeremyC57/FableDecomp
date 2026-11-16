// Standalone original CFloatFilter/CReverbFns objects, target and controlled original virtual operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define private public
#define protected public
// 00c74510 CFloatFilter::ApplyFilter
// Leaf: ?ApplyFilter@CFloatFilter@@AAEXPBM0PAM01_NI@Z
// Flags: none
// Retail: Fable.exe00c74510,828bytes,ret1ch; SDK release PUBLIC/debug full CodeView original name/signature.
#define FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#define FABLE_RETAIL_CFLOATFILTER_APPLY_PASS23
#include "engine/CFloatFilter.h"
#include <string.h>
#pragma intrinsic(memcpy)
void CFloatFilter::ApplyFilter(const float* pfA, const float* pfB, float* pfZ, const float* pfSrc, float* pfDest,
    bool bInputValid, unsigned int uiSamples)
{
    float fA1 = pfA[1];
    float fA2 = pfA[2];
    float fB0 = pfB[0];
    float fB1 = pfB[1];
    float fB2 = pfB[2];
    if (fA2 == 0.0f && fB2 == 0.0f) {
        float fOffsetCorrectionA = 1.0f - fA1;
        float fOffsetCorrectionB = -(fB1 + fB0);
        if (bInputValid) {
            if (fA1 == 0.0f && fB1 == 0.0f) {
                if (fB0 == 1.0f) {
                    if (pfDest != pfSrc)
                        memcpy(pfDest, pfSrc, uiSamples * sizeof(float));
                } else {
                    m_pDSP->ScaleSamples(pfDest, pfSrc, fB0, uiSamples);
                }
                pfZ[0] = pfDest[uiSamples - 1];
            } else {
                float z0 = pfZ[0];
                for (unsigned int i = 0; i < uiSamples; i++) {
                    float fTemp = fB1 * z0;
                    z0 = z0 * fA1 + *pfSrc++ + fOffsetCorrectionA;
                    *pfDest++ = fB0 * z0 + fTemp + fOffsetCorrectionB;
                }
                pfZ[0] = z0;
            }
        } else {
            float z0 = pfZ[0];
            for (unsigned int i = 0; i < uiSamples; i++) {
                float fTemp = fB1 * z0;
                z0 = z0 * fA1 + fOffsetCorrectionA;
                *pfDest++ = fB0 * z0 + fTemp + fOffsetCorrectionB;
            }
            pfZ[0] = z0;
        }
    } else if (bInputValid) {
        m_pDSP->ApplyFilterOrder2(pfDest, pfSrc, pfA, pfB, pfZ, uiSamples);
    } else {
        float fOffsetCorrectionA=1.0f-(fA1+fA2);
        float fOffsetCorrectionB=-(fB0+fB1+fB2);
        float z0=pfZ[0];
        float z1=pfZ[1];
        for(unsigned int i=0;i<uiSamples;i++) {
            float z2=z1;
            z1=z0;
            *pfDest++=(z0=z1*fA1+z2*fA2+fOffsetCorrectionA)*fB0+z1*fB1+z2*fB2+fOffsetCorrectionB;
        }
        pfZ[0]=z0;
        pfZ[1]=z1;
    }
}

#undef private
#undef protected
CReverbDspProcessor::CReverbDspProcessor(){}CReverbDspProcessor::~CReverbDspProcessor(){}
CFloatFilter::CFloatFilter(){}CFloatFilter::~CFloatFilter(){}
CReverbFns*CReverbDspProcessor::m_pDSP=0;
int CReverbDspProcessor::m_iDSP_ResourceCount=0;
CReverbFns::eReverbType CReverbFns::Type(){abort();}
void CReverbFns::ScaleSamples(float*,const float*,float,float,unsigned int){abort();}
void CReverbFns::ScaleSamples(float*,float,float,unsigned int){abort();}
void CReverbFns::ScaleSamples(float*,const float*,const float,const unsigned int){abort();}
void CReverbFns::ScaleSamples(float*,const float,const unsigned int){abort();}
void CReverbFns::AddSamples(float*,const float*,const unsigned int){abort();}
void CReverbFns::ScaleAddSamples(float*,const float*,float,const float,const unsigned int){abort();}
void CReverbFns::ScaleAddSamples(float*,const float*,float,const unsigned int){abort();}
void CReverbFns::ApplyFilterOrder2(float*,const float*,const float*,const float*,float*,const unsigned int){abort();}
static float*ExpectedDest;static const float*ExpectedSource;static float*ExpectedState;static const float*ExpectedA;static const float*ExpectedB;static unsigned ExpectedCount;static int ScaleCalls,Order2Calls;
class FixtureOps:public CReverbFns {
public:
 eReverbType Type(){abort();}
 void ScaleSamples(float*,const float*,float,float,unsigned int){abort();}
 void ScaleSamples(float*,float,float,unsigned int){abort();}
 void ScaleSamples(float*dest,const float*src,const float gain,const unsigned int n){assert(this==CReverbDspProcessor::m_pDSP&&dest==ExpectedDest&&src==ExpectedSource&&n==ExpectedCount&&gain==ExpectedB[0]&&ScaleCalls==0&&Order2Calls==0);++ScaleCalls;for(unsigned i=0;i<n;++i)dest[i]=src[i]*gain;}
 void ScaleSamples(float*,const float,const unsigned int){abort();}
 void AddSamples(float*,const float*,const unsigned int){abort();}
 void ScaleAddSamples(float*,const float*,float,const float,const unsigned int){abort();}
 void ScaleAddSamples(float*,const float*,float,const unsigned int){abort();}
 void ApplyFilterOrder2(float*dest,const float*src,const float*a,const float*b,float*z,const unsigned int n){assert(this==CReverbDspProcessor::m_pDSP&&dest==ExpectedDest&&src==ExpectedSource&&a==ExpectedA&&b==ExpectedB&&z==ExpectedState&&n==ExpectedCount&&Order2Calls==0&&ScaleCalls==0);++Order2Calls;
  // Controlled external kernel result makes any extra inline filtering or state overwrite detectable.
  for(unsigned i=0;i<n;++i)dest[i]=2000.0f+i;z[0]=321.0f;z[1]=-123.0f;
 }
};
static bool Close(float a,float b){return a==b||(a!=a&&b!=b)||fabs((double)a-b)<=2e-5*(fabs((double)a)+fabs((double)b)+1);}
static void Reference(const float*a,const float*b,float*z,const float*src,float*out,bool input,unsigned n){
 bool order1=a[2]==0&&b[2]==0;
 if(!order1&&input){for(unsigned i=0;i<n;++i)out[i]=2000.0f+i;z[0]=321;z[1]=-123;return;}
 if(order1&&input&&a[1]==0&&b[1]==0){for(unsigned i=0;i<n;++i)out[i]=src[i]*b[0];z[0]=out[n-1];return;}
 // Scalar direct-form recurrence over independent state/history arrays. Each sample uses the prior history snapshot.
 double correctionA=1.0-(double)a[1]-(order1?0.0:(double)a[2]);
 double correctionB=-((double)b[0]+b[1]+(order1?0.0:(double)b[2]));
 for(unsigned i=0;i<n;++i){double old0=z[0],old1=z[1];double next=old0*a[1]+(order1?0.0:old1*a[2])+(input?src[i]:0.0)+correctionA;double y=next*b[0]+old0*b[1]+(order1?0.0:old1*b[2])+correctionB;out[i]=(float)y;z[0]=(float)next;if(!order1)z[1]=(float)old0;}
}
int main(){
 FixtureOps ops;CFloatFilter filter;CReverbDspProcessor::m_pDSP=&ops;
 const float a1s[]={0,.25f,-.5f,.75f},a2s[]={0,.125f,-.25f},b0s[]={0,.5f,1,2},b1s[]={0,.25f,-.5f},b2s[]={0,.125f};
 const unsigned counts[]={1,2,3,4,5,7,8,15,16,17,31,32,33,64};unsigned cases=0;
 for(int input=0;input<2;++input)for(int alias=0;alias<2;++alias)for(int x=0;x<4;++x)for(int y=0;y<3;++y)for(int u=0;u<4;++u)for(int v=0;v<3;++v)for(int w=0;w<2;++w)for(int nc=0;nc<14;++nc)for(int seed=0;seed<4;++seed)for(int history=0;history<3;++history){
  float a[3]={-999,a1s[x],a2s[y]},b[3]={b0s[u],b1s[v],b2s[w]};float src[72],dst[72],original[72],expected[64],state[4]={901,0,0,902};unsigned n=counts[nc];
  for(int i=0;i<72;++i){src[i]=(float)((i+seed)%9-4)*.125f;original[i]=src[i];dst[i]=777.0f;}
  float*member=filter.GetFilterHistory();member[0]=seed==0?1.0f:.25f*seed;member[1]=seed==0?1.0f:-.125f*seed;member[2]=seed==0?1.0f:-.25f*seed;member[3]=seed==0?1.0f:.125f*seed;float old_members[4];for(int i=0;i<4;++i)old_members[i]=member[i];
  state[1]=seed==0?1.0f:.5f*seed;state[2]=seed==0?1.0f:-.25f*seed;float*z=history==0?state+1:member+(history==1?0:2);float reference_z[2]={z[0],z[1]};float*dest=alias?src+4:dst+4;
  const float*source_ptr=!input&&(seed&1)?0:src+4;
  ExpectedDest=dest;ExpectedSource=source_ptr;ExpectedState=z;ExpectedA=a;ExpectedB=b;ExpectedCount=n;ScaleCalls=Order2Calls=0;
  Reference(a,b,reference_z,original+4,expected,input!=0,n);filter.ApplyFilter(a,b,z,source_ptr,dest,input!=0,n);
  bool order1=a[2]==0&&b[2]==0,pure=order1&&input&&a[1]==0&&b[1]==0;assert(ScaleCalls==(pure&&b[0]!=1?1:0)&&Order2Calls==(!order1&&input?1:0));
  for(unsigned i=0;i<n;++i)assert(Close(dest[i],expected[i]));assert(Close(z[0],reference_z[0])&&Close(z[1],reference_z[1]));
  for(int i=0;i<72;++i){if(i<4||i>=4+(int)n)assert(src[i]==original[i]&&dst[i]==777);else if(alias)assert(dst[i]==777);else assert(src[i]==original[i]);}
  assert(state[0]==901&&state[3]==902);for(int i=0;i<4;++i)if(history==0||i<(history==1?0:2)||i>=(history==1?2:4))assert(member[i]==old_members[i]);
  assert(a[0]==-999&&a[1]==a1s[x]&&a[2]==a2s[y]&&b[0]==b0s[u]&&b[1]==b1s[v]&&b[2]==b2s[w]);++cases;
 }
 assert(cases==193536);puts("00c74510_TEST PASS");return 0;
}