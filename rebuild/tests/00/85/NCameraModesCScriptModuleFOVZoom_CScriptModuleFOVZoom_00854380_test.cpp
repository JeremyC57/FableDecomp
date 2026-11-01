// Behaviour test for CScriptModuleFOVZoom::~CScriptModuleFOVZoom (0x00854380): the implicit derived destructor runs the base body
// (POwner and PCameraManager cleared) before the two intelligent-pointer members are destroyed, PThingOrigin first.
#include <cstdio>
static int g_order[4];
static int g_count = 0;
static void* g_ownerAtFirstMemberDtor = (void*)1;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer(int id, void** owner) : Id(id), Owner(owner) { }
    virtual ~CBaseIntelligentPointer()
    {
        if (g_count == 0) g_ownerAtFirstMemberDtor = *Owner;
        g_order[g_count++] = Id;
    }
    int Id;
    void** Owner;
};
class CScriptModuleBase {
public:
    CScriptModuleBase() : PCamPoint(1, &POwner), PThingOrigin(2, &POwner), POwner((void*)this), PCameraManager((void*)this) { }
    virtual ~CScriptModuleBase()
    {
        POwner = 0;
        PCameraManager = 0;
    }
    CBaseIntelligentPointer PCamPoint;
    CBaseIntelligentPointer PThingOrigin;
    void* POwner;
    void* PCameraManager;
};
class CScriptModuleFOVZoom : public CScriptModuleBase {
public:
    float Extra[3];
};
int main()
{
    {
        CScriptModuleFOVZoom module;
        module.Extra[0] = 1.0f;
    }
    if (g_count != 2 || g_order[0] != 2 || g_order[1] != 1 || g_ownerAtFirstMemberDtor != 0) {
        std::printf("00854380_TEST FAIL\n");
        return 1;
    }
    std::printf("00854380_TEST PASS\n");
    return 0;
}