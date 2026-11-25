#ifndef FABLE_CSALRENDERPAN_H
#define FABLE_CSALRENDERPAN_H
// GameCODA volume panner (NF452; constructor 0x00C6D9D0, destructor 0x00C6DA30, vtable 0x012BED90, slots 0x00C6E1D0 / 0x00C6E690
// (CSalRender::ClearFilterHistory, inherited) / 0x00C6DBA0 / 0x00C6E020 / 0x00C6E050). Class, base, member and method names, access and the
// member order: the GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderPan.obj CodeView (LF_CLASS CSalRenderPan : public CSalRender; ParamsPan;
// FilterPan as in engine/CSalRenderDisable.h). Offsets and array bounds are retail's, never the SDK's: the retail CSalRender is 0x28 bytes
// (engine/CSalRender.h), and retail keeps 6 entries where SDK 2.10 keeps 4 (ParamsPan::pan, m_fGainPan): `push 78h` before `call 0x00C55090`
// / `call 0x00C550F0` in CSalSw3DVoice::CreateRenderer 0x00C55270 (SDK 0x64), and the constructor clears +0x28..+0x3F and +0x40..+0x57
// (6 dwords each), aims +0x58 / +0x5C at them and zeroes +0x60..+0x77 -- the SDK Release constructor's exact shape with 4 entries
// (+0x28 / +0x38, pointers +0x48 / +0x4C, gains +0x50..+0x5C).
// Process / SetGain / SetPosition use the CSalRender.h SDK slot signatures (retail pops 8 / 12 / 28 argument bytes); retail indexes the
// outputs 0..3 only (outputs 2 / 3 behind the 4-speaker test), so `int*(&)[4]` is the retail type too.
// The private helpers Params3DInit (0x00C6DE60) and UpdateFilters (0x00C6DEA0): SDK CodeView private records; the SDK Release object calls
// UpdateFilters from SetGain / SetPosition and Params3DInit from UpdateFilters, as retail 0x00C6E020 / 0x00C6E050 -> 0x00C6DEA0 -> 0x00C6DE60;
// retail pops 8 / 4 argument bytes and has the SDK's exact instruction counts (17 / 111).
// The SDK's private static mixers (MixFadeIn / MixFadeOut / Mix) are not declared: their landed rows keep their own spellings
// (0x00C6DA40 / 0x00C6DAD0) until they are ported with a body tie.
#if !defined(FABLE_RETAIL_CSALRENDER_MEMBERS) || !defined(FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23)
#error engine/CSalRenderPan.h needs FABLE_RETAIL_CSALRENDER_MEMBERS and FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23 (retail CSalRender layout)
#endif
#include "engine/CSalRender.h"
#ifndef FABLE_GAMECODA_FILTERPAN_DEFINED
#define FABLE_GAMECODA_FILTERPAN_DEFINED
struct FilterPan {                    // SDK SalRenderDisable.obj / SalRenderPan.obj CodeView, 4 bytes (retail and SDK)
 unsigned short gain;                 // +0 Q15 gain
 short gainDelta;                     // +2 per-sample ramp step
};
#endif
struct ParamsPan {
 FilterPan pan[6];                    // retail 6 entries (0x18); SDK 2.10 has 4
};
class CSalRenderPan:public CSalRender {
public:
 CSalRenderPan();                                                         // 0x00C6D9D0
 virtual ~CSalRenderPan();                                                // 0x00C6DA30
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6DBA0
 virtual void SetGain(float,const float*,bool);                           // slot 3, 0x00C6E020 (also the SIMD classes' slot 3)
 virtual void SetPosition(float,float,float,float,const float*,bool,bool); // slot 4, 0x00C6E050 (also the SIMD classes' slot 4)
private:
 void Params3DInit(FilterPan* pFilter,float fGainPan);                    // 0x00C6DE60
 void UpdateFilters(const bool bNew);                                     // 0x00C6DEA0
protected:
 ParamsPan m_paramsA;                 // +0x28
 ParamsPan m_paramsB;                 // +0x40
 ParamsPan* m_pParamsCurrent;         // +0x58
 ParamsPan* m_pParamsNew;             // +0x5C
private:
 float m_fGainPan[6];                 // +0x60
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char FableCSalRenderPanRetailSize[sizeof(CSalRenderPan)==0x78?1:-1];
#endif
#endif
