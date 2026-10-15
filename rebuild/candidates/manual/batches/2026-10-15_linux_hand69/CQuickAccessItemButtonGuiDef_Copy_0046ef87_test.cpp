// Behaviour test for CQuickAccessItemButtonGuiDef::Copy(const CDefClassBase*) @ 0x0046EF87
#include <cstdio>

static int g_baseCopies = 0;

struct C2DVector { float X, Y; };

struct CDefClassBase {
    virtual ~CDefClassBase() { }
};

struct CParentDefClassBase : CDefClassBase {
    long m_baseValue;
    virtual void Copy(const CDefClassBase* other)
    {
        m_baseValue = static_cast<const CParentDefClassBase*>(other)->m_baseValue;
        ++g_baseCopies;
    }
};

struct CQuickAccessItemButtonGuiDef : CParentDefClassBase {
    long      ButtonGraphic;
    C2DVector Position;
    C2DVector Offset;
    virtual void Copy(const CDefClassBase* other)
    {
        CParentDefClassBase::Copy(other);
        const CQuickAccessItemButtonGuiDef* source = static_cast<const CQuickAccessItemButtonGuiDef*>(other);
        ButtonGraphic = source->ButtonGraphic;
        Position = source->Position;
        Offset = source->Offset;
    }
};

int main()
{
    CQuickAccessItemButtonGuiDef a, b;
    a.m_baseValue = 5; a.ButtonGraphic = 88;
    a.Position.X = 10.0f; a.Position.Y = 20.0f; a.Offset.X = -1.0f; a.Offset.Y = 0.5f;
    b.m_baseValue = 0; b.ButtonGraphic = 0;
    CDefClassBase* target = &b;
    static_cast<CParentDefClassBase*>(target)->Copy(&a);
    if (g_baseCopies != 1 || b.m_baseValue != 5 || b.ButtonGraphic != 88) { std::printf("0046ef87_TEST FAIL\n"); return 1; }
    if (b.Position.X != 10.0f || b.Position.Y != 20.0f || b.Offset.X != -1.0f || b.Offset.Y != 0.5f) { std::printf("0046ef87_TEST FAIL\n"); return 1; }
    std::printf("0046ef87_TEST PASS\n");
    return 0;
}
