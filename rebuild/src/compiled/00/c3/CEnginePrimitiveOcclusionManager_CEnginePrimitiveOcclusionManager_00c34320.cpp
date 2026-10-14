// CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager() -- the base, a cleared generation at +0x10, two float ranges (1..10 and 0..0) at +0x14 / +0x1c built out of line, cleared words at +0x24 / +0x28, four 1.0f factors at +0x2c..+0x38, two cleared shorts at +0x3c / +0x3e
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C34320, length 76 bytes.
// Callees: the base constructor @ 0x00C20390 (the manifest label "CNavigationLayer" is propagated), the range constructor @ 0x00C1BD20 twice (two floats: the
// label "CDateAndTime" is propagated; it stores the pair). Shape: no vtable store (`__declspec(novtable)`), 1.0f lives in ebx (pushed for the first range and stored four times), the zero register in edi
// (the second range's two arguments, the words and the shorts); the +0x10 store is scheduled between the first range's `lea ecx` and its call.
struct COcclusionBase_00c20390 {
    virtual void v00();
    unsigned char _pad[0xc];                       // +0x04 .. +0x10
    COcclusionBase_00c20390();                     // 0x00C20390
};

struct SFloatRange_00c1bd20 {                      // 8 bytes
    float m_min;
    float m_max;
    SFloatRange_00c1bd20(float min, float max);    // 0x00C1BD20
};

struct __declspec(novtable) CEnginePrimitiveOcclusionManager : COcclusionBase_00c20390 {   // no vtable store of its own in retail
    long                 m_generation;             // +0x10
    SFloatRange_00c1bd20 m_distanceRange;          // +0x14
    SFloatRange_00c1bd20 m_fadeRange;              // +0x1c
    long                 m_a;                      // +0x24
    long                 m_b;                      // +0x28
    float                m_factor0;                // +0x2c
    float                m_factor1;                // +0x30
    float                m_factor2;                // +0x34
    float                m_factor3;                // +0x38
    short                m_countA;                 // +0x3c
    short                m_countB;                 // +0x3e
    CEnginePrimitiveOcclusionManager();
};

CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager()
    : COcclusionBase_00c20390()
    , m_generation(0)
    , m_distanceRange(1.0f, 10.0f)
    , m_fadeRange(0.0f, 0.0f)
    , m_a(0), m_b(0)
    , m_factor0(1.0f), m_factor1(1.0f), m_factor2(1.0f), m_factor3(1.0f)
    , m_countA(0), m_countB(0)
{
}