// CTCOwnedEntity::OnForceRecreateUID() -- re-captures the live owner's UID into OwnerUID
// Leaf: ?OnForceRecreateUID@CTCOwnedEntity@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E8370, length 56 bytes.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three times on POwner: null test, dead bit, then the read).
// Retail CTCOwnedEntity (PDB): POwner at the unaligned +0x0d (pack(1)), OwnerUID +0x18; the thing's UID at +0x18.
// Shape: the landed IS_THING_ALIVE idiom (two fetches, nested ifs) with &POwner held in edi, then a third fetch and the
// two-word copy.
#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

#pragma pack(push, 1)
struct CTCBase_007e8370 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCOwnedEntity : public CTCBase_007e8370 { public:
    bool                    SwitchableNavigationTCAdded;   // +0x0c
    CIntelligentPointer<CThing> POwner;                 // +0x0d CIntelligentPointer<CThing>
    unsigned char           _pad15[3];
    unsigned __int64        OwnerUID;               // +0x18
    void OnForceRecreateUID();
};
#pragma pack(pop)

void CTCOwnedEntity::OnForceRecreateUID()
{
    if (IS_THING_ALIVE(POwner))
        OwnerUID = POwner.Get()->GetUniqueID();
}