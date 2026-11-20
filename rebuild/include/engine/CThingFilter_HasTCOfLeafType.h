#ifndef FABLE_ENGINE_CTHINGFILTER_HASTCOFLEAFTYPE_H
#define FABLE_ENGINE_CTHINGFILTER_HASTCOFLEAFTYPE_H
// CThingFilter_HasTCOfLeafType<T> (chunk 849, row 0x00501450): Ego_r.pdb CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>, an
// empty class (sizeof 1) with the unary_function<CThing,bool> base (Codex pass 13 TPI read) and the const call operator on a
// non-const CThing& (??R?$CThingFilter_HasTCOfLeafType@VCTCActionUseScriptedHook@@@@QBE_NAAVCThing@@@Z). The operator is declared
// only: its only callers are the out-of-line GetAllThings instantiations (0x005198B0).
#include "engine/CThing.h"
template<class T> class CThingFilter_HasTCOfLeafType : public std::unary_function<CThing,bool> {
public:
#ifdef FABLE_RETAIL_CTHINGFILTER_HASTCOFLEAFTYPE_BODY
    // Nested opt-in (hand3c2, row 0x0064DD9A NPlayerGui::CDrawMiniMap::UpdateRegionExitNames): the call operator's body, visible in
    // the TU (Ego_d.pdb: test_thing, ptc_type; debug length 40, lines 488 / 490 / 492 = the call and test, `return true`, `return false`),
    // still out of line in retail (0x00518DC0,
    // landed). With it visible c2 knows the empty filter temporary passed to the same-TU GetAllThings body is never stored, and homes it
    // in the caller's argument byte (`lea eax,[ebp+0Bh]` at 0x0064DEC0) as retail does; declared only, it takes its own frame word.
    // Needs FABLE_RETAIL_CTHING_GETTCOFLEAFTYPE and T::StatGetClassTypeName.
    bool operator()(CThing& test_thing) const {
        T* ptc_type;
        if (test_thing.GetTCOfLeafType(&ptc_type))
            return true;
        return false;
    }
#else
    bool operator()(CThing& thing) const;
#endif
};
#endif
