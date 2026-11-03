#ifndef FABLE_ENGINE_CTHINGFILTER_HASTCOFLEAFTYPE_H
#define FABLE_ENGINE_CTHINGFILTER_HASTCOFLEAFTYPE_H
#include "rebuild_abi.h"
#include "engine/CThing.h"
// Original complete data members: both PDB receipts evidence/00501450_*_types.json.
template<class T> class CThingFilter_HasTCOfLeafType : public std::unary_function<CThing,bool> { public: bool operator()(CThing& thing) const; };
#endif
