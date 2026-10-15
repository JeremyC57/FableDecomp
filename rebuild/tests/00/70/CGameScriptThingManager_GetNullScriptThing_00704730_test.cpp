// Behaviour test for CGameScriptThingManager::GetNullScriptThing (0x00704730): returns a counted copy of NullThing.
#include <cstdio>

struct CCPPointerInfo { long m_references; };
struct CScriptThing;
struct CCountedPointer_CScriptThing {
    CScriptThing* Data;
    CCPPointerInfo* Info;
    CCountedPointer_CScriptThing(CScriptThing* d, CCPPointerInfo* i) : Data(d), Info(i) {}
    CCountedPointer_CScriptThing(const CCountedPointer_CScriptThing& other)
        : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
};
struct CScriptThing {
    CCountedPointer_CScriptThing PImp;
    CScriptThing(CScriptThing* d, CCPPointerInfo* i) : PImp(d, i) {}
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}
    virtual ~CScriptThing() {}
};
struct CGameScriptThingManager {
    void* ScriptThings;
    CScriptThing NullThing;
    CGameScriptThingManager(CScriptThing* d, CCPPointerInfo* i) : ScriptThings(0), NullThing(d, i) {}
    CScriptThing GetNullScriptThing() { return NullThing; }
};

int main()
{
    CCPPointerInfo info = { 1 };
    CScriptThing impl(0, 0);
    CGameScriptThingManager manager(&impl, &info);
    CScriptThing a = manager.GetNullScriptThing();
    if (a.PImp.Data != &impl || a.PImp.Info != &info || info.m_references != 2) { std::printf("00704730_TEST FAIL\n"); return 1; }
    CGameScriptThingManager empty(0, 0);
    CScriptThing b = empty.GetNullScriptThing();
    if (b.PImp.Data != 0 || b.PImp.Info != 0) { std::printf("00704730_TEST FAIL\n"); return 1; }
    std::printf("00704730_TEST PASS\n");
    return 0;
}