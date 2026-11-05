// CAIStateGroup_WanderAroundShops::IsStillValid() const -- valid in work hours while the shop and the pretty thing are alive
// Leaf: ?IsStillValid@CAIStateGroup_WanderAroundShops@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946F30, length 86 bytes (+ int3 padding).
// Callees: CAIStateGroup_WorkBase::IsInWorkHours @ 0x0091A6F0 (on this), then
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 four times (Shop at +0x14, PrettyThing at +0x1c,
// engine header CAIStateGroup_WanderAroundShops.h): the inlined IS_THING_ALIVE predicate (null test,
// then the dead bit at +0x91 & 1), the landed 0x00836BE0 shape.
// Shape: `IsInWorkHours() && IsAlive(Shop) && IsAlive(PrettyThing)` -- the int-typed `&&` chain
// materialises `mov eax,1` / shared `xor eax,eax` (full register); each pointer's address is formed
// once into esi for both GetPItem calls. Base modelled locally (flattened header).
#include "engine/CThing.h"

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem();                         // 0x00A01B50
    __forceinline bool IsAlive()
    {
        if (GetPItem() != 0) {
            if (!GetPItem()->IsDead())
                return true;
        }
        return false;
    }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInWorkHours();                       // 0x0091A6F0
};

struct CAIStateGroup_WanderAroundShops : CAIStateGroup_WorkBase {
    CBaseIntelligentPointer Shop;               // +0x14 CIntelligentPointer<CThing>
    CBaseIntelligentPointer PrettyThing;        // +0x1c CIntelligentPointer<CThing>
    long NextFrameBuy;                          // +0x24
    bool IsStillValid() const;
};

bool CAIStateGroup_WanderAroundShops::IsStillValid() const
{
    CAIStateGroup_WanderAroundShops* self = const_cast<CAIStateGroup_WanderAroundShops*>(this);
    return self->IsInWorkHours() && self->Shop.IsAlive() && self->PrettyThing.IsAlive();
}