#ifndef FABLE_PASS22_CREVERBCONSTANTS_H
#define FABLE_PASS22_CREVERBCONSTANTS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CReverbConstants {
public:
 virtual ~CReverbConstants();
 unsigned int m_uiNumOutputs; // +0x4
 unsigned int m_uiSamplesPerSecond; // +0x8
 unsigned int m_uiMaxReflectionsInputSamples; // +0xc
 unsigned int m_uiMaxShortReverbInputSamples; // +0x10
 unsigned int m_uiMaxLongReverbInputSamples; // +0x14
 unsigned int m_uiMaxModulationSamples; // +0x18
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char CReverbConstantsSizePass22[(sizeof(CReverbConstants)==28)?1:-1];
#endif
#endif // FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
#endif
