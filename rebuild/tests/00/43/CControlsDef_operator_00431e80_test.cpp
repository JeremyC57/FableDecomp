// Behaviour test for CControlsDef::operator= (0x00431E80): the base part, the control list and all six toggles are copied and
// the result is the left operand.
#include <cstdio>
class CActionInputControl { public: int action; int key; };
class ControlList {
public:
    ControlList() : count(0), assigns(0) {}
    ControlList& operator=(const ControlList& x) { count = x.count; for (int i = 0; i < count; ++i) items[i] = x.items[i]; ++assigns; return *this; }
    CActionInputControl items[4]; int count; int assigns;
};
class CParentDefClassBase {
public:
    CParentDefClassBase() : globalIndex(0), baseAssigns(0) {}
    virtual ~CParentDefClassBase() {}
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { globalIndex = o.globalIndex; ++baseAssigns; return *this; }
    long globalIndex; int baseAssigns;
};
class CControlsDef : public CParentDefClassBase {
public:
    ControlList Controls;
    bool ToggleZTarget, ToggleSpells, ToggleSneak, ToggleExpressionMenu, ToggleExpressionShift, FlourishNeedsAttackButtonHeld;
};
int main()
{
    CControlsDef a, b;
    a.globalIndex = 7; a.Controls.count = 2; a.Controls.items[0].action = 3; a.Controls.items[1].key = 9;
    a.ToggleZTarget = true; a.ToggleSpells = false; a.ToggleSneak = true;
    a.ToggleExpressionMenu = false; a.ToggleExpressionShift = true; a.FlourishNeedsAttackButtonHeld = true;
    b.ToggleZTarget = false; b.ToggleSpells = true; b.ToggleSneak = false;
    b.ToggleExpressionMenu = true; b.ToggleExpressionShift = false; b.FlourishNeedsAttackButtonHeld = false;
    CControlsDef& r = (b = a);
    int bad = 0;
    if (&r != &b || b.globalIndex != 7 || b.baseAssigns != 1 || b.Controls.assigns != 1) bad = 1;
    if (b.Controls.count != 2 || b.Controls.items[0].action != 3 || b.Controls.items[1].key != 9) bad = 1;
    if (!b.ToggleZTarget || b.ToggleSpells || !b.ToggleSneak || b.ToggleExpressionMenu || !b.ToggleExpressionShift || !b.FlourishNeedsAttackButtonHeld) bad = 1;
    if (bad) { std::printf("00431e80_TEST FAIL\n"); return 1; }
    std::printf("00431e80_TEST PASS\n");
    return 0;
}