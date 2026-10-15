// Behaviour test for 0x00BC68B0 (see src/00bc68b0.cpp): the two-case text CreateInternalPrimitive.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00bc68b0_TEST FAIL\n"); return 1; } } while (0)

static int g_strings = 0;
struct CEngineInternalPrimitiveBase { unsigned int Type; unsigned int Flags; CEngineInternalPrimitiveBase(unsigned int t, unsigned int f) : Type(t), Flags(f) { } virtual ~CEngineInternalPrimitiveBase() { } virtual int Kind() { return 0; } };
struct CWideString { void* d; CWideString() : d(0) { ++g_strings; } };
struct CEngineInternalPrimitiveText : CEngineInternalPrimitiveBase {
    float Pos[3]; float Scale; unsigned int Colour; bool ScaleIn3D; CWideString Text; void* Font;
    CEngineInternalPrimitiveText(unsigned int t, unsigned int f) : CEngineInternalPrimitiveBase(t, f) { }
    virtual int Kind() { return 1; }
};
struct CEngineInternalPrimitive2DText : CEngineInternalPrimitiveText {
    int Layer; CEngineInternalPrimitive2DText() : CEngineInternalPrimitiveText(0x27, 0x80000000u) { } virtual int Kind() { return 2; }
};
struct CEnginePrimitiveManagerText {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(int type)
    {
        switch (type) {
        case 5: return new CEngineInternalPrimitiveText(5, 0x2000000);
        case 0x27: return new CEngineInternalPrimitive2DText;
        }
        return 0;
    }
};
int main()
{
    CEnginePrimitiveManagerText m;
    CEngineInternalPrimitiveBase* t = m.CreateInternalPrimitive(5);
    CHECK(t && t->Type == 5 && t->Flags == 0x2000000u && t->Kind() == 1 && g_strings == 1);
    CEngineInternalPrimitiveBase* t2 = m.CreateInternalPrimitive(0x27);
    CHECK(t2 && t2->Type == 0x27 && t2->Flags == 0x80000000u && t2->Kind() == 2 && g_strings == 2);
    CHECK(m.CreateInternalPrimitive(6) == 0 && m.CreateInternalPrimitive(0) == 0 && g_strings == 2);
    delete t; delete t2;
    std::printf("00bc68b0_TEST PASS\n");
    return 0;
}