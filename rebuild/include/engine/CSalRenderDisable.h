#ifndef FABLE_CSALRENDERDISABLE_H
#define FABLE_CSALRENDERDISABLE_H
// GameCODA plain-gain renderer, the "HRTF disabled" render path (NF452; constructor 0x00C6E760, destructor 0x00C6E780, vtable 0x012BEDB8, slots
// 0x00C6ECB0 / 0x00C6E690 (CSalRender::ClearFilterHistory, inherited) / 0x00C6E8F0 / 0x00C6EC50 / 0x00C6EC80). Class, base, member and method
// names, access and member order: the GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderDisable.obj CodeView (LF_CLASS CSalRenderDisable : public
// CSalRender; FilterPan / ParamsDisable from the same object). Offsets are retail's, never the SDK's: the retail CSalRender is 0x28 bytes
// (engine/CSalRender.h), so m_paramsA..m_pParamsNew sit at +0x28..+0x34 (SDK +0x2C..+0x38) and the class is 0x38 bytes: `push 38h` before
// `call 0x00C550B0` / `call 0x00C55110` in CSalSw3DVoice::CreateRenderer 0x00C55270, and the constructor aims +0x30 / +0x34 at +0x28 / +0x2C.
// ParamsDisable keeps the SDK's single FilterPan (4 bytes in retail too): one gain for every output.
// Process / SetGain / SetPosition use the CSalRender.h SDK slot signatures (retail pops 8 / 12 / 28 argument bytes).
// The private helpers Params3DInit (0x00C6EBA0) and UpdateFilters (0x00C6EBE0): SDK CodeView private records; the SDK Release object calls
// UpdateFilters from SetGain / SetPosition and expands Params3DInit into it, as retail 0x00C6EC6F / 0x00C6ECA7 -> 0x00C6EBE0; retail pops 4 / 4
// argument bytes and has the SDK's exact instruction counts (17 / 35).
// The SDK's private static mixers (MixFadeIn / MixFadeOut / Mix) are not declared: no landed row defines them under these names yet.
#if !defined(FABLE_RETAIL_CSALRENDER_MEMBERS) || !defined(FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23)
#error engine/CSalRenderDisable.h needs FABLE_RETAIL_CSALRENDER_MEMBERS and FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23 (retail CSalRender layout)
#endif
#include "engine/CSalRender.h"
#ifndef FABLE_GAMECODA_FILTERPAN_DEFINED
#define FABLE_GAMECODA_FILTERPAN_DEFINED
struct FilterPan {                    // SDK SalRenderDisable.obj / SalRenderPan.obj CodeView, 4 bytes (retail and SDK)
 unsigned short gain;                 // +0 Q15 gain
 short gainDelta;                     // +2 per-sample ramp step
};
#endif
struct ParamsDisable {
 FilterPan pan;                       // +0 (SDK: one FilterPan, 4 bytes)
};
class CSalRenderDisable:public CSalRender {
public:
 CSalRenderDisable();                                                     // 0x00C6E760
 virtual ~CSalRenderDisable();                                            // 0x00C6E780
 virtual void Process(const short*,int*(&)[4]);                           // slot 2, 0x00C6E8F0
 virtual void SetGain(float,const float*,bool);                           // slot 3, 0x00C6EC50 (also the SIMD classes' slot 3)
 virtual void SetPosition(float,float,float,float,const float*,bool,bool); // slot 4, 0x00C6EC80 (also the SIMD classes' slot 4)
private:
 void Params3DInit(FilterPan* pFilter);                                   // 0x00C6EBA0
 void UpdateFilters(const bool bNew);                                     // 0x00C6EBE0
#ifdef FABLE_RETAIL_CSALRENDERDISABLE_MIXFADE
 // Opt-in (NF463, queue nf452): the SDK private static mixers MixFadeIn / MixFadeOut, GameCODA 2.10 CODAApi.lib Release/SalRenderDisable.obj
 // (CodeView private static records; retail object order ctor 0x00C6E760, dtor 0x00C6E780, MixFadeIn 0x00C6E790, MixFadeOut 0x00C6E820, Mix 0x00C6E8B0, Process 0x00C6E8F0 = the SDK object order; instruction counts 53 / 54 with a
 // plain ret, as retail 0x00C6E790 / 0x00C6E820). __cdecl static: the callers in Process pop with `add esp`. Declarations only: no layout change.
 static void MixFadeIn(const short*,const FilterPan*,int*);                     // 0x00C6E790
 static void MixFadeOut(const short*,const FilterPan*,int*);                    // 0x00C6E820
#endif
protected:
 ParamsDisable m_paramsA;             // +0x28
 ParamsDisable m_paramsB;             // +0x2C
 ParamsDisable* m_pParamsCurrent;     // +0x30
 ParamsDisable* m_pParamsNew;         // +0x34
};
#if defined(_MSC_VER) && defined(_M_IX86)
typedef char FableCSalRenderDisableRetailSize[sizeof(CSalRenderDisable)==0x38?1:-1];
#endif
#endif
