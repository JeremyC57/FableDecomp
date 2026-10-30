// Behaviour test for CTCInventoryWeapons::ConstructWieldEntry (retail 0x005C323A): the returned entry carries the
// description (melee / ranged guide by the flag) and name children and one type-1 select action; the description is the
// viewport child.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
struct CAction { long ActionType; };
struct CComponent { std::string def, text; std::vector<CComponent*> children; std::vector<CAction*> actions;
    void SetText(const std::string& t) { text = t; } void AddChild(CComponent* c) { children.push_back(c); }
    void AddSelectedAction(CAction* a) { actions.push_back(a); } };
struct CViewport { CComponent* child; void SetViewportChild(CComponent* c) { child = c; } };
struct CList { int listed; void AddListedChild(CComponent*) { ++listed; } };
static CComponent* Create(const char* def) { CComponent* c = new CComponent(); c->def = def; return c; }
static std::string Text(const std::string& s) { return "<" + s + ">"; }
static CComponent* Construct(bool melee, CViewport* viewport, CList* list)
{
    CComponent* entry = Create("ENTRY"); entry->text = Text("TEXT_GUI_MENU_WIELD");
    CComponent* description = Create("DESCRIPTION");
    std::string guide = Text(melee ? "TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_WIELD" : "TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_WIELD");
    if (0) guide += "\n\nItem";
    description->SetText(guide);
    viewport->SetViewportChild(description);
    entry->AddChild(description);
    if (list) list->AddListedChild(description);
    CComponent* name = Create("NAME"); name->SetText(entry->text);
    entry->AddChild(name);
    if (list) list->AddListedChild(name);
    CAction* action = new CAction(); action->ActionType = 1;
    entry->AddSelectedAction(action);
    return entry;
}
int main()
{
    CViewport v = { 0 }; CList l = { 0 };
    CComponent* e = Construct(true, &v, 0 ? &l : 0);
    CComponent* r = Construct(false, &v, 0);
    bool ok = e->children.size() == 2 && v.child == r->children[0] && e->actions.size() == 1 && e->actions[0]->ActionType == 1
        && e->children[0]->text.find("MELEE") != std::string::npos && r->children[0]->text.find("RANGED") != std::string::npos
        && e->children[1]->text == "<TEXT_GUI_MENU_WIELD>" && l.listed == (0 ? 2 : 0);
    if (!ok) { std::printf("005c323a_TEST FAIL\n"); return 1; }
    std::printf("005c323a_TEST PASS\n");
    return 0;
}