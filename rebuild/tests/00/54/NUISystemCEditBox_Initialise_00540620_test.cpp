// Behaviour test for 0x00540620 NUISystem::CEditBox::Initialise()
#include <cstdio>

struct CActionParam { int Tag; };
struct CAction { long ActionType; CActionParam* Params[4]; int ParamCount; CAction() : ActionType(0), ParamCount(0) { } };
struct CComponent {
    CAction* LeftClicked[4]; int LeftClickedCount;
    CComponent() : LeftClickedCount(0) { }
    void AddActionOnLeftClicked(CAction* a) { LeftClicked[LeftClickedCount++] = a; }
};
static int g_baseInits = 0;
struct CEditBox : CComponent {
    CActionParam ParamBase;
    bool ParentIsButton;
    CComponent* Parent;
    CAction* OnEnter[4]; int OnEnterCount;
    CEditBox() : ParentIsButton(false), Parent(0), OnEnterCount(0) { }
    void AddActionOnEnter(CAction* a) { OnEnter[OnEnterCount++] = a; }
    void Initialise()
    {
        ++g_baseInits;
        if (ParentIsButton && Parent) {
            CAction* enterAction = new CAction; enterAction->ActionType = 0xBCB;
            AddActionOnEnter(enterAction);
            CAction* clickAction = new CAction; clickAction->ActionType = 0xBCA;
            clickAction->Params[clickAction->ParamCount++] = &ParamBase;
            Parent->AddActionOnLeftClicked(enterAction);
            Parent->AddActionOnLeftClicked(clickAction);
        }
    }
};

int main()
{
    CComponent parent;
    CEditBox plain; plain.Initialise();
    if (g_baseInits != 1 || plain.OnEnterCount != 0) { std::printf("00540620_TEST FAIL\n"); return 1; }
    CEditBox orphan; orphan.ParentIsButton = true; orphan.Initialise();           // no parent: nothing wired
    if (orphan.OnEnterCount != 0) { std::printf("00540620_TEST FAIL\n"); return 1; }
    CEditBox box; box.ParentIsButton = true; box.Parent = &parent; box.Initialise();
    if (box.OnEnterCount != 1 || box.OnEnter[0]->ActionType != 0xBCB) { std::printf("00540620_TEST FAIL\n"); return 1; }
    if (parent.LeftClickedCount != 2 || parent.LeftClicked[0] != box.OnEnter[0] || parent.LeftClicked[1]->ActionType != 0xBCA) { std::printf("00540620_TEST FAIL\n"); return 1; }
    if (parent.LeftClicked[1]->ParamCount != 1 || parent.LeftClicked[1]->Params[0] != &box.ParamBase) { std::printf("00540620_TEST FAIL\n"); return 1; }
    delete parent.LeftClicked[0]; delete parent.LeftClicked[1];
    std::printf("00540620_TEST PASS\n");
    return 0;
}