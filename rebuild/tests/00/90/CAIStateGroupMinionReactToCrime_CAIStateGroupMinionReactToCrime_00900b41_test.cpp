// Behaviour test for CAIStateGroup_MinionReactToCrime::CAIStateGroup_MinionReactToCrime (0x00900B41): the base constructor runs first, then this class's vtable is live and
// the tracked pointer member ForceOpenBuilding is constructed (empty) after it; on the 32-bit build the 1-aligned pointer
// sits at 0x1B and the class is 0x24 bytes (the factory's `new` size).
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
class Base_ : public CAIStateGroupBase { };
class CAIStateGroup_MinionReactToCrime : public Base_ { public:
    long OpinionDeedHandle; bool GuardTold; bool AllowDuplicateReactToCrime; bool DeedMaskApplied;
    CIntelligentPointer<CThingBuilding> ForceOpenBuilding;
    CAIStateGroup_MinionReactToCrime() { }
    virtual int Kind() const { return 7; }
};
int main()
{
    CAIStateGroup_MinionReactToCrime* group = new CAIStateGroup_MinionReactToCrime;
    if (group->Kind() != 7 || group->ForceOpenBuilding.PData != 0 || g_log[0] != 1 || g_log[1] != 2 || g_log[g_n - 1] != 3) {
        std::printf("00900b41_TEST FAIL\n"); return 1;
    }
    if (sizeof(void*) == 4 && (offsetof(CAIStateGroup_MinionReactToCrime, ForceOpenBuilding) != 0x1B || sizeof(CAIStateGroup_MinionReactToCrime) != 0x24)) {
        std::printf("00900b41_TEST FAIL\n"); return 1;
    }
    delete group;
    std::printf("00900b41_TEST PASS\n");
    return 0;
}