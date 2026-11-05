// CTCKraken::EndHeadEffects() -- Kill the live effect and clear its intelligent pointer.
// Leaf: ?EndHeadEffects@CTCKraken@@QAEXXZ
// Identity: Own CreateHeadEffect007fbb00 and InitTentaclesAndHead007fc710; PDB EndHeadEffects and PHeadEffect+40.
#ifndef _MSC_VER
#define __forceinline inline
#endif
// Layout: CThing active header is absent (donor-only header quarantined). Retail Dead is bit0 at+0x91; no other fields accessed.
class CBaseObject;
#include "engine/CThing.h"
class CBaseObjectPointer;
// Layout: genuine polymorphic intelligent pointer has PData+4, total8bytes on x86; no active shared wrapper header.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

// Layout: no active owner header; PHeadEffect is an8-byte intelligent pointer at 40, proved by retail address and PDB member. Packed layout preserves the original unaligned member where applicable.
#pragma pack(push,1)
class CTCKraken {public:unsigned char Prefix[40];CIntelligentPointer<CThing> PHeadEffect;
public:void EndHeadEffects();friend struct TestAccess;
};
#pragma pack(pop)
void CTCKraken::EndHeadEffects(){if(PHeadEffect.Get() && !PHeadEffect.Get()->IsDead()){PHeadEffect.Get()->Kill(true);PHeadEffect=CIntelligentPointer<CThing>(0);}}