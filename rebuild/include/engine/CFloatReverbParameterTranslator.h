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
#ifdef FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_TRANSLATE
private:
 // Retail opt-in (NF99, q11477): the private CODAApiDbg CodeView members the release Translate 0x00C783F0 calls (REL32 in CODAApi.lib
 // FloatReverbParameterTranslator.obj ?Translate@...: +0x1e UpdateSize, +0x38 / +0x4a UpdateDiffusion, +0x9f UpdateEchoTime,
 // +0xfb UpdateModulationDepth, +0x10c / +0x126 UpdateReverbDelay, +0x11d UpdateReflectionsDelay, +0x153 UpdateReflectionsOutputGain,
 // +0x162 UpdateReverbPan, +0x172 UpdateReverbOutputGain). Native addresses: hand4d NOTES (b10 NAME_FIXES).
 void UpdateSize(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateDiffusion(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateEchoTime(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateModulationDepth(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateReverbDelay(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateReflectionsDelay(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateReflectionsOutputGain(CFloatReverbParameters *,CHLRvbParams *);
 void UpdateReverbPan(CHLRvbParams *);
 void UpdateReverbOutputGain(CFloatReverbParameters *,CHLRvbParams *);
#endif
#ifdef FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_PANTOGAINS
private:
 // Retail opt-in (NF99P2, q11510): SDK 2.10 CodeView private static ?PanToGains@CFloatReverbParameterTranslator@@CAXPBU_RVB_VECTOR@@PAMII@Z
 // (native 0x00C77D90; byte-identical CODAApi.lib member, 557 bytes; the release Translate calls it at REL32 +0x140 for the
 // inlined UpdateReflectionsPan, UpdateReverbPan 0x00C783B0 twice).
 static void PanToGains(const struct _RVB_VECTOR *,float *,unsigned int,unsigned int);
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
