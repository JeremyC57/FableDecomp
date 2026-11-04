// Behaviour test for CTCActionUseBase::OnInitialActivate (0x007EAC70): a model of the row -- the base slot always runs; a
// CTCEntityEvents component is added (inactive, no params) only when the parent thing lacks one.
#include <stdio.h>
#include <string.h>
static int g_base = 0, g_added = 0;
struct CThing { bool HasEntityEvents; bool HasTC() const { return HasEntityEvents; }
    void AddTC(const char* name, bool activate, const void* params) { if (!strcmp(name, "CTCEntityEvents") && !activate && !params) { ++g_added; HasEntityEvents = true; } } };
struct CTCBase { CThing* ParentThing; void OnInitialActivate() { ++g_base; } };
struct CTCActionUseBase : CTCBase {
    void OnInitialActivate() { CTCBase::OnInitialActivate(); if (!ParentThing->HasTC()) ParentThing->AddTC("CTCEntityEvents", false, 0); }
};
int main()
{
    CThing t; t.HasEntityEvents = false; CTCActionUseBase a; a.ParentThing = &t;
    a.OnInitialActivate(); a.OnInitialActivate();
    int ok = g_base == 2 && g_added == 1;
    printf(ok ? "007eac70_TEST PASS\n" : "007eac70_TEST FAIL\n");
    return ok ? 0 : 1;
}