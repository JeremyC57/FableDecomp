// Behaviour test for 0x004AA8D0 (CScriptThing::GetDefName): forwards to the implementation's
// virtual when PImp_Data is set, else returns a copy of the static default.
#include <cstdio>

struct CCharString {
    int m_id;
    explicit CCharString(int id) : m_id(id) { }
    CCharString(const CCharString& other) : m_id(other.m_id) { }
};
CCharString g_EmptyString(0);

struct CScriptThingVirtuals {
    virtual void v0() { }
    virtual void v1() { }
    virtual CCharString GetDefName() const { return CCharString(42); }
    virtual ~CScriptThingVirtuals() { }
};

struct CScriptThing {                               // the handle: vptr, PImp_Data, PImp_Info
    void* __vftable;
    CScriptThingVirtuals* PImp_Data;
    void* PImp_Info;
    CCharString GetDefName() const;
};

CCharString CScriptThing::GetDefName() const
{
    const CScriptThingVirtuals* imp = PImp_Data;
    if (imp == 0)
        return g_EmptyString;
    return imp->GetDefName();
}

int main()
{
    CScriptThingVirtuals impl;
    CScriptThing thing;
    thing.__vftable = 0; thing.PImp_Info = 0;
    thing.PImp_Data = &impl;
    {
        CCharString r = thing.GetDefName();
        if (r.m_id != 42) { std::printf("004aa8d0_TEST FAIL\n"); return 1; }
    }
    thing.PImp_Data = 0;
    {
        CCharString r = thing.GetDefName();
        if (r.m_id != 0) { std::printf("004aa8d0_TEST FAIL\n"); return 1; }
    }
    std::printf("004aa8d0_TEST PASS\n");
    return 0;
}
