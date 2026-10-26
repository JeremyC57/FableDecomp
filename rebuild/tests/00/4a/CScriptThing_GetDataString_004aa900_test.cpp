// Behaviour test for 0x004AA900 (CScriptThing::GetDataString): forwards to the implementation's
// virtual when PImp_Data is set, else returns a copy of the static default.
#include <cstdio>

class CCharString { public:
    int m_id;
    explicit CCharString(int id) : m_id(id) { }
    CCharString(const CCharString& other) : m_id(other.m_id) { }
};
CCharString g_EmptyString(0);

struct CScriptThingVirtuals {
    virtual void v0() { }
    virtual void v1() { }
    virtual void v2() { }
    virtual CCharString GetDataString() const { return CCharString(42); }
    virtual ~CScriptThingVirtuals() { }
};

class CScriptThing { public:                               // the handle: vptr, PImp_Data, PImp_Info
    void* __vftable;
    CScriptThingVirtuals* PImp_Data;
    void* PImp_Info;
    CCharString GetDataString() const;
};

CCharString CScriptThing::GetDataString() const
{
    const CScriptThingVirtuals* imp = PImp_Data;
    if (imp == 0)
        return g_EmptyString;
    return imp->GetDataString();
}

int main()
{
    CScriptThingVirtuals impl;
    CScriptThing thing;
    thing.__vftable = 0; thing.PImp_Info = 0;
    thing.PImp_Data = &impl;
    {
        CCharString r = thing.GetDataString();
        if (r.m_id != 42) { std::printf("004aa900_TEST FAIL\n"); return 1; }
    }
    thing.PImp_Data = 0;
    {
        CCharString r = thing.GetDataString();
        if (r.m_id != 0) { std::printf("004aa900_TEST FAIL\n"); return 1; }
    }
    std::printf("004aa900_TEST PASS\n");
    return 0;
}