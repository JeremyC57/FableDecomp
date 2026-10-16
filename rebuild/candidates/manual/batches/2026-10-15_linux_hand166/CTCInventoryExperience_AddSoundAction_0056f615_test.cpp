// Behaviour test for 0x0056F615 CTCInventoryExperience::AddSoundAction
#include <cstdio>
#include <cstring>
struct CActionParamString { char text[32]; CActionParamString(const char* t) { std::strcpy(text, t); } };
struct CAction {
    long ActionType; CActionParamString* params[4]; int count;
    CAction() : ActionType(0), count(0) {}
    void AddUIActionParam(CActionParamString* p) { params[count++] = p; }
};
struct CGUIElement {
    CAction* last;
    CGUIElement() : last(0) {}
    void AddAction(CAction* a) { last = a; }
};
static void AddSoundAction(CGUIElement* element, const char* soundName)
{
    CAction* action = new CAction;
    action->ActionType = 0xBBB;
    action->AddUIActionParam(new CActionParamString(soundName));
    element->AddAction(action);
}
int main()
{
    CGUIElement e;
    AddSoundAction(&e, "SND_CLICK");
    bool ok = e.last && e.last->ActionType == 3003 && e.last->count == 1 && std::strcmp(e.last->params[0]->text, "SND_CLICK") == 0;
    if (!ok) { std::printf("0056f615_TEST FAIL\n"); return 1; }
    std::printf("0056f615_TEST PASS\n");
    return 0;
}
