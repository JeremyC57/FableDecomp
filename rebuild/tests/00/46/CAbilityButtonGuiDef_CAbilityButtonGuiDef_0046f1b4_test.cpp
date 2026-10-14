#include <cstdio>

static int g_baseCopies = 0;
struct CParentDefClassBase {
    virtual void v00() { }
    unsigned char _body[0x38];
    CParentDefClassBase() { }
    CParentDefClassBase(const CParentDefClassBase& other) { ++g_baseCopies; }
};
struct CAbilityButtonGuiDef : CParentDefClassBase {
    long m_buttonX, m_buttonY, m_buttonWidth, m_buttonHeight, m_iconX, m_iconY, m_iconWidth, m_iconHeight;
    virtual void v00() { }
    CAbilityButtonGuiDef() : m_buttonX(1), m_buttonY(2), m_buttonWidth(3), m_buttonHeight(4), m_iconX(5), m_iconY(6), m_iconWidth(7), m_iconHeight(8) { }
    CAbilityButtonGuiDef(const CAbilityButtonGuiDef& o) : CParentDefClassBase(o), m_buttonX(o.m_buttonX), m_buttonY(o.m_buttonY), m_buttonWidth(o.m_buttonWidth), m_buttonHeight(o.m_buttonHeight), m_iconX(o.m_iconX), m_iconY(o.m_iconY), m_iconWidth(o.m_iconWidth), m_iconHeight(o.m_iconHeight) { }
};
int main()
{
    CAbilityButtonGuiDef a; CAbilityButtonGuiDef b(a);
    if (g_baseCopies != 1 || b.m_buttonX != 1 || b.m_buttonY != 2 || b.m_buttonWidth != 3 || b.m_buttonHeight != 4 || b.m_iconX != 5 || b.m_iconY != 6 || b.m_iconWidth != 7 || b.m_iconHeight != 8) { std::printf("0046f1b4_TEST FAIL\n"); return 1; }
    std::printf("0046f1b4_TEST PASS\n"); return 0;
}