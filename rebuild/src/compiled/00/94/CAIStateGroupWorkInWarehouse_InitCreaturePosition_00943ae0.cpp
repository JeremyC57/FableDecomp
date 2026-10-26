// CAIStateGroup_WorkInWarehouse::InitCreaturePosition() -- drop the worker somewhere within 30m of the crate's warehouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00943AE0, length 91 bytes (+ int3 padding).
// Callees: virtual slot 11 (+0x2c, IsRunnable) on this, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 twice (PCrate at +0x1c, then the crate's owner pointer at +0x10 through the inline
// accessor), CAIStateGroupBase::GetPWorld @ 0x008FCFA0 (the second copy of the PBrain -> +0x1c ->
// +0x24 getter), CWorld::GetPRandomSeed @ 0x0049D810 (`[world+8] + 1613Ch`), GFFloatRandom
// (float, unsigned long*) @ 0x004978A0 (__fastcall: the float on the stack, the seed in ecx),
// CThingPhysical::GetPosition @ 0x004C73D0, CThingCreatureBase::AutoPlaceNear(const C3DVector&,
// float) @ 0x00665DF0 whose byte result is returned as is.
// Shape: nested if with the `xor al,al` false tail at the end (an early return would reuse the
// virtual's zero al inline); the warehouse (edi) and the creature (ebx, a named
// local read before GetPWorld) are fetched first, then the arguments right to left: the random
// radius is computed into the reserved slot (`push ecx ; fstp [esp]`), then the position.
class C3DVector;

class CThing { public:
    unsigned char _pad[0x10];
    unsigned char PWarehouse[8];                // +0x10 CIntelligentPointer<CThing>
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

struct CThingCrate : CThing {
    CThing* GetPWarehouse() { return reinterpret_cast<CBaseIntelligentPointer*>(PWarehouse)->GetPItem(); }
};

class CThingAICreature { public:
    bool AutoPlaceNear(const C3DVector& position, float radius);    // 0x00665DF0
};

class CGame { public:
    unsigned char _pad[0x1613c];
    unsigned long m_randomSeed;                 // +0x1613c
};

class CWorld { public:
    unsigned char _pad[8];
    CGame* m_pGame;                             // +0x08
    unsigned long* GetPRandomSeed();            // 0x0049D810
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld_008fcfa0();                        // 0x008FCFA0
};

struct CAIStateGroup_WorkInWarehouse : CAIStateGroupBase {
    unsigned char PWorkBuilding[8];             // +0x14 CIntelligentPointer<CThing>
    CBaseIntelligentPointer PCrate;             // +0x1c CIntelligentPointer<CThing>
    virtual bool InitCreaturePosition();
};

bool CAIStateGroup_WorkInWarehouse::InitCreaturePosition()
{
    if (IsRunnable()) {
        CThing* warehouse = static_cast<CThingCrate*>(PCrate.GetPItem())->GetPWarehouse();
        CThingAICreature* creature = PBrain->Creature;
        return creature->AutoPlaceNear(warehouse->GetPosition(), GFFloatRandom(30.0f, GetPWorld_008fcfa0()->GetPRandomSeed()));
    }
    return false;
}