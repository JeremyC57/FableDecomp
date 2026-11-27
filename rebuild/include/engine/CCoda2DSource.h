#ifndef FABLE_CCODA2DSOURCE_H
#define FABLE_CCODA2DSOURCE_H
#if defined(FABLE_RETAIL_PASS22_CCODACONTEXT_CREATE) || defined(FABLE_RETAIL_CCODACONTEXT_CREATE2D)
#include "engine/CCodaSource.h"
#include "engine/Coda2DSourceTypes.h"
class CCoda2DSource:public CCodaSource {
public:CCoda2DSource();virtual ~CCoda2DSource();
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE2D
// Retail opt-in (Codex pass22): original SDK CodeView plus native00C3DA60 virtual Create.
virtual bool Create(unsigned int);
#endif
#ifdef FABLE_RETAIL_CSALSW2D_CALCPARAMS_PASS29
 const CODA2DSOURCE& Get2DParams() const { return m_2D; }
#endif
private:
 CODA2DSOURCE m_2D;
 _CSPEAKERGAINS m_SpeakerGains;
 _CSPEAKERLEVELS m_SpeakerLevels;
 _CSPEAKERGAIN m_SpeakerGain[8];
 _CSPEAKERLEVEL m_SpeakerLevel[8];
 _CLPFILTER m_StereoLPFilter;
 _CEQFILTER m_StereoEQFilter;
};
#ifdef FABLE_RETAIL_CCODACONTEXT_CREATE2D
#if !defined(FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS) || defined(FABLE_RETAIL_CCODACONTEXT_PROCESS)
inline CCoda2DSource* CCodaSource::Get2D()const {
 return Is3D()?0:static_cast<CCoda2DSource*>(const_cast<CCodaSource*>(this));
}
#endif
FABLE_STATIC_ASSERT_AT(27,sizeof(CCoda2DSource)==444);
#endif
#endif
#endif
