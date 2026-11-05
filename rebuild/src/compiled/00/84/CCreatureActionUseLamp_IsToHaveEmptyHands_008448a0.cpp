// CCreatureAction_UseLamp::IsToHaveEmptyHands() -- false only while the creature is alive and already holds the lamp (bit 20 of the flags dword at +0x34), true otherwise
// Leaf: ?IsToHaveEmptyHands@CCreatureAction_UseLamp@@QAEEXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008448A0, length 64 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times: the landed IS_THING_ALIVE idiom re-fetches, then the flags read).
// Shape: PCreature rides in esi; the result is an `unsigned char` (retail `xor al,al` / `mov al,1` with no `xor eax,eax`), so the
// bool-returning virtual is modelled with that return type. Interface type 0xa0 = bit 20 of the bitset dword at +0x34 (the lamp component).
#include "engine/CThing.h"
struct CThingBits {                         // the interface bitset at +0x20..+0x43 read as its sixth dword
    unsigned char _pad[0x34];
    unsigned long m_presentBits5;           // +0x34 (interface types 0xa0..0xbf)
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CCreatureAction_UseLamp { public:
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char IsToHaveEmptyHands();
};

unsigned char CCreatureAction_UseLamp::IsToHaveEmptyHands()
{
    if (IS_THING_ALIVE(PCreature)) {
        if (((CThingBits*)PCreature.Get())->m_presentBits5 & 0x100000)
            return 0;
    }
    return 1;
}