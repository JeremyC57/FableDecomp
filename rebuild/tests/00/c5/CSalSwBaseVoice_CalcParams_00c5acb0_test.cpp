// 00c5acb0 full production-body behavior fixture. Engine ABI/storage are fixture projections.
// Every external audio calculation/dispatch is a recording double; no original SDK implementation is copied.
#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef _MSC_VER
typedef __int64 CREFTIME;
#else
typedef long long CREFTIME;
#endif
struct _CLEVELENVELOPE {float fDelay,fAttack,fHold,fDecay,fSustain,fRelease;};
struct _CMULTIFUNCENVELOPE {float fDelay,fAttack,fHold,fDecay,fSustain,fRelease,fPitchScale,fFilterCutOff;};
struct _CPITCHLFO {float fDelay,fFrequency,fPitchModulation;};
struct _CMULTIFUNCLFO {float fDelay,fFrequency,fPitchModulation,fFilterCutOff;int iLevelModulation;};
struct CODASOURCE {unsigned int ffUpdate;float fGain;int iLevel;float fPitch;void*pLPFilter;void*pEQFilter;_CLEVELENVELOPE*pLevelEnv;_CMULTIFUNCENVELOPE*pMultiEnv;_CPITCHLFO*pPitchLFO;_CMULTIFUNCLFO*pMultiLFO;};
struct Events {int n,a[32];bool bad;void add(int x){if(n==32){bad=true;return;}a[n++]=x;}Events():n(0),bad(false){}};
static Events events;static bool levelResult,multiResult;static unsigned int initUpdates;static const CODASOURCE*expectedParams;static const void*expectedSource;static unsigned int expectedDispatch,expectedInputUpdates;static bool expectedLoop;static bool pitchExpected,cutoffExpected;static int pitchCalls;
class CCoda3DSource;class CCoda2DSource;
class CCodaSource {public:CODASOURCE m_Params;unsigned int m_ffStatus;bool spatial;CCodaSource(){memset(&m_Params,0,sizeof(m_Params));m_ffStatus=0;spatial=false;}virtual ~CCodaSource(){}virtual bool Is3D()const{events.add(13);return spatial;}const CODASOURCE&GetParams()const{return m_Params;}bool IsLooping()const{return(m_ffStatus&0x100)!=0;}CCoda3DSource*Get3D()const;CCoda2DSource*Get2D()const;};
class CCoda3DSource:public CCodaSource{};class CCoda2DSource:public CCodaSource{};
inline CCoda3DSource*CCodaSource::Get3D()const{return Is3D()?static_cast<CCoda3DSource*>(const_cast<CCodaSource*>(this)):0;}
inline CCoda2DSource*CCodaSource::Get2D()const{return Is3D()?0:(CCoda2DSource*)this;}
class CSalSwBuffer {public:void*m_pvBuffer;bool IsCreated()const{return m_pvBuffer!=0;}};
class CSalBaseVoice {public:unsigned int m_ffStatus,m_ffParams;CREFTIME m_rtRelease;float m_fLevelEnvDelta;bool IsStarting()const{return(m_ffStatus&1)!=0;}bool IsReleasingEnvelopes()const{return(m_ffParams&0x200)!=0;}bool IsLevelEnvReleased()const{return(m_ffStatus&0x40)!=0;}static float CalcExpDelta(float a,float b){events.add(3);if(a!=1.0f||b!=4.25f)events.bad=true;return.75f;}static float CalcPitchDelta(float a,float b,float c){events.add(4);float expect=(!pitchCalls++&&pitchExpected)?2.0f:3.0f;if(a!=expect||b!=0.0f||c!=6.5f)events.bad=true;return.5f;}void UpdatePitchLFO(const _CPITCHLFO*p){events.add(7);if(p!=expectedParams->pPitchLFO)events.bad=true;}void CalcPitchLFO(const _CPITCHLFO*p){events.add(8);if(p!=expectedParams->pPitchLFO)events.bad=true;}};
class CSalSwBaseVoice:public CSalBaseVoice {public:CSalSwBuffer m_PlayBuffer;float m_fMultiEnvPitch,m_fMultiEnvPitchDelta,m_fMultiEnvCutoff,m_fMultiEnvCutoffDelta;int m_nMultiEnvFrames;bool spatial;virtual~CSalSwBaseVoice(){}virtual bool Is3D()const{events.add(12);return spatial;}virtual unsigned int InitParams(const CCodaSource*p,const CODASOURCE&q){events.add(1);if(p!=expectedSource||&q!=expectedParams)events.bad=true;return initUpdates;}void UpdatePlayBuffer(bool b){events.add(2);if(b!=expectedLoop)events.bad=true;}bool CalcLevelEnvelope(const _CLEVELENVELOPE*p,bool b){events.add(5);if(p!=expectedParams->pLevelEnv||b!=((expectedInputUpdates&8)!=0))events.bad=true;return levelResult;}bool IsMultiEnvReleased()const{return(m_ffStatus&0x4000)!=0;}bool CalcMultiFuncEnvelope(const _CMULTIFUNCENVELOPE*p,bool b){events.add(6);if(p!=expectedParams->pMultiEnv||b!=((expectedInputUpdates&0x10)!=0))events.bad=true;return multiResult;}void UpdateMultiFuncLFO(const _CMULTIFUNCLFO*p){events.add(9);if(p!=expectedParams->pMultiLFO)events.bad=true;}void CalcMultiFuncLFO(const _CMULTIFUNCLFO*p){events.add(10);if(p!=expectedParams->pMultiLFO)events.bad=true;}void CalcParams(const CCodaSource*,CREFTIME);};
class CSalSwVoice:public CSalSwBaseVoice{};
class CSalSw3DVoice:public CSalSwVoice{public:void CalcParams(const CCoda3DSource*p,unsigned int f){events.add(14);if(p!=(static_cast<const CCodaSource*>(expectedSource)->spatial?expectedSource:0)||f!=expectedDispatch)events.bad=true;}};
class CSalSw2DVoice:public CSalSwVoice{public:void CalcParams(const CCoda2DSource*p,unsigned int f){events.add(15);if(p!=(!static_cast<const CCodaSource*>(expectedSource)->spatial?expectedSource:0)||f!=expectedDispatch)events.bad=true;}};
class CSalPlatform{public:unsigned int updates;unsigned int GetUpdate()const{return updates;}};static CSalPlatform platform;static CSalPlatform*g_pSal=&platform;

void CSalSwBaseVoice::CalcParams(const CCodaSource* pSource, CREFTIME rtTime)
{
    const CODASOURCE& params = pSource->GetParams();
    unsigned int ffUpdate;
    if (IsStarting())
        ffUpdate = InitParams(pSource,params);
    else
        ffUpdate = params.ffUpdate;
    if (m_PlayBuffer.IsCreated() && (ffUpdate & 1))
        UpdatePlayBuffer(pSource->IsLooping());
    if (IsReleasingEnvelopes() && m_rtRelease <= rtTime) {
        m_ffParams &= ~0x200;
        m_ffStatus &= ~0x7e7e;
        if ((params.pLevelEnv || params.pMultiEnv) && !(m_ffStatus & 0x20)) {
            m_ffStatus |= 0x20;
            if (params.pLevelEnv)
                m_fLevelEnvDelta = CalcExpDelta(1.0f,params.pLevelEnv->fRelease);
            if (params.pMultiEnv) {
                if (params.pMultiEnv->fPitchScale != 0.0f) {
                    m_fMultiEnvPitchDelta = CalcPitchDelta(params.pMultiEnv->fPitchScale,0.0f,params.pMultiEnv->fRelease);
                    m_nMultiEnvFrames = (int)(log10(m_fMultiEnvPitch) * (-1.0f / log10(m_fMultiEnvPitchDelta)));
                }
                if (params.pMultiEnv->fFilterCutOff != 0.0f) {
                    m_fMultiEnvCutoffDelta = CalcPitchDelta(params.pMultiEnv->fFilterCutOff,0.0f,params.pMultiEnv->fRelease);
                    if (params.pMultiEnv->fPitchScale == 0.0f)
                        m_nMultiEnvFrames = (int)(log10(m_fMultiEnvCutoff) * (-1.0f / log10(m_fMultiEnvCutoffDelta)));
                }
            }
        }
    }
    if (!IsLevelEnvReleased() && CalcLevelEnvelope(params.pLevelEnv,(ffUpdate & 8) != 0))
        ffUpdate |= 8;
    if (!IsMultiEnvReleased() && CalcMultiFuncEnvelope(params.pMultiEnv,(ffUpdate & 0x10) != 0))
        ffUpdate |= 0x10;
    if ((ffUpdate & 0x20) && !IsStarting())
        UpdatePitchLFO(params.pPitchLFO);
    if (params.pPitchLFO) {
        CalcPitchLFO(params.pPitchLFO);
        ffUpdate |= 0x20;
    }
    if ((ffUpdate & 0x40) && !IsStarting())
        UpdateMultiFuncLFO(params.pMultiLFO);
    if (params.pMultiLFO) {
        CalcMultiFuncLFO(params.pMultiLFO);
        ffUpdate |= 0x40;
    }
    if (ffUpdate || g_pSal->GetUpdate()) {
        if (Is3D()) {
            const CCoda3DSource* p3D = pSource->Get3D();
            ((CSalSw3DVoice*)this)->CalcParams(p3D,ffUpdate);
        } else {
            const CCoda2DSource* p2D = pSource->Get2D();
            ((CSalSw2DVoice*)this)->CalcParams(p2D,ffUpdate);
        }
    }
    m_ffStatus &= ~1;
}


static unsigned int randomState=0x13456879;
static unsigned int Next(){randomState=randomState*1664525u+1013904223u;return randomState^(randomState>>17);}
static int fail(int id,const char*message){printf("00c5acb0_TEST FAIL %d %s\n",id,message);return 1;}
int main(){
    _CLEVELENVELOPE le;memset(&le,0,sizeof(le));le.fRelease=4.25f;
    _CMULTIFUNCENVELOPE me;memset(&me,0,sizeof(me));me.fRelease=6.5f;
    _CPITCHLFO pl;_CMULTIFUNCLFO ml;memset(&pl,0,sizeof(pl));memset(&ml,0,sizeof(ml));
    CREFTIME times[8];times[0]=0;times[1]=1;times[2]=-1;times[3]=((CREFTIME)1<<32)-1;times[4]=((CREFTIME)1<<32);times[5]=((CREFTIME)1<<32)+1;times[6]=-((CREFTIME)1<<32);times[7]=-((CREFTIME)1<<32)+1;
    int coverage[16];memset(coverage,0,sizeof(coverage));
    for(int id=0;id<8192;++id){
        unsigned int a=Next(),b=Next();bool v3=(a&1)!=0,s3=(a&2)!=0;
        CSalSw3DVoice voice3;CSalSw2DVoice voice2;CSalSwBaseVoice*v=v3?static_cast<CSalSwBaseVoice*>(&voice3):static_cast<CSalSwBaseVoice*>(&voice2);
        CCoda3DSource source3;CCoda2DSource source2;CCodaSource*s=s3?static_cast<CCodaSource*>(&source3):static_cast<CCodaSource*>(&source2);
        s->spatial=s3;v->spatial=v3;s->m_ffStatus=(a&4)?0x100:0;expectedLoop=(a&4)!=0;
        unsigned int originalFlags=(a>>8)&0xff;originalFlags|=(a&8)?0x4000:0;originalFlags|=0x80000000u;
        unsigned int originalParams=(a&16)?0x200:0;originalParams|=0x10000000;
        unsigned int chosenUpdates=(b>>16)&0x79;initUpdates=(a>>16)&0x79;s->m_Params.ffUpdate=chosenUpdates;
        v->m_ffStatus=originalFlags;v->m_ffParams=originalParams;v->m_rtRelease=times[(a>>4)&7];CREFTIME now=times[(b>>4)&7];
        v->m_PlayBuffer.m_pvBuffer=(a&32)?v:0;
        v->m_fLevelEnvDelta=.875f;v->m_fMultiEnvPitch=10.0f;v->m_fMultiEnvCutoff=100.0f;v->m_fMultiEnvPitchDelta=.875f;v->m_fMultiEnvCutoffDelta=.875f;v->m_nMultiEnvFrames=123;
        s->m_Params.pLevelEnv=(a&64)?&le:0;s->m_Params.pMultiEnv=(a&128)?&me:0;s->m_Params.pPitchLFO=(b&64)?&pl:0;s->m_Params.pMultiLFO=(b&128)?&ml:0;
        me.fPitchScale=(b&1)?2.0f:0.0f;me.fFilterCutOff=(b&2)?3.0f:0.0f;pitchExpected=(b&1)!=0;cutoffExpected=(b&2)!=0;pitchCalls=0;
        levelResult=(b&4)!=0;multiResult=(b&8)!=0;platform.updates=(b&32)?0x400:0;
        bool starting=(originalFlags&1)!=0;unsigned int before=starting?initUpdates:chosenUpdates;
        bool release=(originalParams&0x200)&&v->m_rtRelease<=now;
        unsigned int effectiveFlags=release?originalFlags&~0x7e7e:originalFlags;
        bool releaseLevel=release&&s->m_Params.pLevelEnv;
        bool releaseMulti=release&&s->m_Params.pMultiEnv;
        unsigned int after=before;
        if(!(effectiveFlags&0x40)&&levelResult)after|=8;
        if(!(effectiveFlags&0x4000)&&multiResult)after|=0x10;
        if(s->m_Params.pPitchLFO)after|=0x20;
        if(s->m_Params.pMultiLFO)after|=0x40;
        expectedParams=&s->m_Params;expectedSource=s;expectedDispatch=before;expectedInputUpdates=before;
        Events want;events=Events();
        if(starting)want.add(1);
        if(v->m_PlayBuffer.m_pvBuffer&&(before&1))want.add(2);
        if(releaseLevel)want.add(3);
        if(releaseMulti&&pitchExpected)want.add(4);
        if(releaseMulti&&cutoffExpected)want.add(4);
        if(!(effectiveFlags&0x40))want.add(5);
        if(!(effectiveFlags&0x4000))want.add(6);
        if((before&0x20)&&!starting)want.add(7);
        if(s->m_Params.pPitchLFO)want.add(8);
        if((before&0x40)&&!starting)want.add(9);
        if(s->m_Params.pMultiLFO)want.add(10);
        if(after||platform.updates){want.add(12);want.add(13);want.add(v3?14:15);}
        // Argument checks for envelope helpers see input update flags; downstream dispatch sees augmented flags.
        expectedDispatch=before;
        // Dispatch expected flag storage is set by fixture wrapper below through separate expectation.
        // The helper doubles use original flag bits8/10; later bits only affect dispatch.
        expectedDispatch=after;
        v->CalcParams(s,now);
        if(events.bad)return fail(id,"dependency arguments");
        if(events.n!=want.n)return fail(id,"event count");
        for(int j=0;j<want.n;++j)if(events.a[j]!=want.a[j])return fail(id,"event order");
        unsigned int expectedFlags=effectiveFlags;
        if(release&&(s->m_Params.pLevelEnv||s->m_Params.pMultiEnv))expectedFlags|=0x20;
        expectedFlags&=~1u;
        if(v->m_ffStatus!=expectedFlags||v->m_ffParams!=(originalParams&~(release?0x200u:0u)))return fail(id,"flag preservation");
        if(v->m_fLevelEnvDelta!=(releaseLevel?.75f:.875f))return fail(id,"level release");
        if(v->m_fMultiEnvPitchDelta!=(releaseMulti&&pitchExpected?.5f:.875f)||v->m_fMultiEnvCutoffDelta!=(releaseMulti&&cutoffExpected?.5f:.875f))return fail(id,"multifunc release");
        int expectedFrames=releaseMulti?(pitchExpected?3:cutoffExpected?6:123):123;
        if(v->m_nMultiEnvFrames!=expectedFrames){printf("actual=%d expected=%d rel=%d multi=%d pitch=%d cutoff=%d flags=%x params=%x\n",v->m_nMultiEnvFrames,expectedFrames,release,releaseMulti,pitchExpected,cutoffExpected,originalFlags,originalParams);return fail(id,"release frame count");}
        ++coverage[(starting?1:0)|(release?2:0)|(v3?4:0)|(s3?8:0)];
    }
    for(int k=0;k<16;++k)if(!coverage[k])return fail(k,"coverage");
    printf("00c5acb0_TEST PASS\n");return 0;
}