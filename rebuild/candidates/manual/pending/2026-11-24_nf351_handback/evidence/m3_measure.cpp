// NF351 measurement (item 3, NOT a candidate): 0x0045C5F6 as ??1CThingNoiseDef@@UAE@XZ written as a user destructor on the shared
// CThingBaseDef.h / CTCNoiseDef.h as they stand (CThingNoiseDef has no shared header: local model, Ego_r Noise at retail +0x70).
// Leaf: ??1CThingNoiseDef@@UAE@XZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCNoiseDef.h"
class CThingNoiseDef : public CThingBaseDef {
public:
    CTCNoiseDef Noise;                                                   // +0x70
    virtual ~CThingNoiseDef();
};
CThingNoiseDef::~CThingNoiseDef() { }
