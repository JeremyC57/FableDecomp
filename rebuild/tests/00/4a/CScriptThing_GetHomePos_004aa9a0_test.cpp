// Behaviour test for 0x004AA9A0 (CScriptThing::GetHomePos): forwards to the implementation's
// virtual when PImp_Data is set, else returns a copy of the static default.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
C3DVector g_ZeroVector(0.0f, 0.0f, 0.0f);

struct CScriptThingVirtuals {
    virtual void v0() { }
    virtual void v1() { }
    virtual void v2() { }
    virtual void v3() { }
    virtual void v4() { }
    virtual void v5() { }
    virtual void v6() { }
    virtual C3DVector GetHomePos() const { return C3DVector(1.0f, 2.0f, 3.0f); }
    virtual ~CScriptThingVirtuals() { }
};

struct CScriptThing {                               // the handle: vptr, PImp_Data, PImp_Info
    void* __vftable;
    CScriptThingVirtuals* PImp_Data;
    void* PImp_Info;
    C3DVector GetHomePos() const;
};

C3DVector CScriptThing::GetHomePos() const
{
    const CScriptThingVirtuals* imp = PImp_Data;
    if (imp == 0)
        return g_ZeroVector;
    return imp->GetHomePos();
}

int main()
{
    CScriptThingVirtuals impl;
    CScriptThing thing;
    thing.__vftable = 0; thing.PImp_Info = 0;
    thing.PImp_Data = &impl;
    {
        C3DVector r = thing.GetHomePos();
        if (r.X != 1.0f || r.Y != 2.0f || r.Z != 3.0f) { std::printf("004aa9a0_TEST FAIL\n"); return 1; }
    }
    thing.PImp_Data = 0;
    {
        C3DVector r = thing.GetHomePos();
        if (r.X != 0.0f || r.Y != 0.0f || r.Z != 0.0f) { std::printf("004aa9a0_TEST FAIL\n"); return 1; }
    }
    std::printf("004aa9a0_TEST PASS\n");
    return 0;
}