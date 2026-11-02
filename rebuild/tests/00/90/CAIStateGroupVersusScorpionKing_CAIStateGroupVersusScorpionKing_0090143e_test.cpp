// Behaviour test for CAIStateGroup_VersusScorpionKing::CAIStateGroup_VersusScorpionKing (0x0090143E): the base constructor runs first, then this class's vtable is live and
// the tracked pointer member PArenaCentreThing is constructed (empty) after it; on the 32-bit build the 1-aligned pointer
// sits at 0x1D and the class is 0x28 bytes (the factory's `new` size).
#include <cstdio>
#include <cstddef>
static int g_log[16]; static int g_n = 0;
class CBaseClass { public: CBaseClass() { g_log[g_n++] = 1; } virtual ~CBaseClass() { } };
class CBaseObjectPointer;
class CThing; class CThingBuilding;
#pragma pack(push, 1)
class CBaseIntelligentPointer { public:
    CBaseIntelligentPointer() : PData(0) { g_log[g_n++] = 3; }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
#pragma pack(pop)
class CAIStateGroupBase : public CBaseClass { public:
    void* PBrain; int ScriptAIPriority; void* Name; bool Terminated; unsigned char Priority; bool Enabled;
    CAIStateGroupBase() { g_log[g_n++] = 2; }
    virtual int Kind() const { return 0; }
};
class CThingCreatureBase;
class CAIStateGroup_VersusEnemyBase : public CAIStateGroupBase { public:
    CIntelligentPointer<CThingCreatureBase> PTargetThing;
    CAIStateGroup_VersusEnemyBase() { g_log[g_n++] = 4; }
    virtual int Kind() const { return 1; }
};
typedef CAIStateGroup_VersusEnemyBase Base_;
class CAIStateGroup_VersusScorpionKing : public Base_ { public:
    bool GoClockwise;
    CIntelligentPointer<CThing> PArenaCentreThing;
    CAIStateGroup_VersusScorpionKing() { }
    virtual int Kind() const { return 7; }
};
int main()
{
    CAIStateGroup_VersusScorpionKing* group = new CAIStateGroup_VersusScorpionKing;
    if (group->Kind() != 7 || group->PArenaCentreThing.PData != 0 || g_log[0] != 1 || g_log[1] != 2 || g_log[g_n - 1] != 3) {
        std::printf("0090143e_TEST FAIL\n"); return 1;
    }
    if (sizeof(void*) == 4 && (offsetof(CAIStateGroup_VersusScorpionKing, PArenaCentreThing) != 0x1D || sizeof(CAIStateGroup_VersusScorpionKing) != 0x28)) {
        std::printf("0090143e_TEST FAIL\n"); return 1;
    }
    delete group;
    std::printf("0090143e_TEST PASS\n");
    return 0;
}