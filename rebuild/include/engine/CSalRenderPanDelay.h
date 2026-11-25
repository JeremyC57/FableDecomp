#ifndef FABLE_CSALRENDERPANDELAY_H
#define FABLE_CSALRENDERPANDELAY_H
// GameCODA HRTF-delay panner (NF447; constructor 0x00C6CA20, destructor 0x00C6CAC0, vtable 0x012BED7C, slots 0x00C6CAD0 / 0x00C6CC60 /
// 0x00C6D120 / 0x00C6D150). Class, base, member and method names, access and the member order: the GameCODA 2.10 SDK CODAApiDbg.lib
// Debug/SalRenderPanDelay.obj CodeView (LF_CLASS CSalRenderPanDelay : public CSalRender). Offsets and array bounds are retail's, never the
// SDK's: the retail CSalRender is 0x28 bytes (engine/CSalRender.h), and retail keeps 6 entries where SDK 2.10 keeps 4 (ParamsPanDelay::pan,
// m_fGainPan, m_itd): `push 214h` before `call 0x00C6CA20` in CSalSw3DVoice::CreateRenderer 0x00C55270 (SDK 0x1E8), and the constructor
// clears +0x17C..+0x1DB (two ParamsPanDelay, `rep stosd` x 12 dwords each) and +0x1E4..+0x213 (two 6-entry arrays). m_audioBuffer keeps
// the SDK size (short[168], 0x150 bytes): m_pAudio is at +0x178 (`mov edi,[ebx+178h]` in Process 0x00C6CC60).
// Process / SetGain / SetPosition use the CSalRender.h SDK slot signatures; retail indexes the output array 0..3 only (0x00C6CC9F..0x00C6CD0E:
// outputs[2] / [3] behind the 4-speaker test `cmp [CSalPlatform+1BCh],4`), so `int*(&)[4]` is the retail type too.
// The private helpers Params3DInit (0x00C6CEF0) and UpdateFilters (0x00C6CF40): SDK CodeView LF_ONEMETHOD private thiscall records
// 0x1208 / 0x120A; the SDK Release object calls UpdateFilters from SetGain / SetPosition and Params3DInit from UpdateFilters, as retail
// 0x00C6D13C / 0x00C6D341 -> 0x00C6CF40 -> 0x00C6CEF0 (four sites); retail pops 4 / 12 argument bytes = the SDK signatures.
// The SDK's private static mixers (MixFadeIn / MixFadeOut / Mix) are not declared: their landed rows keep their own spellings
// (0x00C6CAF0 / 0x00C6CB80 / 0x00C6CC10) until they are ported with a body tie.
#if !defined(FABLE_RETAIL_CSALRENDER_MEMBERS) || !defined(FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23)
#error engine/CSalRenderPanDelay.h needs FABLE_RETAIL_CSALRENDER_MEMBERS and FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23 (retail CSalRender layout)
#endif
#include "engine/CSalRender.h"
struct FilterPanDelay {               // SDK SalRenderPanDelay.obj CodeView, 8 bytes (retail and SDK)
 unsigned itd;                        // +0 delay in samples
 unsigned short gain;                 // +4 Q15 gain
 short gainDelta;                     // +6 per-sample ramp step
};
struct ParamsPanDelay {
 FilterPanDelay pan[6];               // retail 6 entries (0x30); SDK 2.10 has 4
};
class CSalRenderPanDelay:public CSalRender {
public:
 CSalRenderPanDelay();                                                    // 0x00C6CA20
 virtual ~CSalRenderPanDelay();                                           // 0x00C6CAC0
 virtual void ClearFilterHistory();                                       // slot 1, 0x00C6CAD0
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6CC60
 virtual void SetGain(float,const float*,bool);                           // slot 3, 0x00C6D120 (also the SIMD classes' slot 3)
 virtual void SetPosition(float,float,float,float,const float*,bool,bool); // slot 4, 0x00C6D150 (also the SIMD classes' slot 4)
protected:
 short m_audioBuffer[168];            // +0x28: 32 samples of history + 128 input samples, used 16-byte aligned through m_pAudio
 short* m_pAudio;                     // +0x178
 ParamsPanDelay m_paramsA;            // +0x17C
 ParamsPanDelay m_paramsB;            // +0x1AC
 ParamsPanDelay* m_pParamsCurrent;    // +0x1DC
 ParamsPanDelay* m_pParamsNew;        // +0x1E0
private:
 float m_fGainPan[6];                 // +0x1E4
 unsigned m_itd[6];                   // +0x1FC
 void Params3DInit(FilterPanDelay* pFilter,float fGainPan,unsigned itd);  // 0x00C6CEF0
 void UpdateFilters(const bool bNew);                                     // 0x00C6CF40
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char FableCSalRenderPanDelayRetailSize[sizeof(CSalRenderPanDelay)==0x214?1:-1];
#endif
#endif
