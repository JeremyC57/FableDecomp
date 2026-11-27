// 00c5ca60 CSalSw2DVoice::CalcParams
// Leaf: ?CalcParams@CSalSw2DVoice@@QAEXPBVCCoda2DSource@@I@Z
// Flags: (none)
// Retail-asm-helper: FtoL
// Retail-asm: 00C5CC3E and 00C5CD81, single x87 integer stores in sal_ftoi and sal_round.
// Retail-asm-helper: LIBPK_POW10_BLOCK
// Retail-asm: 00C5C130, the GameCODA sal_alog helper expanded inside sal_levelToGain.
#define FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE
#define FABLE_RETAIL_CSALSW3D_CALCPARAMS
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define private public
#define protected public
#include "engine/CCoda2DSource.h"
#include "engine/CSalSw2DVoice.h"
#undef private
#undef protected
#include "engine/SalMath.h"
#include "libpk/Pow10.h"

static float sal_alog(float x)
{
    float scale=3.32192802f,one=1.0f,n,dummy,y;
    LIBPK_POW10_BLOCK(scale,x,n,dummy,one,y);
    return y;
}
static float sal_levelToGain(int level)
{
    if(level<=-10000) return 0.0f;
    return sal_alog(level/2000.0f);
}
ChanId CSalSw2DVoice::SpeakerIdToChannel(CSPEAKERID id)
{
    switch(id) {
    case C_SPEAKER_FRONTLEFT: return SAL_CHAN_LEFT;
    case C_SPEAKER_FRONTRIGHT: return SAL_CHAN_RIGHT;
    case C_SPEAKER_REARLEFT: return SAL_CHAN_LEFT_REAR;
    case C_SPEAKER_REARRIGHT: return SAL_CHAN_RIGHT_REAR;
    case C_SPEAKER_CENTRE: return SAL_CHAN_CENTRE;
    case C_SPEAKER_LFE: return SAL_CHAN_LFE;
    default: return SAL_CHAN_UNKNOWN;
    }
}
inline void CSalSw2DVoice::SetSingleChannelGain(unsigned int chan,float fGain)
{
    unsigned int iGain=sal_ftoi(fGain*m_fGains[chan]*32767.0f);
    if(iGain>32767) iGain=32767;
    if(IsStarting()) m_gainArray[chan]=(unsigned short)iGain;
    else {
        m_gainArrayNew[chan]=(unsigned short)iGain;
        if(m_gainArrayNew[chan]!=m_gainArray[chan]) m_bUpdateGain=true;
    }
}
inline void CSalSw2DVoice::SetReverbLevel(int iLevel)
{
    if(iLevel!=m_iReverbLevel) {
        m_iReverbLevel=iLevel;
        unsigned short iGain=(unsigned short)sal_round(sal_levelToGain(iLevel)*32767.0f);
        if(IsStarting()) m_gainEnvironment=iGain;
        else {m_gainEnvironmentNew=iGain;m_bUpdateEnvironmentGain=true;}
    }
}
void CSalSw2DVoice::CalcParams(const CCoda2DSource* pcSource,unsigned int ffUpdate)
{
    const CODASOURCE& params=pcSource->GetParams();
    const CODA2DSOURCE& params2D=pcSource->Get2DParams();
    if(ffUpdate&0x74) SetPitch(params.fPitch,params);
    if(IsLevelEnvReleased()) ReleasePlayBuffer();
    if(params2D.pSpeakerLevels && ((ffUpdate&0x400)||((ffUpdate&0x48)&&(m_ffParams&0x40000)))) {
        for(int i=0;i<params2D.pSpeakerLevels->nSpeakers;++i) {
            ChanId chan=SpeakerIdToChannel(params2D.pSpeakerLevels->pParams[i].id);
            m_fGains[chan]=sal_levelToGain(params2D.pSpeakerLevels->pParams[i].iLevel);
        }
        m_ffParams=(m_ffParams&~0x70000)|0x40000;
    }
    if(params2D.pSpeakerGains && ((ffUpdate&0x800)||((ffUpdate&0x48)&&(m_ffParams&0x20000)))) {
        for(int i=0;i<params2D.pSpeakerGains->nSpeakers;++i) {
            ChanId chan=SpeakerIdToChannel(params2D.pSpeakerGains->pParams[i].id);
            m_fGains[chan]=params2D.pSpeakerGains->pParams[i].fGain;
        }
        m_ffParams=(m_ffParams&~0x70000)|0x20000;
    }
    if((ffUpdate&0x202)||((ffUpdate&0x48)&&(m_ffParams&0x10000))) {
        PanToGains(params2D.fPan,m_fGains[SAL_CHAN_LEFT],m_fGains[SAL_CHAN_RIGHT]);
        m_fGains[SAL_CHAN_LEFT]*=params.fGain;
        m_fGains[SAL_CHAN_RIGHT]*=params.fGain;
        PanToGains(params2D.fPan,m_fGains[SAL_CHAN_LEFT_REAR],m_fGains[SAL_CHAN_RIGHT_REAR]);
        m_fGains[SAL_CHAN_LEFT_REAR]*=params.fGain;
        m_fGains[SAL_CHAN_RIGHT_REAR]*=params.fGain;
        m_ffParams=(m_ffParams&~0x70000)|0x10000;
    }
    if(ffUpdate&0xe4a) {
        float fEnvLFOGain=m_fMultiLFOGain*m_fLevelEnvGain;
        for(unsigned int i=0;i<6;++i) SetSingleChannelGain(i,fEnvLFOGain);
    }
    if(ffUpdate&0x11d0) {
        m_ffStatus&=~0x1e0000;
        float fCutoff=m_fMultiLFOCutoff*m_fMultiEnvCutoff;
        if(pcSource->IsStereoFilter()) {
            if(params.pLPFilter) {m_Filter[0].SetLP(params.pLPFilter,fCutoff);m_ffStatus|=0x20000;}
            else if(params.pEQFilter) {m_Filter[0].SetEQ(params.pEQFilter,fCutoff);m_ffStatus|=0x40000;}
            else m_Filter[0].ClearHistory();
            if(params2D.pStereoLPFilter) {m_Filter[1].SetLP(params2D.pStereoLPFilter,fCutoff);m_ffStatus|=0x80000;}
            else if(params2D.pStereoEQFilter) {m_Filter[1].SetEQ(params2D.pStereoEQFilter,fCutoff);m_ffStatus|=0x100000;}
            else m_Filter[1].ClearHistory();
        } else {
            if(params.pLPFilter) {m_Filter[0].SetLP(params.pLPFilter,fCutoff);m_ffStatus|=0x20000;}
            else m_Filter[0].ClearHistory();
            if(params.pEQFilter) {m_Filter[1].SetEQ(params.pEQFilter,fCutoff);m_ffStatus|=0x100000;}
            else m_Filter[1].ClearHistory();
        }
    }
    if(ffUpdate&0x4000) SetReverbLevel(params2D.iReverbLevel);
}

#include <assert.h>
#include <stdio.h>
CCodaObjectBase::~CCodaObjectBase() {}
CCodaObject::~CCodaObject() {}
CCodaSource::~CCodaSource() {}
bool CCodaSource::Is3D() const { return 0; }
bool CCodaSource::IsCreated() const { return 0; }
bool CCodaSource::Create(unsigned int) { return 0; }
CCoda2DSource::~CCoda2DSource() {}
int CSalVoice::Create(const CCodaSource *) { return 0; }
int CSalVoice::SetBuffer(const CCodaSource *) { return 0; }
void CSalVoice::Release() {}
bool CSalVoice::IsSoftware() const { return 0; }
bool CSalVoice::Is3D() const { return 0; }
bool CSalVoice::IsCreated() const { return 0; }
unsigned int CSalVoice::PlayBuffer(const CCodaSource *) { return 0; }
unsigned int CSalVoice::PlayQueue(CCodaSource *,CREFTIME ) { return 0; }
unsigned int CSalVoice::Update(CCodaSource *,CREFTIME ) { return 0; }
unsigned int CSalVoice::Resume(CREFTIME ) { return 0; }
bool CSalVoice::Stop(CREFTIME ) { return 0; }
void CSalVoice::ReleaseEnvelopes(CREFTIME ) {}
unsigned int CSalVoice::GetPlayCursor() const { return 0; }
void CSalVoice::SetPlayCursor(unsigned int ) {}
int CSalVoice::EAXSet(const _GUID *,int ,void * ,int ) { return 0; }
int CSalVoice::EAXGet(const _GUID *,int ,void * ,int ) { return 0; }
CSalVoice::~CSalVoice() {}
CSalBaseVoice::~CSalBaseVoice() {}
bool CSalBaseVoice::IsCreated() const { return 0; }
int CSalBaseVoice::Create(const CCodaSource *) { return 0; }
int CSalBaseVoice::SetBuffer(const CCodaSource *) { return 0; }
void CSalBaseVoice::Release() {}
unsigned int CSalBaseVoice::PlayBuffer(const CCodaSource *) { return 0; }
unsigned int CSalBaseVoice::Resume(CREFTIME ) { return 0; }
void CSalBaseVoice::ReleaseEnvelopes(CREFTIME ) {}
int CSalBaseVoice::EAXSet(const _GUID *,int ,void * ,int ) { return 0; }
int CSalBaseVoice::EAXGet(const _GUID *,int ,void * ,int ) { return 0; }
void CSalBaseVoice::InitPlay() {}
void CSalBaseVoice::SetPlayBuffer(const CSalBuffer &,bool ) {}
CSalSwBaseVoice::~CSalSwBaseVoice() {}
bool CSalSwBaseVoice::IsSoftware() const { return 0; }
int CSalSwBaseVoice::Create(const CCodaSource *) { return 0; }
void CSalSwBaseVoice::Release() {}
unsigned int CSalSwBaseVoice::PlayQueue(CCodaSource *,CREFTIME ) { return 0; }
bool CSalSwBaseVoice::Stop(CREFTIME ) { return 0; }
unsigned int CSalSwBaseVoice::Update(CCodaSource *,CREFTIME ) { return 0; }
unsigned int CSalSwBaseVoice::GetPlayCursor() const { return 0; }
void CSalSwBaseVoice::SetPlayCursor(unsigned int ) {}
void CSalSwBaseVoice::InitPlay() {}
void CSalSwBaseVoice::SetPlayBuffer(const CSalBuffer &,bool ) {}
unsigned int CSalSwBaseVoice::GetPlatformObjectSize() { return 0; }
unsigned int CSalSwBaseVoice::GetPlatformBufferLoopStart() { return 0; }
unsigned int CSalSwBaseVoice::GetPlatformBufferEnd() { return 0; }
unsigned int CSalSwBaseVoice::InitParams(const CCodaSource *,const CODASOURCE &) { return 0; }
void CSalSwBaseVoice::ConvertFormat(CSalSwBuffer &,const void *,unsigned int ,unsigned int ) {}
void CSalSwBaseVoice::Mix(CSalSwBuffer &,unsigned int ,bool ) {}
void CSalSwBaseVoice::MixReverbInput(CSalReverb *,CSalSwBuffer &,unsigned int ) {}
void CSalSwBaseVoice::MixChannel(CSalSwBuffer &,unsigned int ,unsigned int ) {}
void CSalSwBaseVoice::StartRunOut(CREFTIME ) {}
CSalSwVoice::~CSalSwVoice() {}
CSalSw2DVoice::~CSalSw2DVoice() {}
CSalSw2DVoice::CSalSw2DVoice() {}
CCoda2DSource::CCoda2DSource() {}
#include <stdlib.h>
#include <math.h>
struct FilterCall { const CSalSwFilter* filter; int kind; const void* params; float cutoff; };
static FilterCall filterCalls[2];
static unsigned filterCount,panCount,pitchCount,releaseCount;
static const CODASOURCE* seenPitchParams;
static const CSalSwBuffer* seenRelease;
static float seenPitch;
static void recordFilter(const CSalSwFilter* filter,int kind,const void* params,float cutoff)
{
    assert(filterCount<2);
    FilterCall& call=filterCalls[filterCount++];
    call.filter=filter; call.kind=kind; call.params=params; call.cutoff=cutoff;
}
void CSalSwFilter::ClearHistory(){ recordFilter(this,0,0,0); }
void CSalSwFilter::SetLP(const _CLPFILTER* p,float v){ recordFilter(this,1,p,v); }
void CSalSwFilter::SetEQ(const _CEQFILTER* p,float v){ recordFilter(this,2,p,v); }
void CSalBaseVoice::PanToGains(float pan,float& left,float& right)
{
    assert(pan==0.25f); ++panCount; left=0.25f;right=0.75f;
}
void CSalSwBaseVoice::SetPitch(float pitch,const CODASOURCE& params)
{
    ++pitchCount;seenPitch=pitch;seenPitchParams=&params;
}
CSalSwBuffer::~CSalSwBuffer(){}
void CSalSwBuffer::Release(){++releaseCount;seenRelease=this;}
extern "C" void __cdecl CodaFree(void* p) { free(p); }
static unsigned short referenceGain(float g)
{
    double x=(double)(float)(g*32767.0f)-0.5;
    int whole=(int)floor(x);
    double fraction=x-whole;
    int nearest=whole+(fraction>0.5 || (fraction==0.5 && (whole%2)!=0));
    if(nearest<0) ++nearest;
    return (unsigned short)((unsigned int)nearest>32767?32767:nearest);
}
int main()
{
    CSalSw2DVoice voice;
    CCoda2DSource source;
    _CLPFILTER low[2]; _CEQFILTER eq[2];
    _CSPEAKERGAIN gains[6]; _CSPEAKERLEVEL levels[6];
    const CSPEAKERID speaker[]={C_SPEAKER_CENTRE,C_SPEAKER_LFE,C_SPEAKER_FRONTLEFT,C_SPEAKER_FRONTRIGHT,C_SPEAKER_REARLEFT,C_SPEAKER_REARRIGHT};
    const unsigned order[]={4,5,0,1,2,3};
    for(unsigned i=0;i<6;++i) { gains[i].id=speaker[i];gains[i].fGain=(float)(i%3)*0.5f;levels[i].id=speaker[i];levels[i].iLevel=(i&1)?0:-10000; }
    _CSPEAKERGAINS gainList={6,gains}; _CSPEAKERLEVELS levelList={6,levels};
    const unsigned masks[]={0,1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,0x7fff};
    const unsigned modes[]={0,0x10000,0x20000,0x40000};
    unsigned trials=0;
    for(unsigned k=0;k<sizeof(masks)/sizeof(*masks);++k)
    for(unsigned mode=0;mode<4;++mode)
    for(unsigned options=0;options<256;++options) {
        unsigned update=masks[k];bool start=(options&1)!=0, released=(options&2)!=0,stereo=(options&4)!=0;
        bool useLevels=(options&8)!=0,useGains=(options&16)!=0;
        gainList.nSpeakers=(options&32)?6:0;levelList.nSpeakers=(options&64)?6:0;
        bool different=(options&128)!=0;
        source.m_ffStatus=stereo?0x10000:0;
        source.m_Params.fPitch=1.5f;source.m_Params.fGain=0.5f;
        source.m_Params.pLPFilter=(options&8)?&low[0]:0;source.m_Params.pEQFilter=(options&16)?&eq[0]:0;
        source.m_2D.pStereoLPFilter=(options&32)?&low[1]:0;source.m_2D.pStereoEQFilter=(options&64)?&eq[1]:0;
        source.m_2D.fPan=0.25f;source.m_2D.iReverbLevel=different?0:-10000;
        source.m_2D.pSpeakerLevels=useLevels?&levelList:0;source.m_2D.pSpeakerGains=useGains?&gainList:0;
        unsigned status=0x200000|(start?1:0)|(released?64:0)|0x1e0000;
        voice.m_ffStatus=status; voice.m_ffParams=modes[mode]|0x10;
        voice.m_pcBuffer=0;
        voice.m_fMultiLFOGain=2;voice.m_fLevelEnvGain=0.5f;voice.m_fMultiLFOCutoff=0.5f;voice.m_fMultiEnvCutoff=0.5f;
        voice.m_iReverbLevel=-10000;voice.m_gainEnvironment=17;voice.m_gainEnvironmentNew=18;
        voice.m_bUpdateGain=false;voice.m_bUpdateEnvironmentGain=false;
        float expected[6];unsigned short original[6];
        for(unsigned ch=0;ch<6;++ch) {expected[ch]=voice.m_fGains[ch]=0.5f;original[ch]=voice.m_gainArray[ch]=different?0:referenceGain(0.5f);voice.m_gainArrayNew[ch]=23;}
        unsigned active=modes[mode];
        if(useLevels && ((update&1024)||((update&0x48)&&active==0x40000))) {
            for(int j=0;j<levelList.nSpeakers;++j)expected[order[j]]=(j&1)?1.0f:0.0f;
            active=0x40000;
        }
        if(useGains && ((update&2048)||((update&0x48)&&active==0x20000))) {
            for(int j=0;j<gainList.nSpeakers;++j)expected[order[j]]=gains[j].fGain;
            active=0x20000;
        }
        bool pan=(update&0x202)||((update&0x48)&&active==0x10000);
        if(pan){expected[0]=expected[2]=0.125f;expected[1]=expected[3]=0.375f;active=0x10000;}
        pitchCount=releaseCount=panCount=filterCount=0;
        voice.CalcParams(&source,update);
        assert(pitchCount==((update&0x74)!=0));
        if(pitchCount)assert(seenPitchParams==&source.m_Params && seenPitch==1.5f);
        assert(releaseCount==(unsigned)released);
        if(released)assert(seenRelease==&voice.m_PlayBuffer && voice.m_pcBuffer==0);
        assert(panCount==(pan?2U:0U) && voice.m_ffParams==(active|0x10));
        bool dirty=false;
        for(unsigned ch=0;ch<6;++ch) {
            assert(voice.m_fGains[ch]==expected[ch]);unsigned short g=referenceGain(expected[ch]);
            assert(voice.m_gainArray[ch]==((start&&(update&0xe4a))?g:original[ch]));
            assert(voice.m_gainArrayNew[ch]==((!start&&(update&0xe4a))?g:23));
            dirty=dirty||(!start&&(update&0xe4a)&&original[ch]!=g);
        }
        assert(voice.m_bUpdateGain==dirty);
        if(update&0x11d0) {
            assert(filterCount==2);status&=~0x1e0000;
            const void* chosen[2];int kind[2];
            if(source.m_Params.pLPFilter){chosen[0]=&low[0];kind[0]=1;}
            else if(stereo&&source.m_Params.pEQFilter){chosen[0]=&eq[0];kind[0]=2;}
            else {chosen[0]=0;kind[0]=0;}
            if(stereo&&source.m_2D.pStereoLPFilter){chosen[1]=&low[1];kind[1]=1;}
            else if(stereo?source.m_2D.pStereoEQFilter!=0:source.m_Params.pEQFilter!=0){chosen[1]=stereo?&eq[1]:&eq[0];kind[1]=2;}
            else {chosen[1]=0;kind[1]=0;}
            for(unsigned j=0;j<2;++j){assert(filterCalls[j].filter==&voice.m_Filter[j] && filterCalls[j].kind==kind[j] && filterCalls[j].params==chosen[j]);if(kind[j]){assert(filterCalls[j].cutoff==0.25f);status|=(j?0x80000:0x20000)<<(kind[j]-1);}}
        }else assert(filterCount==0);
        assert(voice.m_ffStatus==status);
        bool change=(update&0x4000)&&different;
        assert(voice.m_iReverbLevel==(change?0:-10000));
        assert(voice.m_gainEnvironment==(change&&start?32767:17));
        assert(voice.m_gainEnvironmentNew==(change&&!start?32767:18));
        assert(voice.m_bUpdateEnvironmentGain==(change&&!start));++trials;
    }
    assert(trials==17408 && referenceGain(2.0f)==32767);
    voice.m_ffStatus=1;voice.m_fGains[0]=1.0f;voice.SetSingleChannelGain(0,2.0f);assert(voice.m_gainArray[0]==32767);
    assert(voice.SpeakerIdToChannel((CSPEAKERID)0)==SAL_CHAN_UNKNOWN);
    puts("00c5ca60_TEST PASS");return 0;
}
