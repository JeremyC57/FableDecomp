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
// Retail-asm: 0x00C5CC3E `fistp dword ptr [esp+10h]` and 0x00C5CD81 `fistp dword ptr [esp+18h]` (FtoL's sal_ftoi / sal_round, expanded in this function); LIBPK_POW10_BLOCK runs in this TU's MillibelsToAmplitude, 0x00C5C188 `f2xm1` / 0x00C5C175 `fscale`.
#include "engine/CCoda2DSource.h"
#include "engine/CSalSw2DVoice.h"
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