#ifndef FABLE_ENGINE_CTCDRIVERBASE_H
#define FABLE_ENGINE_CTCDRIVERBASE_H
#include "engine/CTCBase.h"
// Original Ego_r0x19b45; no added storage. Native CTCDExplosion base offsets agree.
class CTCDriverBase : public CTCBase {
#ifdef FABLE_ONHIT_TEST_ACCESS
public: CTCDriverBase(CThing&);
#endif
};
FABLE_STATIC_ASSERT(sizeof(CTCDriverBase)==0xc);
#endif
