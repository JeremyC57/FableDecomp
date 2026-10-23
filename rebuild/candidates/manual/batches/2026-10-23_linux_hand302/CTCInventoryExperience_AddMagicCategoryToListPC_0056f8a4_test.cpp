// Behaviour test for CTCInventoryExperience::AddMagicCategoryToListPC @ 0x0056F8A4 (builds one magic-category branch entry)
#include <cstdio>
#include <cstring>

static const bool kPC = true;
struct Comp { char Name[48]; const Comp* Children[8]; int ChildCount; char Text[48]; int Sounds; };
static Comp g_pool[16]; static int g_used;
static Comp* Create(const char* name) { Comp* c = &g_pool[g_used++]; std::strcpy(c->Name, name); c->ChildCount = 0; c->Text[0] = 0; c->Sounds = 0; return c; }
static void AddChild(Comp* parent, const Comp* child) { parent->Children[parent->ChildCount++] = child; }
static int g_registered, g_viewportChildren, g_listItems;
static void AddCategory(bool hasViewport, Comp* subList, const char* definition, const char* icon, const char* description)
{
    Comp* entry = Create(kPC ? "PC_MENU_ENTRY_BRANCH_TEMPLATE" : "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW");
    ++entry->Sounds;
    Comp* def = Create(definition);
    AddChild(entry, subList); AddChild(entry, def);
    if (kPC) ++g_registered;
    Comp* desc = Create(kPC ? "PC_EXPSPEND_CAT_DESC" : "UI_EXPSPEND_CAT_DESC");
    std::strcpy(desc->Text, icon);
    AddChild(entry, desc);
    if (hasViewport) ++g_viewportChildren;
    if (kPC) ++g_registered;
    Comp* sub = Create(kPC ? "PC_SUBTITLE" : "UI_ONE_OFF_SUBTITLE");
    std::strcpy(sub->Text, description);
    AddChild(entry, sub);
    if (kPC) ++g_registered;
    ++g_listItems;
}
int main()
{
    Comp sub; std::strcpy(sub.Name, "SUB"); sub.ChildCount = 0;
    AddCategory(true, &sub, "DEF", "ICON", "DESC");
    AddCategory(false, &sub, "DEF2", "ICON2", "DESC2");
    const Comp& e = g_pool[0];
    if (e.ChildCount != 4 || e.Children[0] != &sub || std::strcmp(e.Children[1]->Name, "DEF") != 0 || std::strcmp(e.Children[2]->Text, "ICON") != 0 ||
        std::strcmp(e.Children[3]->Text, "DESC") != 0 || e.Sounds != 1 || g_viewportChildren != 1 || g_listItems != 2 || g_registered != (kPC ? 6 : 0))
    { std::printf("0056f8a4_TEST FAIL\n"); return 1; }
    std::printf("0056f8a4_TEST PASS\n");
    return 0;
}
