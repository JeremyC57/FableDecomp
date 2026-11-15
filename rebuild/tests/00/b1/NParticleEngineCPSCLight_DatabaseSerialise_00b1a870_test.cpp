// Full production 00B1A870 body, verbatim; fixtures expose serialized effects and call order.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
static bool bad;static int liveStrings,crcCalls,resizeCalls;static unsigned int resizeRequested;
class C3DVector {public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class CRGBColour {public:unsigned char B,G,R,A;CRGBColour():B(0),G(0),R(0),A(0){}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a):B(b),G(g),R(r),A(a){}};
class CEnginePrimitiveHandle {public:unsigned int guard[2];};
static unsigned long Hash(const char*p){unsigned long h=2166136261UL;while(*p){h=((h^(unsigned char)*p++)*16777619UL)&0xffffffffUL;}return h;}
class CCharString {public:std::string text;CCharString(){++liveStrings;}CCharString(const CCharString&x):text(x.text){++liveStrings;}~CCharString(){--liveStrings;}unsigned long GetCRC()const{++crcCalls;return Hash(text.c_str());}};
template<class T>class CArray {public:std::vector<T> value;unsigned int size()const{return (unsigned int)value.size();}void resize(unsigned int n){++resizeCalls;resizeRequested=n;if(n>16){bad=true;return;}value.resize(n);}T&operator[](unsigned int n){if(n>=value.size()){bad=true;static T sink;return sink;}return value[n];}};
struct Value {int kind;unsigned long w[3];bool operator==(const Value&v)const{return kind==v.kind&&w[0]==v.w[0]&&w[1]==v.w[1]&&w[2]==v.w[2];}};
static Value Make(int k,unsigned long a,unsigned long b=0,unsigned long c=0){Value v;v.kind=k;v.w[0]=a&0xffffffffUL;v.w[1]=b&0xffffffffUL;v.w[2]=c&0xffffffffUL;return v;}
static unsigned long Bits(float x){unsigned int u;memcpy(&u,&x,4);return u;}
static Value Encode(bool v){return Make(0,v?1:0);}static Value Encode(unsigned long v){return Make(1,v);}static Value Encode(long v){return Make(2,(unsigned long)v);}static Value Encode(float v){return Make(3,Bits(v));}static Value Encode(const CRGBColour&v){return Make(4,(unsigned long)v.B|((unsigned long)v.G<<8)|((unsigned long)v.R<<16)|((unsigned long)v.A<<24));}static Value Encode(const C3DVector&v){return Make(5,Bits(v.X),Bits(v.Y),Bits(v.Z));}static Value Encode(const CCharString&v){return Make(6,Hash(v.text.c_str()),(unsigned long)v.text.size());}
static void Loaded(bool&v,int seed,int id,int){v=((seed*13+id*7)&2)!=0;}static void Loaded(unsigned long&v,int seed,int id,int){v=(0x81000000UL+(unsigned long)seed*997+(unsigned long)id*31)&0xffffffffUL;}static void Loaded(long&v,int seed,int id,int count){v=id==25?count:(seed*19-id*13-100);}static void Loaded(float&v,int seed,int id,int){v=(float)(seed*7-id*5)*0.25f;}static void Loaded(CRGBColour&v,int seed,int id,int){v=CRGBColour((unsigned char)(seed+id),(unsigned char)(seed*3-id),(unsigned char)(seed^id),(unsigned char)(255-id));}static void Loaded(C3DVector&v,int seed,int id,int){v=C3DVector((float)(seed+id)*0.25f,(float)(seed-id)*0.5f,(float)(id*3-seed));}static void Loaded(CCharString&v,int seed,int,int){char b[48];sprintf(b,"light-symbol-%d",seed);v.text=b;}
static bool Present(int profile,int id){return profile==0||(profile==1&&(id&1)==0)||(profile==3&&(id%3)==1);}
template<class T>static void Apply(T&v,const T&def,int mode,int profile,bool defaults,int seed,int id,int count){if(mode==4)v=def;else if(mode==2||(mode==0&&Present(profile,id)))Loaded(v,seed,id,count);else if(mode==0&&defaults)v=def;}
struct Event {std::string key;Value before,def,after;};static Event events[64];static int eventCount;
class CPersistContext {public:int mode,profile,seed,count;bool defaults;bool IsSaving()const{return mode==1||mode==3;}template<class T>void Transfer(const char*n,T&v){T def=T();Transfer(n,v,def);}template<class T>void Transfer(const char*n,T&v,const T&d){if(eventCount>=64){bad=true;return;}int id=eventCount++;Event&e=events[id];e.key=n;e.before=Encode(v);e.def=Encode(d);Apply(v,d,mode,profile,defaults,seed,id,count);e.after=Encode(v);}};
namespace NParticleEngine {class CEmitterDatabaseSerialiseInfo {public:unsigned long guards[2];};class CPSCLight {public:void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
    long LightLifeTimer;
    long LightRespawnDelayTimer;
    bool LightIsAlive;
    unsigned long LightPositionParam;
    float LightLifeSecs;
    float LightRespawnDelaySecs;
    float LightStartTime;
    float LightTimelineSecs;
    float LightStartRenderWorldRadius;
    float LightEndRenderWorldRadius;
    long LightAttenuationFactor;
    long LightFadeInEndInteger;
    long LightFadeOutBeginInteger;
    float LightWorldRadiusFadeMinimum;
    CRGBColour LightColourFadeMinimum;
    bool LightWorldRadiusFadeEnable:1;
    bool LightUseLifeSecs:1;
    bool LightEnabled:1;
    bool LightRespawns:1;
    bool LightColourFadeEnable:1;
    bool LightUseStartColour:1;
    bool LightUseMidColour:1;
    bool LightUseEndColour:1;
    bool LightUseFadeColour:1;
    bool LightUseTimelineSecs:1;
    bool StartedTimer:1;
    CRGBColour LightStartColour;
    CRGBColour LightMidColour;
    CRGBColour LightEndColour;
    CEnginePrimitiveHandle PrimHandle;
    CArray<C3DVector> LightUserPointsArray;
};}

namespace NParticleEngine {
void CPSCLight::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo&)
{
    {
        bool temp = LightEnabled;
        context.Transfer("LightEnabled", temp);
        LightEnabled = temp;
    }
    if (context.IsSaving()) {
        context.Transfer("LightPositionParam", LightPositionParam);
    } else {
        CCharString temp;
        context.Transfer("LightPositionParam", temp);
        LightPositionParam = temp.GetCRC();
    }
    {
        bool temp = LightUseLifeSecs;
        context.Transfer("LightUseLifeSecs", temp);
        LightUseLifeSecs = temp;
    }
    context.Transfer("LightLifeSecs", LightLifeSecs);
    {
        bool temp = LightRespawns;
        context.Transfer("LightRespawns", temp);
        LightRespawns = temp;
    }
    context.Transfer("LightRespawnDelaySecs", LightRespawnDelaySecs);
    context.Transfer("LightStartRenderWorldRadius", LightStartRenderWorldRadius);
    context.Transfer("LightEndRenderWorldRadius", LightEndRenderWorldRadius);
    context.Transfer("LightAttenuationFactor", LightAttenuationFactor);
    context.Transfer("LightFadeInEndInteger", LightFadeInEndInteger);
    context.Transfer("LightFadeOutBeginInteger", LightFadeOutBeginInteger);
    {
        bool temp = LightWorldRadiusFadeEnable;
        context.Transfer("LightWorldRadiusFadeEnable", temp);
        LightWorldRadiusFadeEnable = temp;
    }
    context.Transfer("LightWorldRadiusFadeMinimum", LightWorldRadiusFadeMinimum);
    {
        bool temp = LightUseFadeColour;
        context.Transfer("LightUseFadeColour", temp);
        LightUseFadeColour = temp;
    }
    {
        bool temp = LightColourFadeEnable;
        context.Transfer("LightColourFadeEnable", temp);
        LightColourFadeEnable = temp;
    }
    context.Transfer("LightColourFadeMinimum", LightColourFadeMinimum, CRGBColour(0, 0, 0, 0xff));
    {
        bool temp = LightUseStartColour;
        context.Transfer("LightUseStartColour", temp);
        LightUseStartColour = temp;
    }
    {
        bool temp = LightUseMidColour;
        context.Transfer("LightUseMidColour", temp);
        LightUseMidColour = temp;
    }
    {
        bool temp = LightUseEndColour;
        context.Transfer("LightUseEndColour", temp);
        LightUseEndColour = temp;
    }
    context.Transfer("LightStartColour", LightStartColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightMidColour", LightMidColour, CRGBColour(0, 0, 0, 0xff));
    context.Transfer("LightEndColour", LightEndColour, CRGBColour(0, 0, 0, 0xff));
    {
        bool temp = LightUseTimelineSecs;
        context.Transfer("LightUseTimelineSecs", temp);
        LightUseTimelineSecs = temp;
    }
    context.Transfer("LightStartTime", LightStartTime);
    context.Transfer("LightTimelineSecs", LightTimelineSecs);
    long size = (long)LightUserPointsArray.size();
    context.Transfer("LightUserPointsArraySize", size);
    LightUserPointsArray.resize(size);
    long i = 0;
    if (i < size) {
        C3DVector zero(0.0f, 0.0f, 0.0f);
        do {
            context.Transfer("LightUserPointsArray" + i, LightUserPointsArray[i], zero);
        } while (++i < size);
    }
}
}


static const char*keys[26]={"LightEnabled","LightPositionParam","LightUseLifeSecs","LightLifeSecs","LightRespawns","LightRespawnDelaySecs","LightStartRenderWorldRadius","LightEndRenderWorldRadius","LightAttenuationFactor","LightFadeInEndInteger","LightFadeOutBeginInteger","LightWorldRadiusFadeEnable","LightWorldRadiusFadeMinimum","LightUseFadeColour","LightColourFadeEnable","LightColourFadeMinimum","LightUseStartColour","LightUseMidColour","LightUseEndColour","LightStartColour","LightMidColour","LightEndColour","LightUseTimelineSecs","LightStartTime","LightTimelineSecs","LightUserPointsArraySize"};
static const char*pointKeys[7]={"LightUserPointsArray","ightUserPointsArray","ghtUserPointsArray","htUserPointsArray","tUserPointsArray","UserPointsArray","serPointsArray"};
static void SetFlag(NParticleEngine::CPSCLight&e,int id,bool x){switch(id){case 0:e.LightEnabled=x;break;case 2:e.LightUseLifeSecs=x;break;case 4:e.LightRespawns=x;break;case 11:e.LightWorldRadiusFadeEnable=x;break;case 13:e.LightUseFadeColour=x;break;case 14:e.LightColourFadeEnable=x;break;case 16:e.LightUseStartColour=x;break;case 17:e.LightUseMidColour=x;break;case 18:e.LightUseEndColour=x;break;case 22:e.LightUseTimelineSecs=x;break;default:bad=true;}}
static bool GetFlag(const NParticleEngine::CPSCLight&e,int id){switch(id){case 0:return e.LightEnabled;case 2:return e.LightUseLifeSecs;case 4:return e.LightRespawns;case 11:return e.LightWorldRadiusFadeEnable;case 13:return e.LightUseFadeColour;case 14:return e.LightColourFadeEnable;case 16:return e.LightUseStartColour;case 17:return e.LightUseMidColour;case 18:return e.LightUseEndColour;case 22:return e.LightUseTimelineSecs;default:bad=true;return false;}}
static void Init(NParticleEngine::CPSCLight&e,int seed,int n){e.LightLifeTimer=0x11223344;e.LightRespawnDelayTimer=-591;e.LightIsAlive=(seed&1)!=0;e.StartedTimer=(seed&2)!=0;e.LightPositionParam=0xf1234567UL;e.PrimHandle.guard[0]=0xdeadbeefU;e.PrimHandle.guard[1]=0xabcdef12U;const int flags[10]={0,2,4,11,13,14,16,17,18,22};for(int k=0;k<10;++k)SetFlag(e,flags[k],((seed*37+seed/3)>>k&1)!=0);e.LightLifeSecs=0.5f+seed;e.LightRespawnDelaySecs=-0.25f*seed;e.LightStartRenderWorldRadius=seed*2.0f;e.LightEndRenderWorldRadius=-seed*0.5f;e.LightAttenuationFactor=seed-99;e.LightFadeInEndInteger=seed+100;e.LightFadeOutBeginInteger=-seed-21;e.LightWorldRadiusFadeMinimum=seed*0.25f;e.LightColourFadeMinimum=CRGBColour(1,2,3,4);e.LightStartColour=CRGBColour(51,52,53,54);e.LightMidColour=CRGBColour(101,102,103,104);e.LightEndColour=CRGBColour(201,202,203,204);e.LightStartTime=seed*0.5f;e.LightTimelineSecs=-seed*0.25f;e.LightUserPointsArray.value.resize(n);for(int k=0;k<n;++k)e.LightUserPointsArray.value[k]=C3DVector((float)(seed+k),(float)(seed-k),(float)(-seed*k));}
static int Fail(int id,const char*what){printf("00b1a870_TEST FAIL %d %s\n",id,what);return 1;}
template<class T>static bool CheckTransfer(T&v,const T&d,int id,const CPersistContext&c){Value before=Encode(v);Apply(v,d,c.mode,c.profile,c.defaults,c.seed,id,c.count);return events[id].key==keys[id]&&events[id].before==before&&events[id].def==Encode(d)&&events[id].after==Encode(v);}
int main(){int cases=0;NParticleEngine::CEmitterDatabaseSerialiseInfo info;info.guards[0]=0x72345678;info.guards[1]=0x87654321;
 for(int mode=0;mode<5;++mode)for(int profile=0;profile<4;++profile)for(int defaults=0;defaults<2;++defaults)for(int seed=0;seed<32;++seed)for(int initial=0;initial<3;++initial)for(int desired=0;desired<7;++desired,++cases){
  bad=false;eventCount=resizeCalls=crcCalls=0;resizeRequested=999;NParticleEngine::CPSCLight actual;Init(actual,seed,initial);NParticleEngine::CPSCLight expect=actual;CPersistContext c;c.mode=mode;c.profile=profile;c.defaults=defaults!=0;c.seed=seed;c.count=desired;actual.DatabaseSerialise(c,info);if(bad||liveStrings||resizeCalls!=1||info.guards[0]!=0x72345678||info.guards[1]!=0x87654321)return Fail(cases,"boundary/lifetime");
  for(int id=0;id<26;++id){bool ok=true;switch(id){case 0:case 2:case 4:case 11:case 13:case 14:case 16:case 17:case 18:case 22:{bool b=GetFlag(expect,id);ok=CheckTransfer(b,false,id,c);SetFlag(expect,id,b);break;}
   case 1:if(c.IsSaving())ok=CheckTransfer(expect.LightPositionParam,0UL,id,c);else{CCharString text,empty;ok=CheckTransfer(text,empty,id,c);expect.LightPositionParam=Hash(text.text.c_str());}break;
   case 3:ok=CheckTransfer(expect.LightLifeSecs,0.0f,id,c);break;case 5:ok=CheckTransfer(expect.LightRespawnDelaySecs,0.0f,id,c);break;case 6:ok=CheckTransfer(expect.LightStartRenderWorldRadius,0.0f,id,c);break;case 7:ok=CheckTransfer(expect.LightEndRenderWorldRadius,0.0f,id,c);break;case 8:ok=CheckTransfer(expect.LightAttenuationFactor,0L,id,c);break;case 9:ok=CheckTransfer(expect.LightFadeInEndInteger,0L,id,c);break;case 10:ok=CheckTransfer(expect.LightFadeOutBeginInteger,0L,id,c);break;case 12:ok=CheckTransfer(expect.LightWorldRadiusFadeMinimum,0.0f,id,c);break;case 15:ok=CheckTransfer(expect.LightColourFadeMinimum,CRGBColour(0,0,0,255),id,c);break;case 19:ok=CheckTransfer(expect.LightStartColour,CRGBColour(0,0,0,255),id,c);break;case 20:ok=CheckTransfer(expect.LightMidColour,CRGBColour(0,0,0,255),id,c);break;case 21:ok=CheckTransfer(expect.LightEndColour,CRGBColour(0,0,0,255),id,c);break;case 23:ok=CheckTransfer(expect.LightStartTime,0.0f,id,c);break;case 24:ok=CheckTransfer(expect.LightTimelineSecs,0.0f,id,c);break;case 25:{long n=(long)expect.LightUserPointsArray.size();ok=CheckTransfer(n,0L,id,c);if(resizeRequested!=(unsigned int)n)ok=false;expect.LightUserPointsArray.value.resize((unsigned int)n);break;}default:ok=false;}if(!ok)return Fail(cases,"transfer order/value/default");}
  const int flagIds[10]={0,2,4,11,13,14,16,17,18,22};for(int k=0;k<10;++k)if(GetFlag(actual,flagIds[k])!=GetFlag(expect,flagIds[k]))return Fail(cases,"bitfield neighbours/writeback");
  if(crcCalls!=(c.IsSaving()?0:1)||actual.LightPositionParam!=expect.LightPositionParam)return Fail(cases,"position CRC path");
  const float af[8]={actual.LightLifeSecs,actual.LightRespawnDelaySecs,actual.LightStartRenderWorldRadius,actual.LightEndRenderWorldRadius,actual.LightWorldRadiusFadeMinimum,actual.LightStartTime,actual.LightTimelineSecs,0};const float ef[8]={expect.LightLifeSecs,expect.LightRespawnDelaySecs,expect.LightStartRenderWorldRadius,expect.LightEndRenderWorldRadius,expect.LightWorldRadiusFadeMinimum,expect.LightStartTime,expect.LightTimelineSecs,0};if(memcmp(af,ef,sizeof(af)))return Fail(cases,"float members");if(actual.LightAttenuationFactor!=expect.LightAttenuationFactor||actual.LightFadeInEndInteger!=expect.LightFadeInEndInteger||actual.LightFadeOutBeginInteger!=expect.LightFadeOutBeginInteger)return Fail(cases,"integer members");
  if(!(Encode(actual.LightColourFadeMinimum)==Encode(expect.LightColourFadeMinimum))||!(Encode(actual.LightStartColour)==Encode(expect.LightStartColour))||!(Encode(actual.LightMidColour)==Encode(expect.LightMidColour))||!(Encode(actual.LightEndColour)==Encode(expect.LightEndColour)))return Fail(cases,"colour members");
  unsigned int n=expect.LightUserPointsArray.size();if(actual.LightUserPointsArray.size()!=n||eventCount!=26+(int)n)return Fail(cases,"array size/call count");for(unsigned int i=0;i<n;++i){Event&e=events[26+i];C3DVector&v=expect.LightUserPointsArray.value[i];Value before=Encode(v);C3DVector zero;Apply(v,zero,mode,profile,defaults!=0,seed,26+i,desired);if(e.key!=pointKeys[i]||!(e.before==before)||!(e.def==Encode(zero))||!(e.after==Encode(v))||!(Encode(actual.LightUserPointsArray.value[i])==Encode(v)))return Fail(cases,"array suffix key/default/value");}
  if(actual.LightLifeTimer!=0x11223344||actual.LightRespawnDelayTimer!=-591||actual.LightIsAlive!=((seed&1)!=0)||actual.StartedTimer!=((seed&2)!=0)||actual.PrimHandle.guard[0]!=0xdeadbeefU||actual.PrimHandle.guard[1]!=0xabcdef12U)return Fail(cases,"unserialized members");
 }
 if(cases!=26880||liveStrings)return Fail(cases,"coverage/lifetime");
 puts("00b1a870_TEST PASS");return 0;}