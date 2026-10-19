#include <cstdio>
struct C2DVector { float X, Y; };
struct CActionInputControl {
    long GameAction, ControllerType, KeyboardKey, XboxButton, MouseButton;
    C2DVector ControlDirection;
    CActionInputControl() {}
    CActionInputControl(const CActionInputControl& o)
        : GameAction(o.GameAction), ControllerType(o.ControllerType), KeyboardKey(o.KeyboardKey),
          XboxButton(o.XboxButton), MouseButton(o.MouseButton), ControlDirection(o.ControlDirection) { }
};
static long g_coexist = 0, g_lastA = -1, g_lastB = -1;
bool AreAllowedToCoexist(long a, long b) { g_lastA = a; g_lastB = b; return g_coexist != 0; }
struct CActionList {
    struct node { node* next; node* prev; long value; };
    node* head;
    long front() const { return head->next->value; }
};
static int g_redefined = 0, g_updated = 0;
static CActionInputControl g_got;
static bool g_notify = true;
struct CKeyRedefiner {
    CActionList Actions;
    long SubType;
    CActionInputControl Definition;
    void Redefine(CActionInputControl* c, bool notify) { ++g_redefined; g_got = *c; g_notify = notify; }
    void UpdateKeyText() { ++g_updated; }
    void ClearDuplicateDefinitions(const CActionInputControl& other);
};
void CKeyRedefiner::ClearDuplicateDefinitions(const CActionInputControl& other)
{
    if (AreAllowedToCoexist(other.GameAction, Actions.front()))
        return;
    if (Definition.ControllerType != other.ControllerType)
        return;
    switch (Definition.ControllerType) {
    case 1: if (Definition.XboxButton != other.XboxButton) return; break;
    case 2: if (Definition.KeyboardKey != other.KeyboardKey) return; break;
    case 3: if (Definition.MouseButton != other.MouseButton) return; break;
    }
    CActionInputControl cleared = Definition;
    cleared.ControllerType = 2;
    cleared.KeyboardKey = 0;
    Redefine(&cleared, false);
    UpdateKeyText();
}
static int fail() { std::printf("005580b0_TEST FAIL\n"); return 1; }
int main()
{
    CActionList::node sentinel, n1;
    sentinel.next = &n1; n1.value = 42;
    CKeyRedefiner k; k.Actions.head = &sentinel;
    k.Definition.GameAction = 7; k.Definition.ControllerType = 2; k.Definition.KeyboardKey = 30;
    k.Definition.XboxButton = 5; k.Definition.MouseButton = 3; k.Definition.ControlDirection.X = 1.5f; k.Definition.ControlDirection.Y = -2.0f;
    CActionInputControl o = k.Definition; o.GameAction = 9;
    g_coexist = 1; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 0 || g_lastA != 9 || g_lastB != 42) return fail();
    g_coexist = 0; o.KeyboardKey = 31; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 0) return fail();
    o.KeyboardKey = 30; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 1 || g_updated != 1 || g_notify) return fail();
    if (g_got.GameAction != 7 || g_got.ControllerType != 2 || g_got.KeyboardKey != 0 || g_got.XboxButton != 5 || g_got.MouseButton != 3 || g_got.ControlDirection.Y != -2.0f) return fail();
    k.Definition.ControllerType = 1; o.ControllerType = 1; o.XboxButton = 6; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 1) return fail();
    o.XboxButton = 5; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 2) return fail();
    k.Definition.ControllerType = 3; o.ControllerType = 1; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 2) return fail();
    o.ControllerType = 3; o.MouseButton = 4; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 2) return fail();
    k.Definition.ControllerType = 0; o.ControllerType = 0; k.ClearDuplicateDefinitions(o);
    if (g_redefined != 3) return fail();
    std::printf("005580b0_TEST PASS\n");
    return 0;
}
