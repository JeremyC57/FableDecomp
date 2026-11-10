#ifndef FABLE_CODA_SOURCE_TYPES_H
#define FABLE_CODA_SOURCE_TYPES_H
// Original GameCODA 2.10 CodeView types and SDK public declarations.
struct _CLEVELENVELOPE;
struct _CMULTIFUNCENVELOPE;
struct _CPITCHLFO;
struct _CMULTIFUNCLFO;
struct _CLPFILTER;
struct _CEQFILTER;
struct CODASOURCE;
struct CQUEUEFORMAT;
struct _CLEVELENVELOPE {
 float fDelay;
 float fAttack;
 float fHold;
 float fDecay;
 float fSustain;
 float fRelease;
};
struct _CMULTIFUNCENVELOPE {
 float fDelay;
 float fAttack;
 float fHold;
 float fDecay;
 float fSustain;
 float fRelease;
 float fPitchScale;
 float fFilterCutOff;
};
struct _CPITCHLFO {
 float fDelay;
 float fFrequency;
 float fPitchModulation;
};
struct _CMULTIFUNCLFO {
 float fDelay;
 float fFrequency;
 float fPitchModulation;
 float fFilterCutOff;
 int iLevelModulation;
};
struct _CLPFILTER {
 unsigned int uiFreq;
 float fQ;
};
struct _CEQFILTER {
 unsigned int uiFreq;
 float fGain;
 float fQ;
};
struct CODASOURCE {
 unsigned int ffUpdate;
 float fGain;
 int iLevel;
 float fPitch;
 _CLPFILTER *pLPFilter;
 _CEQFILTER *pEQFilter;
 _CLEVELENVELOPE *pLevelEnv;
 _CMULTIFUNCENVELOPE *pMultiEnv;
 _CPITCHLFO *pPitchLFO;
 _CMULTIFUNCLFO *pMultiLFO;
};
struct CQUEUEFORMAT {
 unsigned int ffFormat;
 unsigned int nChannels;
 unsigned int nSamplesPerSec;
};
typedef void* CHANDLE;
typedef int CBOOL;
#define C_ERR_NOTALLOCATED (-7)
struct _CQUEUEDESC;struct _CBUFFERDESC;struct EAXSOURCE_tag;class CEAXBuffer;class CSalBuffer;class CSalVoice;
#endif
