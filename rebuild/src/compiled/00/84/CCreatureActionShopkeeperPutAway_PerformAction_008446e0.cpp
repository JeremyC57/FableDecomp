// CCreatureAction_ShopkeeperPutAway::PerformAction() -- if the item being put away is alive (pointer set, dead bit clear), kill it silently
// Leaf: ?PerformAction@CCreatureAction_ShopkeeperPutAway@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008446E0, length 64 bytes. Vtable slot 3 (PerformAction).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the pointer member at +0xa8, three times: the landed IS_THING_ALIVE idiom re-fetches,
// then the call target), CThing::Kill(bool) @ 0x004C9B80.
// Shape: the member pointer rides in esi (`lea esi,[ecx+0A8h]`); the shape of the landed 0x008445A0 without the range test. Retail
// CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so the pointer (PDB +0xb4) is at +0xa8 and `this` is modelled locally.
#include "engine/CThing.h"

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

class CCreatureAction_ShopkeeperPutAway { public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[0xa8 - 4];         // +0x04 (retail CCreatureActionBase body)
    CIntelligentPointer<CThing> PItem;          // +0xa8 CIntelligentPointer<CThing> (PDB +0xb4)
};

void CCreatureAction_ShopkeeperPutAway::PerformAction()
{
    if (IS_THING_ALIVE(PItem))
        PItem.Get()->Kill(true);
}