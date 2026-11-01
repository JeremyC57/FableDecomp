// Behaviour test for 0x0083D6D0 CThingPhysicalSwitch::~CThingPhysicalSwitch: the script-name string is destroyed, then the
// CThingGameObject base destructor runs (member before base).
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CCharString {
public:
    int id;
    CCharString() : id(1) {}
    ~CCharString() { g_order[g_count++] = id; }
};
class CThingGameObject {
public:
    CThingGameObject() {}
    virtual ~CThingGameObject() { g_order[g_count++] = 2; }
    unsigned char _pad[0xd0 - sizeof(void*)];
};
class CThingPhysicalSwitch : public CThingGameObject {
public:
    CThingPhysicalSwitch() : Triggered(false) {}
    virtual ~CThingPhysicalSwitch();
    bool        Triggered;
    CCharString TriggerableObjectScriptName;
};
CThingPhysicalSwitch::~CThingPhysicalSwitch()
{
}
int main()
{
    CThingGameObject* p = new CThingPhysicalSwitch;
    delete p;                                    // virtual: the derived destructor runs first
    if (g_count != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("0083d6d0_TEST FAIL\n"); return 1; }
    std::printf("0083d6d0_TEST PASS\n");
    return 0;
}