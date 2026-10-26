// CAIStateGroup_EatAtTable::Eat(CThing*) -- pick the food up from the table, eat in a loop for a random 0..6 seconds, then put the tankard down; false as soon as an action does not succeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096C9A0, length 384 bytes (`ret 4`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("ST_SAT_EATING_INTO" @ 0x01296DB4,
//          "ST_SAT_EATING_LOOP" @ 0x01296EF8), CCreatureAction_PickUpThingAtTable ctor @ 0x008440C0 (the TU's derived
//          action, vtable 0x01296C9C, has an inline ctor), AIAction_PerformActionAndWait @ 0x0097DB70, ~CBaseIntelligentPointer
//          @ 0x00A01C10, the action base dtor @ 0x00693EF0, GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, GFGetCurrentFrame
//          @ 0x0049D870, CCreatureAction_PlayAnimation ctor @ 0x008424F0, CCreatureAction_PutDownTankard ctor @ 0x0096C820.
// Globals: frames per second @ 0x01375550 (int).
// Shape: the pick-up action is the TU's derived class whose inline ctor passes the INTO name as an implicit-conversion
//          temporary (dies before the derived vtable store); each action is a Perform-argument temporary so `!= 2` lands
//          in bl before the destructors. The frame rate is a named local read first (ebx); Random spells the seed update
//          through *seed (`return *seed % range`), which keeps the div ahead of the GFGetCurrentFrame call.
// Layout: engine/CAIStateGroupBase.h (PBrain +4). The pick-up action's CIntelligentPointer member sits at +0xA8.
#include "engine/CAIStateGroupBase.h"

class CThing;
class CThingCreatureBase;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                             // 0x00693EF0
};
enum EPickUpMode { PICK_UP_MODE_28 = 0x28 };
class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:
    CBaseIntelligentPointer PThing;                     // +0xa8
    CCreatureAction_PickUpThingAtTable(CThingCreatureBase* creature, CThing* thing, const CCharString& anim,
        EPickUpMode mode);                              // 0x008440C0
};
struct CCreatureAction_PickUpFoodAtTable : CCreatureAction_PickUpThingAtTable {   // vtable 0x01296C9C (descriptive name)
    CCreatureAction_PickUpFoodAtTable(CThingCreatureBase* creature, CThing* food)
        : CCreatureAction_PickUpThingAtTable(creature, food, "ST_SAT_EATING_INTO", PICK_UP_MODE_28) { }
    virtual void v00();
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
namespace N_0096c820 { struct CCreatureAction_PutDownTankard : CCreatureAction_PlayAnimation {
    CCreatureAction_PutDownTankard(CThingCreatureBase* creature);           // 0x0096C820
}; } using N_0096c820::CCreatureAction_PutDownTankard;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                      // +0x18
};
long __fastcall GFROR13(unsigned long);  // 0x00497890
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                    // 0x0049D870
extern long g_FramesPerSecond;                          // 0x01375550

struct CAIStateGroupEatAtTableMethods : CAIStateGroupBase {
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();                     // 0x008FCFD0
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool stopWhenReplaced);   // 0x0097DB70
    bool Eat(CThing* food);
};

bool CAIStateGroupEatAtTableMethods::Eat(CThing* food)
{
    if (AIAction_PerformActionAndWait(&CCreatureAction_PickUpFoodAtTable(GetCreature(), food), false) != 2)
        return false;
    long fps = g_FramesPerSecond;
    long endFrame = CWorld::GetFrame() + Random(7) * fps;
    do {
        if (AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreature(), "ST_SAT_EATING_LOOP",
                CSoundPair(), false, true, false, 0, 0), false) != 2)
            return false;
    } while (endFrame > CWorld::GetFrame());
    if (AIAction_PerformActionAndWait(&CCreatureAction_PutDownTankard(GetCreature()), false) != 2)
        return false;
    return true;
}