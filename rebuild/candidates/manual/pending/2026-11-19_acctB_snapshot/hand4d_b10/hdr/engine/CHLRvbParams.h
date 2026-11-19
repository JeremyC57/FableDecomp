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
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CHLRvbParamsSizePass22[(sizeof(CHLRvbParams)==104)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
