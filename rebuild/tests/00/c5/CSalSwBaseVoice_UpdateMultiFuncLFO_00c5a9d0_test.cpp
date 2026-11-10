// Exact selected body; TimeToFrames is a controlled frame-rate service.
#include <math.h>
#include <stdio.h>
#include <assert.h>
struct _CMULTIFUNCLFO{float fDelay,fFrequency,fPitchModulation,fFilterCutOff;int iLevelModulation;};
static int time_calls;static float time_argument;
class CSalBaseVoice{protected:static int TimeToFrames(float value){++time_calls;time_argument=value;return (int)(value*64.0f);}};
class CSalSwBaseVoice:public CSalBaseVoice{public:
 unsigned int m_ffStatus;int m_nMultiLFOFrameCount,m_nMultiLFOFrames;
 float m_fMultiLFOPitchDeltaUp,m_fMultiLFOPitchDeltaDown,m_fMultiLFOCutoffDeltaUp,m_fMultiLFOCutoffDeltaDown,m_fMultiLFOGainDeltaUp,m_fMultiLFOGainDeltaDown,m_fMultiLFOPitch,m_fMultiLFOGain,m_fMultiLFOCutoff;
 void UpdateMultiFuncLFO(const _CMULTIFUNCLFO*);
};
void CSalSwBaseVoice::UpdateMultiFuncLFO(const _CMULTIFUNCLFO* pLFO){
 m_ffStatus&=~0x18000;
 if(pLFO){
  if(pLFO->fFrequency>0) m_nMultiLFOFrameCount=TimeToFrames(0.5f/pLFO->fFrequency);
  else m_nMultiLFOFrameCount=0;
  if(m_nMultiLFOFrameCount){
   m_fMultiLFOPitchDeltaUp=powf(2.0f,2.0f*pLFO->fPitchModulation/m_nMultiLFOFrameCount);
   m_fMultiLFOPitchDeltaDown=powf(2.0f,-2.0f*pLFO->fPitchModulation/m_nMultiLFOFrameCount);
   m_fMultiLFOCutoffDeltaUp=powf(2.0f,2.0f*pLFO->fFilterCutOff/m_nMultiLFOFrameCount);
   m_fMultiLFOCutoffDeltaDown=powf(2.0f,-2.0f*pLFO->fFilterCutOff/m_nMultiLFOFrameCount);
   float gain=pLFO->iLevelModulation/1000.0f;
   m_fMultiLFOGainDeltaUp=powf(10.0f,gain/m_nMultiLFOFrameCount);
   m_fMultiLFOGainDeltaDown=powf(10.0f,-gain/m_nMultiLFOFrameCount);
  }else{
   m_fMultiLFOPitchDeltaUp=1.0f; m_fMultiLFOPitchDeltaDown=1.0f;
   m_fMultiLFOGainDeltaUp=1.0f; m_fMultiLFOGainDeltaDown=1.0f;
   m_fMultiLFOCutoffDeltaUp=1.0f; m_fMultiLFOCutoffDeltaDown=1.0f;
  }
 }else m_ffStatus&=~0x18000;
 m_fMultiLFOPitch=1.0f; m_fMultiLFOGain=1.0f; m_fMultiLFOCutoff=1.0f;
 m_nMultiLFOFrames=0;
}

static bool close(float a,double b){return fabs((double)a-b)<0.00002*(fabs(b)+1.0);}
int main(){const float frequencies[]={-1,0,.125f,.25f,.5f,1,2,4};const float depths[]={-1,0,1};const int levels[]={-2000,-1000,0,1000,2000};int cases=0;
 for(int f=0;f<8;++f)for(int p=0;p<3;++p)for(int c=0;c<3;++c)for(int l=0;l<5;++l){
  _CMULTIFUNCLFO params;params.fDelay=.25f;params.fFrequency=frequencies[f];params.fPitchModulation=depths[p];params.fFilterCutOff=depths[c];params.iLevelModulation=levels[l];
  CSalSwBaseVoice voice;voice.m_ffStatus=0xfedfffffU;voice.m_nMultiLFOFrames=19;voice.m_nMultiLFOFrameCount=11;time_calls=0;
  voice.UpdateMultiFuncLFO(&params);int frames=frequencies[f]>0?(int)(32.0f/frequencies[f]):0;
  assert(voice.m_nMultiLFOFrameCount==frames&&voice.m_nMultiLFOFrames==0);assert(voice.m_ffStatus==(0xfedfffffU&~0x18000U));
  assert(time_calls==(frequencies[f]>0?1:0));if(time_calls)assert(time_argument==.5f/frequencies[f]);
  assert(voice.m_fMultiLFOPitch==1&&voice.m_fMultiLFOGain==1&&voice.m_fMultiLFOCutoff==1);
  if(frames){assert(close(voice.m_fMultiLFOPitchDeltaUp,pow(2.,2.*depths[p]/frames)));assert(close(voice.m_fMultiLFOPitchDeltaDown,pow(2.,-2.*depths[p]/frames)));
   assert(close(voice.m_fMultiLFOCutoffDeltaUp,pow(2.,2.*depths[c]/frames)));assert(close(voice.m_fMultiLFOCutoffDeltaDown,pow(2.,-2.*depths[c]/frames)));
   assert(close(voice.m_fMultiLFOGainDeltaUp,pow(10.,levels[l]/1000./frames)));assert(close(voice.m_fMultiLFOGainDeltaDown,pow(10.,-levels[l]/1000./frames)));
  }else assert(voice.m_fMultiLFOPitchDeltaUp==1&&voice.m_fMultiLFOPitchDeltaDown==1&&voice.m_fMultiLFOCutoffDeltaUp==1&&voice.m_fMultiLFOCutoffDeltaDown==1&&voice.m_fMultiLFOGainDeltaUp==1&&voice.m_fMultiLFOGainDeltaDown==1);
  ++cases;
 }
 CSalSwBaseVoice voice;voice.m_ffStatus=0xffffffffU;voice.m_nMultiLFOFrames=21;voice.m_nMultiLFOFrameCount=123;
 voice.m_fMultiLFOPitchDeltaUp=2;voice.m_fMultiLFOPitchDeltaDown=3;voice.m_fMultiLFOCutoffDeltaUp=4;voice.m_fMultiLFOCutoffDeltaDown=5;voice.m_fMultiLFOGainDeltaUp=6;voice.m_fMultiLFOGainDeltaDown=7;
 time_calls=0;voice.UpdateMultiFuncLFO(0);assert(!time_calls&&voice.m_nMultiLFOFrameCount==123&&voice.m_nMultiLFOFrames==0&&voice.m_ffStatus==0xfffe7fffU);
 assert(voice.m_fMultiLFOPitch==1&&voice.m_fMultiLFOGain==1&&voice.m_fMultiLFOCutoff==1);assert(voice.m_fMultiLFOPitchDeltaUp==2&&voice.m_fMultiLFOPitchDeltaDown==3&&voice.m_fMultiLFOCutoffDeltaUp==4&&voice.m_fMultiLFOCutoffDeltaDown==5&&voice.m_fMultiLFOGainDeltaUp==6&&voice.m_fMultiLFOGainDeltaDown==7);
 assert(cases==360);puts("00c5a9d0_TEST PASS");return 0;
}