#define FABLE_RETAIL_CTHING
// Name fix (agentNF6 2026-11-10): callee 0x004C7E90 is ?IsUnderScriptSupervision@CThing@@QBE_NXZ (Ego_r.pdb 0x004C9A40 / FableWin.pdb; engine/CThing.h opt-in).
#define FABLE_RETAIL_CTHING_SCRIPTSUPERVISION
#include "engine/CThing.h"
class CThingFilter_HasScriptSupervision { public: bool m_wanted; };


bool __fastcall CThingFilter_HasScriptSupervision__op(CThingFilter_HasScriptSupervision* filter, void* edx, CThing* thing)
{
    return thing->IsUnderScriptSupervision() == filter->m_wanted;
}