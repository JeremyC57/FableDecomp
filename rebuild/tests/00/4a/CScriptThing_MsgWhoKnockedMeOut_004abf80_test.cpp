// Behaviour test for CScriptThing::MsgWhoKnockedMeOut (0x004ABF80): null handle returns a copy of itself, else the implementation's answer.
#include <cstdio>

class CCPPointerInfo { public: long m_references; };
class CScriptThing;
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
static CCPPointerInfo g_knockerInfo = { 1 };
static CScriptThing* g_knocker = 0;
class CScriptThing { public:
    CCountedPointer_CScriptThing PImp;
    CScriptThing(CScriptThing* d, CCPPointerInfo* i) : PImp(d, i) {}
    CScriptThing(const CScriptThing& other) : PImp(other.PImp) {}
    virtual ~CScriptThing() {}
    virtual CScriptThing MsgWhoKnockedMeOut() const
    {
        CScriptThing* implementation = PImp.Data;
        if (implementation == 0)
            return *this;
        return implementation->MsgWhoKnockedMeOut();
    }
};
struct CScriptThingImplementation : CScriptThing {
    CScriptThingImplementation() : CScriptThing(0, 0) {}
    virtual CScriptThing MsgWhoKnockedMeOut() const { return CScriptThing(g_knocker, &g_knockerInfo); }
};

int main()
{
    CScriptThing nullHandle(0, 0);
    CScriptThing r0 = nullHandle.MsgWhoKnockedMeOut();
    if (r0.PImp.Data != 0 || r0.PImp.Info != 0) { std::printf("004abf80_TEST FAIL\n"); return 1; }
    CScriptThingImplementation impl;
    CCPPointerInfo info = { 1 };
    CScriptThing handle(&impl, &info);
    CScriptThing knockerObj(0, 0);
    g_knocker = &knockerObj;
    CScriptThing r1 = handle.MsgWhoKnockedMeOut();
    if (r1.PImp.Data != &knockerObj || r1.PImp.Info != &g_knockerInfo) { std::printf("004abf80_TEST FAIL\n"); return 1; }
    std::printf("004abf80_TEST PASS\n");
    return 0;
}