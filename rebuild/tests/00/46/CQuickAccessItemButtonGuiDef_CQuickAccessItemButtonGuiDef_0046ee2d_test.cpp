// Behaviour test for CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&) @ 0x0046EE2D
#include <cstdio>

static int g_baseCopies = 0;

class C2DVector { public: float X, Y; };

class CParentDefClassBase { public:
    virtual ~CParentDefClassBase() { }
    long m_baseValue;
    CParentDefClassBase() : m_baseValue(0) { }
    CParentDefClassBase(const CParentDefClassBase& other) : m_baseValue(other.m_baseValue) { ++g_baseCopies; }
};

class CQuickAccessItemButtonGuiDef : public CParentDefClassBase { public:
    long      ButtonGraphic;
    C2DVector Position;
    C2DVector Offset;
    CQuickAccessItemButtonGuiDef() { }
    CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& other)
        : CParentDefClassBase(other), ButtonGraphic(other.ButtonGraphic), Position(other.Position), Offset(other.Offset) { }
};

int main()
{
    CQuickAccessItemButtonGuiDef a;
    a.m_baseValue = 9; a.ButtonGraphic = 1234;
    a.Position.X = 1.5f; a.Position.Y = -2.0f; a.Offset.X = 3.0f; a.Offset.Y = 4.25f;
    CQuickAccessItemButtonGuiDef b(a);
    if (g_baseCopies != 1 || b.m_baseValue != 9 || b.ButtonGraphic != 1234) { std::printf("0046ee2d_TEST FAIL\n"); return 1; }
    if (b.Position.X != 1.5f || b.Position.Y != -2.0f || b.Offset.X != 3.0f || b.Offset.Y != 4.25f) { std::printf("0046ee2d_TEST FAIL\n"); return 1; }
    std::printf("0046ee2d_TEST PASS\n");
    return 0;
}