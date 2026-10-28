// CAIStateGroup_WanderAroundShops::InitCreaturePosition() -- during work hours and with the script-def chance (percent), pick a random shop; when one is found, place the creature next to it (beside the building for a building, else near its position, radius 5)
// Leaf: ?InitCreaturePosition@CAIStateGroup_WanderAroundShops
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009473E0, length 266 bytes (plain `ret`, virtual, bool).
// Callees: CAIStateGroup_WorkBase::IsInWorkHours @ 0x0091A6F0, GetPWorldSeed @ 0x008FCFD0 + GFROR13 @ 0x00497890 (the
//          landed CAIStateGroupBase::Random step), the shop picker @ 0x00946F90 (manifest
//          "CAIStateGroup_WanderAroundShops_SelectRandomEnemy"), CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//          0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CThing::GetPos 0x004C73D0,
//          CThingCreatureBase::AutoPlaceNearBuilding 0x00666EF0 / AutoPlaceNear 0x00665DF0; data: the chance 0x0138CE34.
// Layout: engine/CAIStateGroup_WanderAroundShops.h (Shop +0x14, PrettyThing +0x1c, NextFrameBuy +0x24) over the landed
//          CAIStateGroupBase model; the shop's +0x6c bit 6 marks a building.
// Shape: one `&&` chain whose last term assigns the picked shop through an IMPLICIT CThing* -> CIntelligentPointer
//          conversion temporary (`(Shop = SelectRandomShop()).GetPItem() != 0`): VC keeps the temporary's construction
//          flag in ebx (set after the converting ctor; an explicit functional cast sets it before the picker call) and the
//          condition in a frame byte; each placement arm reads the creature into a named local before its call.
class CThing;
class C3DVector { public: float x, y, z; };
class CThingBuilding;
class CThing { public:
    const C3DVector& GetPos() const;             // 0x004C73D0
};
class CThingPhysical : public CThing { public:
    unsigned char _pad[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 6: a building
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThingPhysical* GetPItem() const;           // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem((CThing*)other.GetPItem()); return *this; }
};
class CThingCreatureBase { public:
    bool AutoPlaceNearBuilding(CThingBuilding* building, float radius);   // 0x00666EF0
    bool AutoPlaceNear(const C3DVector& position, float radius);          // 0x00665DF0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern unsigned long g_WanderAroundShopsChance;          // 0x0138CE34 (percent)

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};
struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    CIntelligentPointerThing Shop;              // +0x14
    CIntelligentPointerThing PrettyThing;       // +0x1c
    long NextFrameBuy;                          // +0x24
    bool IsInWorkHours();                       // 0x0091A6F0
    CThing* SelectRandomShop();                 // 0x00946F90
    bool InitCreaturePosition();
};

bool CAIStateGroup_WanderAroundShops::InitCreaturePosition()
{
    bool ok = IsInWorkHours() && Random(100) < g_WanderAroundShopsChance
              && (Shop = SelectRandomShop()).GetPItem() != 0;
    if (ok) {
        if (Shop.GetPItem()->m_flags6c & 0x40) {
            CThingCreatureBase* creature = PBrain->PCreature;
            return creature->AutoPlaceNearBuilding((CThingBuilding*)Shop.GetPItem(), 5.0f);
        }
        CThingCreatureBase* creature = PBrain->PCreature;
        return creature->AutoPlaceNear(Shop.GetPItem()->GetPos(), 5.0f);
    }
    return false;
}