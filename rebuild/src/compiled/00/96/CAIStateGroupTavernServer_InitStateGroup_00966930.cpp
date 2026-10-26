// CAIStateGroup_TavernServer::InitStateGroup(const CWorld&, CThingAICreature&) -- cache the creature's walk speed and clear the jug/bar/table/pub/tankard pointers and the serving, bar and action point indices
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00966930, length 352 bytes (`ret 8`, an `and esp,-8` aligned frame).
// Callees: the creature's virtual slot 79 (+0x13c, a float speed for a direction vector), CBaseIntelligentPointer
//          ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (temporaries: CIntelligentPointer<
//          CThingObject> vtable 0x0125A858 held in edi, CIntelligentPointer<CThingBuilding> vtable 0x01244344).
// Shape: a function-scope zero C3DVector (its own slot) passed by reference; each pointer reset is `P = CIntelligentPointer<T>(0)` through
//          one 8-byte frame temporary (the aligned frame comes from the 8-byte temporaries); the three indices take one
//          `or eax,-1`.
// Layout: engine/CAIStateGroup_TavernServer.h (PJug +0x14, PBar +0x1c, PTable +0x24, PTankard +0x2c, PPub +0x34,
//          ServingActionPointIndex +0x40, BarActionPointIndex +0x68, ActionPointIndex +0x90, AverageWalkSpeed +0x98).
// Leaf: InitStateGroup
#include "engine/CAIStateGroup_TavernServer.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
};
class CThing;
class CWorld;
class CThingAICreature;
#define V16(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); \
    virtual void n##4(); virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); \
    virtual void n##9(); virtual void n##a(); virtual void n##b(); virtual void n##c(); virtual void n##d(); \
    virtual void n##e(); virtual void n##f();
struct CThingCreatureSpeed {                    // the brain's creature
    V16(a) V16(b) V16(c) V16(d)
    virtual void e0(); virtual void e1(); virtual void e2(); virtual void e3(); virtual void e4();
    virtual void e5(); virtual void e6(); virtual void e7(); virtual void e8(); virtual void e9();
    virtual void ea(); virtual void eb(); virtual void ec(); virtual void ed(); virtual void ee();
    virtual float GetWalkSpeed(const C3DVector& direction);   // slot 79 (+0x13c, descriptive)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    __forceinline CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerObject& operator=(const CIntelligentPointerObject& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {   // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    __forceinline CIntelligentPointerBuilding(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureSpeed* PCreature;             // +0x18
};

struct CAIStateGroupTavernServerMethods : CAIStateGroup_TavernServer {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CIntelligentPointerObject& Jug() { return *(CIntelligentPointerObject*)PJug; }
    CIntelligentPointerObject& Bar() { return *(CIntelligentPointerObject*)PBar; }
    CIntelligentPointerObject& Table() { return *(CIntelligentPointerObject*)PTable; }
    CIntelligentPointerObject& Tankard() { return *(CIntelligentPointerObject*)PTankard; }
    CIntelligentPointerBuilding& Pub() { return *(CIntelligentPointerBuilding*)PPub; }
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroupTavernServerMethods::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    C3DVector direction(0.0f, 0.0f, 0.0f);
    AverageWalkSpeed = Brain()->PCreature->GetWalkSpeed(direction);
    Jug() = CIntelligentPointerObject(0);
    Bar() = CIntelligentPointerObject(0);
    Table() = CIntelligentPointerObject(0);
    Pub() = CIntelligentPointerBuilding(0);
    Tankard() = CIntelligentPointerObject(0);
    ServingActionPointIndex = -1;
    BarActionPointIndex = -1;
    ActionPointIndex = -1;
}