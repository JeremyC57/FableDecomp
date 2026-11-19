#ifndef FABLE_PASS22_CFLOATREVERBPARAMETERTRANSLATOR_H
#define FABLE_PASS22_CFLOATREVERBPARAMETERTRANSLATOR_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CFloatReverbConstants;
class CFloatReverbParameters;
class CHLRvbParams;
class CFloatReverbParameterTranslator {
public:
 void Create(CFloatReverbConstants *);
 void Translate(CFloatReverbParameters *,CHLRvbParams *);
#ifdef FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES
private:
 void UpdateDecayTimes(CFloatReverbParameters *,CHLRvbParams *);
#endif
private:
 float m_fRefPanGain[4]; // +0x0
 float m_fShortRvbPanGain[4]; // +0x10
 float m_fLongRvbPanGain[2]; // +0x20
 CFloatReverbConstants *m_pReverbConstants; // +0x28
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CFloatReverbParameterTranslatorSizePass22[(sizeof(CFloatReverbParameterTranslator)==44)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
