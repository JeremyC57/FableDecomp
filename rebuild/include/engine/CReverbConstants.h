#ifndef FABLE_PASS22_CREVERBCONSTANTS_H
#define FABLE_PASS22_CREVERBCONSTANTS_H
#ifdef FABLE_RETAIL_PASS22_FLOATREVERB_CREATE
// Original GameCODA2.10 debug CodeView: complete data layout and used original API.
class CReverbConstants {
public:
#ifdef FABLE_RETAIL_CREVERBCONSTANTS_INLINE_CTOR
 // Retail opt-in (NF99P2, q11498): the SDK 2.10 header-inline constructor and virtual destructor (CODAApi.lib carries
 // ??0CReverbConstants@@QAE@XZ / ??1CReverbConstants@@UAE@XZ as COMDATs in five objects; release ctor: vptr, 48000 at +8, 2 at +4,
 // then +0xc..+0x18 = 0; dtor: vptr only). CDecodedStream::Initialise 0x00C54510 expands both (vtable 0x012B89C4).
 CReverbConstants(){m_uiSamplesPerSecond=48000;m_uiNumOutputs=2;m_uiMaxReflectionsInputSamples=0;m_uiMaxShortReverbInputSamples=0;m_uiMaxLongReverbInputSamples=0;m_uiMaxModulationSamples=0;}
 virtual ~CReverbConstants(){}
#else
 virtual ~CReverbConstants();
#endif
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
