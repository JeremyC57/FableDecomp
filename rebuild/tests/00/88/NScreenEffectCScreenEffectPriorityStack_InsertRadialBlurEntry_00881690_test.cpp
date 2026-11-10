#include <cstdio>
#include <list>
#include <set>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector(const C3DVector& o):X(o.X),Y(o.Y),Z(o.Z){}};
class CRGBFloatColour{public:float R,G,B,A;};
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION{ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_INVALID=0,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE=1,ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_SCREEN_SPACE=2};
class CEnvironmentSpellEffectInterface{public:struct CRadialBlurParams{CRGBFloatColour ThresholdColour,IntensityColour;float Length,Fade,Range,Twist;};struct CRadialBlurPositionParams{C3DVector AreaCenter,BlurCenter;ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos,BlurTrackPos;};};
class CMainGameComponent{public:static long GetConstantFPS(){return 40;}};
namespace NScreenEffect {enum ScreenEffectPriority{SCREEN_EFFECT_PRIORITY_OTHER=0,SCREEN_EFFECT_PRIORITY_SCRIPT=1,SCREEN_EFFECT_PRIORITY_SPELL=2,NUM_SCREEN_EFFECT_PRIORITIES=3};
class CScreenEffectPriorityStack{public:struct CStackEntry{CStackEntry(long frames):curdurationcount(0),durationcount(frames),fadingout(false){}virtual ~CStackEntry(){}long curdurationcount,durationcount;bool fadingout;};struct CRadialBlurEntry:public CStackEntry{CRadialBlurEntry(const CEnvironmentSpellEffectInterface::CRadialBlurParams& p,const CEnvironmentSpellEffectInterface::CRadialBlurPositionParams& pos,long frames):CStackEntry(frames),param(p),posparam(pos){}CRadialBlurEntry(const CRadialBlurEntry&);virtual ~CRadialBlurEntry(){}CEnvironmentSpellEffectInterface::CRadialBlurParams param;CEnvironmentSpellEffectInterface::CRadialBlurPositionParams posparam;};std::list<CRadialBlurEntry> RadialParamStack[3];std::set<CStackEntry*> RadialBlurKeys;void* InsertRadialBlurEntry(float,const CEnvironmentSpellEffectInterface::CRadialBlurParams&,const CEnvironmentSpellEffectInterface::CRadialBlurPositionParams&,ScreenEffectPriority);};}
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
NScreenEffect::CScreenEffectPriorityStack::CRadialBlurEntry::CRadialBlurEntry(const CRadialBlurEntry& other)
 :CStackEntry(other),param(other.param),posparam(other.posparam){}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void* NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry(float dur_secs,const CEnvironmentSpellEffectInterface::CRadialBlurParams& p,const CEnvironmentSpellEffectInterface::CRadialBlurPositionParams& pp,ScreenEffectPriority pri)
{
 long d=(long)(dur_secs*CMainGameComponent::GetConstantFPS());
 RadialParamStack[pri].push_back(CRadialBlurEntry(p,pp,d));
 CStackEntry* e=&RadialParamStack[pri].back();
 RadialBlurKeys.insert(e);
 void* k=e;
 return k;
}

static bool SameColour(const CRGBFloatColour& a,const CRGBFloatColour& b){return a.R==b.R&&a.G==b.G&&a.B==b.B&&a.A==b.A;}
static bool SameVector(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static bool SameParam(const CEnvironmentSpellEffectInterface::CRadialBlurParams& a,const CEnvironmentSpellEffectInterface::CRadialBlurParams& b){return SameColour(a.ThresholdColour,b.ThresholdColour)&&SameColour(a.IntensityColour,b.IntensityColour)&&a.Length==b.Length&&a.Fade==b.Fade&&a.Range==b.Range&&a.Twist==b.Twist;}
int main(){using namespace NScreenEffect;CScreenEffectPriorityStack stack;float times[5]={0,0.5f,1.75f,-0.5f,1.0f/16.0f};long expected[5]={0,20,70,-20,2};CEnvironmentSpellEffectInterface::CRadialBlurParams p;p.ThresholdColour.R=1;p.ThresholdColour.G=0.5f;p.ThresholdColour.B=0.25f;p.ThresholdColour.A=0.125f;p.IntensityColour.R=2;p.IntensityColour.G=4;p.IntensityColour.B=8;p.IntensityColour.A=16;p.Length=32;p.Fade=0.5f;p.Range=64;p.Twist=-2;CEnvironmentSpellEffectInterface::CRadialBlurParams saved=p;CEnvironmentSpellEffectInterface::CRadialBlurPositionParams pp;pp.AreaCenter=C3DVector(1,2,3);pp.BlurCenter=C3DVector(4,5,6);pp.AreaTrackPos=ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_WORLD_SPACE;pp.BlurTrackPos=ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION_SCREEN_SPACE;void* handles[15];int ok=1,n=0;
for(int pri=0;pri<3;++pri)for(int t=0;t<5;++t){void* handle=stack.InsertRadialBlurEntry(times[t],p,pp,(ScreenEffectPriority)pri);handles[n]=handle;CScreenEffectPriorityStack::CStackEntry* key=static_cast<CScreenEffectPriorityStack::CStackEntry*>(handle);CScreenEffectPriorityStack::CRadialBlurEntry* entry=static_cast<CScreenEffectPriorityStack::CRadialBlurEntry*>(key);ok &= key==&stack.RadialParamStack[pri].back()&&stack.RadialBlurKeys.count(key)==1;ok &= entry->curdurationcount==0&&entry->durationcount==expected[t]&&!entry->fadingout;ok &= SameParam(entry->param,p)&&SameVector(entry->posparam.AreaCenter,pp.AreaCenter)&&SameVector(entry->posparam.BlurCenter,pp.BlurCenter)&&entry->posparam.AreaTrackPos==pp.AreaTrackPos&&entry->posparam.BlurTrackPos==pp.BlurTrackPos;for(int j=0;j<n;++j)ok &= handles[j]!=handle;++n;ok &= stack.RadialBlurKeys.size()==(unsigned int)n;}
p.Length=128;pp.AreaCenter=C3DVector(9,9,9);for(int pri=0;pri<3;++pri){ok &= stack.RadialParamStack[pri].size()==5;for(std::list<CScreenEffectPriorityStack::CRadialBlurEntry>::const_iterator it=stack.RadialParamStack[pri].begin();it!=stack.RadialParamStack[pri].end();++it){ok &= SameParam(it->param,saved)&&it->posparam.AreaCenter.X==1&&it->posparam.AreaCenter.Y==2&&it->posparam.AreaCenter.Z==3;}}
ok &= n==15;std::printf(ok?"00881690_TEST PASS\n":"00881690_TEST FAIL\n");return ok?0:1;}