#ifndef FABLE_PASS22_CHLRVBPARAMS_H
#define FABLE_PASS22_CHLRVBPARAMS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
#include "engine/_RVB_VECTOR.h"
class CHLRvbParams {
#ifdef FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
 friend class CFloatReverbParameterTranslator;
#endif
public:
 CHLRvbParams();
private:
 unsigned int m_uiUpdateFlags; // +0x0
 int m_iLevel; // +0x4
 int m_iLevelLF; // +0x8
 int m_iLevelHF; // +0xc
 float m_fDecayTime; // +0x10
 float m_fDecayTimeLFRatio; // +0x14
 float m_fDecayTimeHFRatio; // +0x18
 float m_fRefFrequencyLF; // +0x1c
 float m_fRefFrequencyHF; // +0x20
 float m_fReflectionsGain; // +0x24
 float m_fReflectionsDelay; // +0x28
 float m_fReverbGain; // +0x2c
 float m_fReverbDelay; // +0x30
 float m_fDiffusion; // +0x34
 float m_fDensity; // +0x38
 float m_fEchoTime; // +0x3c
 float m_fEchoDepth; // +0x40
 float m_fModulationTime; // +0x44
 float m_fModulationDepth; // +0x48
 _RVB_VECTOR m_ReflectionsPan; // +0x4c
 _RVB_VECTOR m_ReverbPan; // +0x58
 float m_fSize; // +0x64
public:
 float GetDecayTime()const{return m_fDecayTime;}
#ifdef FABLE_RETAIL_CHLRVBPARAMS_SETTERS
 // GameCODA 2.10 CodeView public setters (SDK header inlines; retail calls their out-of-line copies 0x00C53440..0x00C53A40).
 void SetDecayTime(float fDecayTime);
 void SetDecayLFRatio(float fDecayLFRatio);
 void SetDecayHFRatio(float fDecayHFRatio);
 void SetReflectionsGain(float fReflectionsGain);
 void SetReflectionsDelay(float fReflectionsDelay);
 void SetReverbGain(float fReverbGain);
 void SetReverbDelay(float fReverbDelay);
 void SetLFReference(float fLFReference);
 void SetHFReference(float fHFReference);
 void SetEchoTime(float fEchoTime);
 void SetEchoDepth(float fEchoDepth);
 void SetModulationTime(float fModulationTime);
 void SetModulationDepth(float fModulationDepth);
 void SetDensity(float fDensity);
 void SetDiffusion(float fDiffusion);
 void SetSize(float fSize);
#endif
#ifdef FABLE_RETAIL_CHLRVBPARAMS_SETLEVEL_INLINE
 // SDK header inlines (CODAApi.lib SalSwReverb.obj COMDATs, byte-identical to retail 0x00C53410 / 0x00C53420 / 0x00C53430).
 void SetLevel(int iLevel){m_iLevel=iLevel;m_uiUpdateFlags|=0x20;}
 void SetLevelLF(int iLevelLF){m_iLevelLF=iLevelLF;m_uiUpdateFlags|=0x40;}
 void SetLevelHF(int iLevelHF){m_iLevelHF=iLevelHF;m_uiUpdateFlags|=0x80;}
#endif
#ifdef FABLE_RETAIL_CHLRVBPARAMS_SETPAN_INLINE
 // SDK header inlines (CODAApi.lib SalSwReverb.obj COMDATs ?SetReflectionsPan / ?SetReverbPan@CHLRvbParams@@QAEXABU_RVB_VECTOR@@@Z).
 void SetReflectionsPan(const _RVB_VECTOR& vPan){m_ReflectionsPan=vPan;m_uiUpdateFlags|=0x800;}
 void SetReverbPan(const _RVB_VECTOR& vPan){m_ReverbPan=vPan;m_uiUpdateFlags|=0x4000;}
#endif
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CHLRvbParamsSizePass22[(sizeof(CHLRvbParams)==104)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
