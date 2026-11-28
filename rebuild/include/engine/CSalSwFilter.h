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
#ifdef FABLE_RETAIL_CSALSWFILTER_VOICE_PORT30
 // GameCODA SalSwFilter release and debug objects agree on this private fade member.
 void ProcessLPFade(short*,unsigned int);
#endif
 void ProcessEQFade(short*,unsigned int);
#ifdef FABLE_RETAIL_CSALSWFILTER_SETFREQUENCY
 // Retail opt-in (NF99, q11410..q11412): SDK 2.10 CodeView private ?SetFrequency@CSalSwFilter@@AAEXIM@Z (native 0x00C6BBD0; the
 // release SetLP / SetEQ call it at +0x0D, REL32 to the same symbol in CODAApi.lib SalSwFilter.obj).
 void SetFrequency(unsigned int,float);
#endif
#ifdef FABLE_RETAIL_CSALSWFILTER_SETQ_INLINE
 // Retail opt-in (NF99): SDK private ?SetQ@CSalSwFilter@@AAEXM@Z, expanded in place by the release SetLP 0x00C6BC80 / SetEQ 0x00C6BD00
 // (`fld 1.0 ; fdiv ; fst`, no call); the body is those rows' inline (the Dbg SalSwFilter.obj keeps it out of line).
 void SetQ(float fQ)
 {
  m_NewParams.fQ = 1.0f / fQ;
  if (m_NewParams.fF * m_NewParams.fQ > 1.0f)
   m_NewParams.fQ = 1.0f / m_NewParams.fF;
 }
#endif
#ifdef FABLE_RETAIL_CSALSWFILTER_SETGAIN_INLINE
 // Retail opt-in (NF99): SDK private ?SetGain@CSalSwFilter@@AAEXM@Z, expanded in place by the release SetEQ 0x00C6BD00 (constants
 // 0.09f / 1.0f / 1.1f, the -1.001f immediate store); parameter name descriptive (no Dbg local record read).
 void SetGain(float fGain)
 {
  if (fGain < 0.09f)
   m_NewParams.fG = -1.001f;
  else
   m_NewParams.fG = (fGain - 1.0f) * 1.1f;
 }
#endif
#ifdef FABLE_RETAIL_CSALSWFILTER_ISHISTORYCLEAR_INLINE
 // Retail opt-in (NF99): SDK private const ?IsHistoryClear@CSalSwFilter@@ABE_NXZ, expanded in place by the release SetLP / SetEQ
 // (`fld [ecx+18h] ; fld 0.0 ; fucompp`: the member is loaded first).
 bool IsHistoryClear() const { return 0.0f == m_fZ0 && 0.0f == m_fZ1; }
#endif
#ifdef FABLE_RETAIL_CSALSW3D_CALCPARAMS
public:
 // Retail opt-in (hand4c w24, row 0x00C54C10): SDK 2.10 CodeView public members; native ClearHistory 0x00C6B720,
 // SetLP 0x00C6BC80, SetEQ 0x00C6BD00.
 void ClearHistory();
 void SetLP(const _CLPFILTER*,float);
 void SetEQ(const _CEQFILTER*,float);
#endif
public:
#ifdef FABLE_RETAIL_CSALSW_UPDATE_PASS30
void ProcessLP(short*,unsigned int);
#endif
 void ProcessEQ(short*,unsigned int);
};
#endif
