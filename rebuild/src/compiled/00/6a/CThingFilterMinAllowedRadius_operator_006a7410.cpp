// Byte-exact reconstruction of CThingFilter_MinAllowedRadius::operator()
// Leaf: ??RCThingFilter_MinAllowedRadius@@QAE_NPAVCThing@@@Z
// retail 0x006a7410  bool __fastcall (self /*ecx*/, CThing* thing /*[esp+4]*/)

struct CTCPhysicsBase {
    char pad[0x34];
    float radius;      // +0x34
};

#include "engine/CThing.h"

class CThingFilter_MinAllowedRadius { public:
    float minAllowedRadius; // +0x00
    bool operator()(CThing* thing);
};

// Real member -> genuine thiscall: this in ecx, thing on stack at [esp+4].
bool CThingFilter_MinAllowedRadius::operator()(CThing* thing)
{
    return thing->GetPhysicsTC()->radius > this->minAllowedRadius;
}