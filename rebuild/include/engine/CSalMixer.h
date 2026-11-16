#ifndef FABLE_CSALMIXER_H
#define FABLE_CSALMIXER_H
#ifdef FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#include "engine/CCodaObjectBase.h"
// Layout: original SDK classes; native C591F0 has six pointers and 772 int samples.
// Limiter constructor C3EFA0 creates six380-byte CSalLimiter objects at+c34.
enum LimiterState {LIMITER_STATE_INACTIVE,LIMITER_STATE_WARM_UP,LIMITER_STATE_ACTIVE};
class CSalLimiter {
public: CSalLimiter();virtual ~CSalLimiter();
private:
 int m_buffer[88];unsigned int m_cursor;
 float m_fMagnitudePeak,m_fMagnitudeTrackerValue;
 unsigned int m_magnitudeCount;LimiterState m_state;unsigned int m_warmupCount;
};
// Native table has11 slots. This older fork has gain-taking int/float overloads;
// newer SDK has only three MixFrame overloads and adds two-channel virtuals.
class CSalMixer:public CCodaObjectBase {
public:
 CSalMixer();virtual ~CSalMixer();
 virtual void Reset();virtual void MixFrameClear();
 virtual void MixFrame(const float*,int*);
 virtual void MixFrame(const float*,int*,unsigned short);
 virtual void MixFrame(const float*,int*,float);
 virtual void MixFrame(const int*,int*);
 virtual void MixFrame(const int*,int*,unsigned short);
 virtual void MixFrame(const short*,int*,unsigned short);
 virtual void MixFrameFade(const short*,int*,unsigned short,short);
 virtual void GetFrame(short*);
protected:
 int* m_mixBufs[6];unsigned int m_headroomBits,m_nOutputChannels;int m_buffer[772];
};
class CSalMixerLimiter:public CSalMixer {
public:
 CSalMixerLimiter();virtual ~CSalMixerLimiter();
 virtual void Reset();virtual void GetFrame(short*);
private: CSalLimiter m_limiter[6];
};
class CSalMixerLimiterMMX:public CSalMixerLimiter {
public:
 virtual void MixFrameClear();
 virtual void MixFrame(const short*,int*,unsigned short);
 virtual void MixFrameFade(const short*,int*,unsigned short,short);
};
class CSalMixerLimiterSSE2:public CSalMixerLimiterMMX {
public:
 virtual void MixFrame(const float*,int*);
 virtual void MixFrame(const int*,int*);
 virtual void MixFrame(const short*,int*,unsigned short);
 virtual void MixFrameFade(const short*,int*,unsigned short,short);
};
FABLE_STATIC_ASSERT(sizeof(CSalLimiter)==380);
FABLE_STATIC_ASSERT(sizeof(CSalMixer)==0xc34);
FABLE_STATIC_ASSERT(sizeof(CSalMixerLimiter)==0x151c);
#endif
#endif
