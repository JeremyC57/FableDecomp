// Behaviour test for NUISystem::CMenuEntry::MakeUnavailable() (0x0053E0C0): a model of the row -- an unavailable entry does
// nothing; an available one runs the base behaviour, makes the first text child found (type lookup, then the named
// MENU_ENTRY_TEXT child, then the non-recursive type lookup) unavailable, gives the PC_MENU_ENTRY_ARROW child a
// this-instance deletion and becomes unavailable.
#include <stdio.h>
#include <string.h>
struct Comp {
    const char* name; int type; bool unavailable; int deletion;
    Comp* children[4]; int nchildren; bool recursiveTextVisible;
    Comp(const char* n, int t) : name(n), type(t), unavailable(false), deletion(0), nchildren(0), recursiveTextVisible(true) { }
    Comp* FindFirstChildOfType(int t) { if (!recursiveTextVisible) return 0; for (int i = 0; i < nchildren; ++i) if (children[i]->type == t) return children[i]; return 0; }
    Comp* FindFirstChildOfTypeNonRecursive(int t) { for (int i = 0; i < nchildren; ++i) if (children[i]->type == t) return children[i]; return 0; }
    Comp* FindChildNamed(const char* n) { for (int i = 0; i < nchildren; ++i) if (strcmp(children[i]->name, n) == 0) return children[i]; return 0; }
};
struct MenuEntry : Comp {
    bool Available; int baseCalls;
    MenuEntry() : Comp("entry", 7), Available(true), baseCalls(0) { }
    void MakeUnavailable()
    {
        if (!Available)
            return;
        ++baseCalls;
        Comp* text = FindFirstChildOfType(6);
        if (text)
            text->unavailable = true;
        else {
            text = FindChildNamed("MENU_ENTRY_TEXT");
            if (text)
                text->unavailable = true;
            else {
                text = FindFirstChildOfTypeNonRecursive(6);
                if (text)
                    text->unavailable = true;
            }
        }
        Comp* arrow = FindChildNamed("PC_MENU_ENTRY_ARROW");
        if (arrow)
            arrow->deletion = 1;
        Available = false;
    }
};
int main()
{
    MenuEntry e; Comp text("label", 6); Comp arrow("PC_MENU_ENTRY_ARROW", 0);
    e.children[e.nchildren++] = &text; e.children[e.nchildren++] = &arrow;
    e.MakeUnavailable();
    int ok = !e.Available && e.baseCalls == 1 && text.unavailable && arrow.deletion == 1;
    text.unavailable = false; arrow.deletion = 0;
    e.MakeUnavailable();
    ok = ok && e.baseCalls == 1 && !text.unavailable && arrow.deletion == 0;
    MenuEntry f; Comp named("MENU_ENTRY_TEXT", 0); f.children[f.nchildren++] = &named; f.recursiveTextVisible = false;
    f.MakeUnavailable();
    ok = ok && named.unavailable && !f.Available;
    MenuEntry g; Comp t2("t", 6); g.children[g.nchildren++] = &t2; g.recursiveTextVisible = false;
    g.MakeUnavailable();
    ok = ok && t2.unavailable;
    printf(ok ? "0053e0c0_TEST PASS\n" : "0053e0c0_TEST FAIL\n");
    return ok ? 0 : 1;
}