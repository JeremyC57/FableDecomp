#ifndef FABLE_CSAL_SW_FILTER_H
#define FABLE_CSAL_SW_FILTER_H
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
struct _CLPFILTER;struct _CEQFILTER;
#endif
class CSalSwFilter {
private:
 struct PARAMS {
 float fF;
 float fQ;
 float fG;
 };
 PARAMS m_Params;
 PARAMS m_NewParams;
 float m_fZ0;
 float m_fZ1;
 unsigned int m_nFadeSamples;
 void ProcessEQFade(short*,unsigned int);
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
public:
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView public members; native ClearHistory 0x00C6B720,
 // SetLP 0x00C6BC80, SetEQ 0x00C6BD00.
 void ClearHistory();
 void SetLP(const _CLPFILTER*,float);
 void SetEQ(const _CEQFILTER*,float);
#endif
public:
 void ProcessEQ(short*,unsigned int);
};
#endif
