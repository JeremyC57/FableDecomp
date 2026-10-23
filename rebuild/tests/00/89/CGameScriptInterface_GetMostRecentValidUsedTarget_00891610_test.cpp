#include <cstdio>
#include <cstring>
// Behavior test for CGameScriptInterface::GetMostRecentValidUsedTarget @ 0x00891610 (self-contained copy of the source).
class CBaseObject;
class CScriptThing;
class CThing;

namespace NHeroInformationScreens
{
class CBase
{
public:
    CBase();
    virtual ~CBase();
};
}

struct CCountedPointerWords
{
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};

class CScriptThing : public NHeroInformationScreens::CBase
{
public:
    CScriptThing() { }
    virtual ~CScriptThing();

public:                                            // (test copy: public for inspection)
    CCountedPointerWords PImp;
};

class CBaseObject
{
public:
    CScriptThing BuildScriptThing();
};

class CBaseIntelligentPointer
{
public:
    CBaseObject* GetPItem();
};

class CVillageTaskCarryCrate
{
public:
    void AssignCreatureToTask(CThing* thing);

private:
    unsigned char m_Unused[0xA8];

public:
    CBaseIntelligentPointer m_MostRecentTarget;
};

struct CGSIGetMostRecentValidUsedTarget_BaseObject
{
    unsigned char m_Unused[0x91];
    unsigned char m_Flags;
};

class CGameScriptInterface
{
public:
    virtual CScriptThing
        GetMostRecentValidUsedTarget() const;
};

extern CVillageTaskCarryCrate*
    CGSIGetMostRecentValidUsedTarget_Manager;

CScriptThing
CGameScriptInterface::GetMostRecentValidUsedTarget() const
{
    CBaseObject* const item =
        CGSIGetMostRecentValidUsedTarget_Manager
            ->m_MostRecentTarget.GetPItem();
    if (item != 0 &&
        (reinterpret_cast<
            const CGSIGetMostRecentValidUsedTarget_BaseObject*>(
                item)->m_Flags & 1) == 0)
    {
        CGSIGetMostRecentValidUsedTarget_Manager
            ->AssignCreatureToTask(0);
        return item->BuildScriptThing();
    }

    return CScriptThing();
}


CVillageTaskCarryCrate* CGSIGetMostRecentValidUsedTarget_Manager;
static CVillageTaskCarryCrate* g_ManagerAfterGet;
static CBaseObject* g_Item;
static CBaseIntelligentPointer* g_ObservedIntelligentPointer;
static CVillageTaskCarryCrate* g_ObservedAssignManager;
static CThing* g_ObservedAssignedThing;
static CBaseObject* g_ObservedBuildObject;
static void* g_ObservedBaseThis;
static unsigned long g_GetPItemCallCount, g_AssignCallCount, g_BuildCallCount, g_BaseConstructorCallCount;
static int g_Valid;

NHeroInformationScreens::CBase::CBase() { g_ObservedBaseThis = this; ++g_BaseConstructorCallCount; }
NHeroInformationScreens::CBase::~CBase() { }
CScriptThing::~CScriptThing() { }

CBaseObject* CBaseIntelligentPointer::GetPItem()
{
    g_ObservedIntelligentPointer = this;
    ++g_GetPItemCallCount;
    if (g_ManagerAfterGet != 0)
        CGSIGetMostRecentValidUsedTarget_Manager = g_ManagerAfterGet;
    return g_Item;
}
void CVillageTaskCarryCrate::AssignCreatureToTask(CThing* thing)
{
    g_ObservedAssignManager = this;
    g_ObservedAssignedThing = thing;
    ++g_AssignCallCount;
}
CScriptThing CBaseObject::BuildScriptThing()
{
    g_ObservedBuildObject = this;
    ++g_BuildCallCount;
    CScriptThing result;
    result.PImp.Data = &g_Valid;
    result.PImp.Info = &g_Valid + 1;
    return result;
}
static void Reset()
{
    g_ManagerAfterGet = 0; g_ObservedIntelligentPointer = 0; g_ObservedAssignManager = 0;
    g_ObservedAssignedThing = reinterpret_cast<CThing*>(1); g_ObservedBuildObject = 0; g_ObservedBaseThis = 0;
    g_GetPItemCallCount = g_AssignCallCount = g_BuildCallCount = g_BaseConstructorCallCount = 0;
}

int main()
{
    static CVillageTaskCarryCrate firstManager, secondManager;
    static unsigned char itemBytes[0x100];
    CBaseObject* item = reinterpret_cast<CBaseObject*>(itemBytes);
    CGameScriptInterface interfaceObject;

    Reset(); g_Item = 0; CGSIGetMostRecentValidUsedTarget_Manager = &firstManager;
    {
        CScriptThing r = interfaceObject.GetMostRecentValidUsedTarget();
        if (r.PImp.Data != 0 || r.PImp.Info != 0 || g_ObservedIntelligentPointer != &firstManager.m_MostRecentTarget ||
            g_GetPItemCallCount != 1 || g_AssignCallCount != 0 || g_BuildCallCount != 0 || g_BaseConstructorCallCount != 1)
            { std::printf("FSE2_00891610_TEST FAIL 1\n"); return 1; }
    }
    Reset(); itemBytes[0x91] = 1; g_Item = item; CGSIGetMostRecentValidUsedTarget_Manager = &firstManager;
    {
        CScriptThing r = interfaceObject.GetMostRecentValidUsedTarget();
        if (r.PImp.Data != 0 || r.PImp.Info != 0 || g_AssignCallCount != 0 || g_BuildCallCount != 0 || g_BaseConstructorCallCount != 1)
            { std::printf("FSE2_00891610_TEST FAIL 2\n"); return 2; }
    }
    Reset(); itemBytes[0x91] = 0; g_Item = item; g_ManagerAfterGet = &secondManager; CGSIGetMostRecentValidUsedTarget_Manager = &firstManager;
    {
        CScriptThing r = interfaceObject.GetMostRecentValidUsedTarget();
        if (r.PImp.Data != &g_Valid || r.PImp.Info != &g_Valid + 1 || g_ObservedAssignManager != &secondManager ||
            g_ObservedAssignedThing != 0 || g_ObservedBuildObject != item || g_GetPItemCallCount != 1 ||
            g_AssignCallCount != 1 || g_BuildCallCount != 1)
            { std::printf("FSE2_00891610_TEST FAIL 3\n"); return 3; }
    }
    std::printf("FSE2_00891610_TEST PASS\n");
    return 0;
}
