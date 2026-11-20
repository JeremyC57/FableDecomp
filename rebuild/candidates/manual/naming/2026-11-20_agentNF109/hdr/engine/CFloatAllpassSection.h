#ifndef FABLE_PASS22_CFLOATALLPASSSECTION_H
#define FABLE_PASS22_CFLOATALLPASSSECTION_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#ifdef FABLE_RETAIL_CFLOATALLPASSSECTION_CLEAR
#include <string.h>
#endif
class CFloatAllpassSection {
public:
 virtual ~CFloatAllpassSection();
#ifdef FABLE_RETAIL_CFLOATALLPASSSECTION_CLEAR
 // Retail opt-in (NF105, family of q11545): SDK 2.10 CodeView public inline ?Clear@CFloatAllpassSection@@QAEXXZ (CODAApi.lib COMDAT:
 // the counted memset when the buffer exists, then index 0), expanded twice by the release CFloatDiffuser::Clear 0x00C75010.
 void Clear() { if (m_pBuffer) memset(m_pBuffer, 0, m_length * sizeof(float)); m_index = 0; }
#endif
#ifdef FABLE_RETAIL_CFLOATALLPASSSECTION_CTOR
 // Retail opt-in (NF109, q11607): SDK 2.10 CodeView public ??0CFloatAllpassSection@@QAE@XZ out of line (native 0x00C74F60; the retail
 // CFloatDiffuser ctor 0x00C75410 passes it to the `eh vector constructor iterator` for m_allpass[2], as the CODAApiDbg.lib ctor does).
 CFloatAllpassSection();
#endif
#ifdef FABLE_RETAIL_CFLOATALLPASSSECTION_RELEASE
 // Retail opt-in (NF109, q11607): SDK 2.10 CodeView public ?Release@CFloatAllpassSection@@QAEXXZ (CODAApi.lib release member: zero
 // m_pBuffer / m_length; the CODAApiDbg.lib body stores 0 to +4 / +8), expanded twice by the release CFloatDiffuser::Release 0x00C75000.
 void Release() { m_pBuffer = 0; m_length = 0; }
#endif
#ifdef FABLE_RETAIL_CFLOATALLPASSSECTION_PROCESSSAMPLE
 // Retail opt-in (NF102, q11551): SDK 2.10 CodeView public ?ProcessSample@CFloatAllpassSection@@QAEMM@Z (CODAApi.lib FloatDiffuser.obj),
 // expanded twice per sample by the release CFloatDiffuser::Process 0x00C75140 (allpass coefficient 0.7f); body Inferred from those bytes.
 float ProcessSample(float fInput)
 {
  float fDelayed = m_pBuffer[m_index];
  float fValue = fDelayed * 0.7f + fInput;
  m_pBuffer[m_index] = fValue;
  if (++m_index == m_length)
   m_index = 0;
  return fDelayed - fValue * 0.7f;
 }
#endif
private:
 float * m_pBuffer; // +0x4
 unsigned int m_length; // +0x8
 unsigned int m_index; // +0xc
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatAllpassSectionSizePass22[(sizeof(CFloatAllpassSection)==16)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
