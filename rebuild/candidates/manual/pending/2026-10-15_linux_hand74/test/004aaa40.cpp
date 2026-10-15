// Behaviour test for CScriptThing::GetAngleXY (0x004AAA40): null implementation -> 0.0f, else forward.
#include <cstdio>

struct CScriptThingImplementation {
    float m_angle;
    virtual float GetAngleXY() const { return m_angle; }
};

struct CScriptThing {
    void* __vftable;
    CScriptThingImplementation* PImp_Data;
    void* PImp_Info;
    float GetAngleXY() const
    {
        const CScriptThingImplementation* implementation = PImp_Data;
        if (implementation == 0)
            return 0.0f;
        float angle = implementation->GetAngleXY();
        return angle;
    }
};

int main()
{
    CScriptThing handle;
    handle.__vftable = 0;
    handle.PImp_Data = 0;
    handle.PImp_Info = 0;
    if (handle.GetAngleXY() != 0.0f) { std::printf("004aaa40_TEST FAIL\n"); return 1; }
    CScriptThingImplementation impl;
    impl.m_angle = 1.25f;
    handle.PImp_Data = &impl;
    if (handle.GetAngleXY() != 1.25f) { std::printf("004aaa40_TEST FAIL\n"); return 1; }
    std::printf("004aaa40_TEST PASS\n");
    return 0;
}
