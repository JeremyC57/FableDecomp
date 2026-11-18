#ifndef FABLE_ENGINE_CREACTIONFREQUENCYTRAITSARRAY_H
#define FABLE_ENGINE_CREACTIONFREQUENCYTRAITSARRAY_H
#ifdef FABLE_RETAIL_REACTIONFREQUENCYTRAITS_CLASS_CHAIN_PASS24
#include "engine/CReactionFrequencyTraits.h"
#include "engine/CPersistTraits.h"
class CReactionFrequencyTraitsArray {
public:
    CReactionFrequencyTraits* LowHigh[2][79];
};
template<> class CPersistTraits<CReactionFrequencyTraitsArray> {
public:
    CReactionFrequencyTraitsArray DefaultVal;
    void TransferIn(CPersistContext& persist_context, CStringParser& parser, CReactionFrequencyTraitsArray& val);
};
FABLE_STATIC_ASSERT(sizeof(CReactionFrequencyTraitsArray) == 0x278);
#endif
#endif
