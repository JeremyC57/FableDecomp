// Behaviour test for 0x0094BF60 CAIStateGroup_SellToBuyer::RefuseCustom: the copied logic over a model whose message
// manager records the posted message.
#include <cstdio>

class CThing { public: int Id; };

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CMessageEvent { public: int Unused; };
class CMessageEventManager { public:
    int Count;
    long Type; unsigned long Delay; float PosX, PosY, PosZ; CThing* From; CThing* To; float Value1, Value2; long Data;
    CMessageEventManager() : Count(0) { }
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                              float value1, float value2, long data)
    {
        ++Count; Type = type; Delay = delay; PosX = position.X; PosY = position.Y; PosZ = position.Z;
        From = from; To = to; Value1 = value1; Value2 = value2; Data = data;
        return 0;
    }
};
class CWorld { public: CMessageEventManager* MessageEventManager; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
class CAIBrain { public: CThing* PCreature; };
class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    CWorld* World;                              // what GetPWorld reaches through the brain (test model)
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld() { return World; }
};
class CAIStateGroup_SellToBuyer : public CAIStateGroupBase {
public:
    CBaseIntelligentPointer PBuyer;
    void RefuseCustom();
};

void CAIStateGroup_SellToBuyer::RefuseCustom()
{
    CThing* creature = GetCreature();
    C3DVector position(0.0f, 0.0f, 0.0f);
    CMessageEventManager* messages = GetPWorld()->MessageEventManager;
    messages->AddMessage(0x10, 1000, position, creature, PBuyer.GetPItem(), 0.0f, 0.0f, 0);
}

int main()
{
    CThing seller, buyer;
    CMessageEventManager manager;
    CWorld world; world.MessageEventManager = &manager;
    CAIBrain brain; brain.PCreature = &seller;
    CAIStateGroup_SellToBuyer group;
    group.PBrain = &brain; group.World = &world; group.PBuyer.m_pItem = &buyer;
    group.RefuseCustom();
    if (manager.Count != 1 || manager.Type != 0x10 || manager.Delay != 1000 || manager.From != &seller ||
        manager.To != &buyer || manager.PosX != 0.0f || manager.PosY != 0.0f || manager.PosZ != 0.0f ||
        manager.Value1 != 0.0f || manager.Value2 != 0.0f || manager.Data != 0) {
        std::printf("0094bf60_TEST FAIL\n");
        return 1;
    }
    std::printf("0094bf60_TEST PASS\n");
    return 0;
}