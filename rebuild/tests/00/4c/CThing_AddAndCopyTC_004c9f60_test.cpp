// Behaviour test for CThing::AddAndCopyTC (0x004C9F60): a model of the row -- a component named like the source is added (with the
// flag passed through), then the thing's component of the source's interface type copies the source.
#include <stdio.h>
#include <string.h>
struct Comp { int type; int value; const char* name; void Copy(const Comp& o) { value = o.value; } };
struct Thing { Comp slots[4]; int added; bool lastFlag; const char* lastName;
    void AddTC(const char* name, bool flag) { ++added; lastFlag = flag; lastName = name; }
    bool GetTC(int type, Comp** out) { for (int i = 0; i < 4; ++i) if (slots[i].type == type) { *out = &slots[i]; return true; } return false; }
    void AddAndCopyTC(const Comp& other, bool flag) { AddTC(other.name, flag); Comp* tc = 0; GetTC(other.type, &tc); if (tc) tc->Copy(other); } };
int main()
{
    Thing t = { { { 1, 0, "a" }, { 7, 0, "b" }, { 9, 0, "c" }, { 11, 0, "d" } }, 0, false, 0 };
    Comp src = { 7, 42, "CTCFoo" };
    t.AddAndCopyTC(src, true);
    int ok = t.added == 1 && t.lastFlag && strcmp(t.lastName, "CTCFoo") == 0 && t.slots[1].value == 42 && t.slots[0].value == 0;
    printf(ok ? "004c9f60_TEST PASS\n" : "004c9f60_TEST FAIL\n");
    return ok ? 0 : 1;
}