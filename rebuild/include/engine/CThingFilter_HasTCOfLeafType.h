#ifndef FABLE_ENGINE_CTHINGFILTER_HASTCOFLEAFTYPE_H
#define FABLE_ENGINE_CTHINGFILTER_HASTCOFLEAFTYPE_H
// CThingFilter_HasTCOfLeafType<T> (chunk 849, row 0x00501450): Ego_r.pdb CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>, an
// empty class (sizeof 1) with the unary_function<CThing,bool> base (Codex pass 13 TPI read) and the const call operator on a
// non-const CThing& (??R?$CThingFilter_HasTCOfLeafType@VCTCActionUseScriptedHook@@@@QBE_NAAVCThing@@@Z). The operator is declared
// only: its only callers are the out-of-line GetAllThings instantiations (0x005198B0).
#include "engine/CThing.h"
template<class T> class CThingFilter_HasTCOfLeafType : public std::unary_function<CThing,bool> {
public:
    bool operator()(CThing& thing) const;
};
#endif
